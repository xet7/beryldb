/*
 * BerylDB - A lightweight database.
 * http://www.beryldb.com
 *
 * Copyright (C) 2021 - Carlos F. Ferry <cferry@beryldb.com>
 * 
 * This file is part of BerylDB. BerylDB is free software: you can
 * redistribute it and/or modify it under the terms of the BSD License
 * version 3.
 *
 * More information about our licensing can be found at https://docs.beryl.dev
 */

#include "beryl.h"
#include "queues.h"

SocketTimer::SocketTimer(int fd, std::shared_ptr<LiveSocket> thesock, unsigned int add_secs) : Timer(add_secs), sock(std::move(thesock)), sfd(fd)
{

}

static IOQueue* GetNextQueue(IOQueue* attach)
{
	IOQueueMiddle* const iohm = IOQueueMiddle::ToMiddleQueue(attach);

	if (iohm)
	{
		return iohm->GetNextQueue();
	}
	
	return NULL;
}

LiveSocket::LiveSocket()
{
	Timeout = NULL;
	state = S_ERROR;
}

LiveSocket::LiveSocket(int newfd)
{
	Timeout = NULL;
	this->fd = newfd;
	this->state = S_FULL_CONNECTED;

	if (HasFileDesc())
	{
		SocketPool::AddDescriptor(this, Q_FAST_READ | Q_EDGE_WRITE);
	}
}

void LiveSocket::InitConnection(const engine::sockets::sockaddrs& dest, const engine::sockets::sockaddrs& bind, unsigned int maxtime)
{
	LiveSocketError err = TryLink(dest, bind, maxtime);

	if (err != L_ERR_NONE)
	{
		state = S_ERROR;
		SetError(SocketPool::LastError());
		OnError(err);
	}
}

LiveSocketError LiveSocket::TryLink(const engine::sockets::sockaddrs& dest, const engine::sockets::sockaddrs& bind, unsigned int timeout)
{
	if (!HasFileDesc())
	{
		fd = socket(dest.family(), SOCK_STREAM, 0);
	}

	if (!HasFileDesc())
	{
		return L_ERR_SOCKET;
	}

	if (bind.family() != 0)
	{
		if (SocketPool::Bind(fd, bind) < 0)
		{
			return L_ERR_BIND;
		}
	}

	SocketPool::NonBlocking(fd);

	if (SocketPool::Connect(this, dest) == -1)
	{
		if (errno != EINPROGRESS)
		{
			return L_ERR_CONNECT;
		}
	}

	this->state = S_CONNECTING;

	if (!SocketPool::AddDescriptor(this, Q_NO_READ | Q_SINGLE_WRITE | Q_WRITE_BLOCK))
	{
		return L_ERR_NOMOREFDS;
	}

	this->Timeout = new SocketTimer(this->GetDescriptor(), std::shared_ptr<LiveSocket>(this), timeout);
	Kernel->Tickers->Add(std::shared_ptr<Timer>(this->Timeout));

	return L_ERR_NONE;
}

void StreamSocket::Close()
{
	if (closing)
	{
		return;
	}

	closing = true;
	
	if (HasFileDesc())
	{
		QueueWrite();

		IOQueue* attach = GetIOQueue();
		DelIOQueue();
	
		while (attach)
		{
			attach->OnStreamSocketClose(this);
			IOQueue* const nextattach = GetNextQueue(attach);
			delete attach;
			attach = nextattach;
		}
		
		SocketPool::Shutdown(this, 2);
		SocketPool::Close(this);
	}
}

void StreamSocket::Close(bool writeblock)
{
	if (Getsend_queueSize() != 0 && writeblock)
		closeonempty = true;
	else
		Close();
}

DiscardResult StreamSocket::discard()
{
	Close();
	return EventHandler::discard();
}

bool StreamSocket::find_next_line(std::string& line, char delim)
{
	std::string::size_type i = recvq.find(delim);

	if (i == std::string::npos)
	{
		return false;
	}
	
	line.assign(recvq, 0, i);
	recvq.erase(0, i + 1);
	return true;
}

int StreamSocket::QueueChainRead(IOQueue* attach, std::string& rq)
{
	if (!attach)
	{
		return ProcessRevQueue(rq);
	}

	IOQueueMiddle* const iohm = IOQueueMiddle::ToMiddleQueue(attach);
	
	if (iohm)
	{
		const int ret = QueueChainRead(iohm->GetNextQueue(), iohm->GetRecvQ());
	
		if (ret <= 0)
		{
			return ret;
		}
	}
	
	return attach->OnStreamSocketRead(this, rq);
}

void StreamSocket::StreamRead()
{
	const std::string::size_type prevrecvqsize = recvq.size();

	const int result = QueueChainRead(GetIOQueue(), recvq);
	
	if (result < 0)
	{
		SetError("Read Error");
		return;
	}

	if (recvq.size() > prevrecvqsize)
	{
		StreamData();
	}
}

int StreamSocket::ProcessRevQueue(std::string& rq)
{
	char* PendingBuffer = Kernel->GetPendingBuffer();
	int n = SocketPool::Recv(this, PendingBuffer, BUFFERSIZE, 0);
				
	if (n == BUFFERSIZE)
	{
		SocketPool::EventSwitch(this, Q_FAST_READ | Q_ADD_TRIAL_READ);
		rq.append(PendingBuffer, n);
		Kernel->Interval = 0;
	}
	else if (n > 0)
	{
		SocketPool::EventSwitch(this, Q_FAST_READ);
		rq.append(PendingBuffer, n);
                Kernel->Interval = 0;
	}
	else if (n == 0)
	{
		error = "Connection closed";
		SocketPool::EventSwitch(this, Q_NO_READ | Q_NO_WRITE);
		return -1;
	}
	else if (SocketPool::IgnoreError())
	{
		SocketPool::EventSwitch(this, Q_FAST_READ | Q_READ_WILL_BLOCK);
		return 0;
	}
	else if (errno == EINTR)
	{
		SocketPool::EventSwitch(this, Q_FAST_READ | Q_ADD_TRIAL_READ);
		return 0;
	}
	else
	{
		error = SocketPool::LastError();
		SocketPool::EventSwitch(this, Q_NO_READ | Q_NO_WRITE);
		return -1;
	}

	return n;
}

static const int use_iov_max = iov_max < 128 ? iov_max : 128;

void StreamSocket::QueueWrite()
{
	if (Getsend_queueSize() == 0)
	{
		if (closeonempty)
			Close();

		return;
	}
	if (!error.empty() || !HasFileDesc())
	{
		slog("SOCKET", LOG_DEBUG, "QueueWrite on errored or closed socket");
		return;
	}

	send_queue* psendq = &sendq;
	IOQueue* attach = GetIOQueue();
	
	while (attach)
	{
		int rv = attach->OnStreamSocketWrite(this, *psendq);
		psendq = NULL;

		if (rv == 0)
		{
			break;
		}

		if (rv < 0)
		{
			SetError("Write Error");
			break;
		}

		IOQueueMiddle* const iohm = IOQueueMiddle::ToMiddleQueue(attach);
		attach = NULL;
		
		if (iohm)
		{
			psendq = &iohm->Getsend_queue();
			attach = iohm->GetNextQueue();
		}
	}

	if (psendq)
	{
		Dispatchsend_queue(*psendq);
	}

	if (Getsend_queueSize() == 0 && closeonempty)
	{
		Close();
	}
}

void StreamSocket::Dispatchsend_queue(send_queue& sq)
{
		if (GetEventMask() & Q_WRITE_BLOCK)
		{
			return;
		}
		
		int eventChange = Q_EDGE_WRITE;

		while (error.empty() && !sq.empty() && eventChange == Q_EDGE_WRITE)
		{
			int bufcount = sq.size();
			
			if (bufcount > use_iov_max)
			{
				bufcount = use_iov_max;
			}

			int rv_max = 0;
			int rv;
			{
				SocketPool::IOVector iovecs[use_iov_max];
				size_t j = 0;
				
				for (send_queue::const_iterator i = sq.begin(), end = i+bufcount; i != end; ++i, j++)
				{
					const send_queue::Element& elem = *i;
					iovecs[j].iov_base = const_cast<char*>(elem.data());
					iovecs[j].iov_len = elem.length();
					rv_max += iovecs[j].iov_len;
				}
		
				rv = SocketPool::WriteV(this, iovecs, bufcount);
			}

			if (rv == (int)sq.bytes())
			{
				sq.clear();
			}
			else if (rv > 0)
			{
				if (rv < rv_max)
				{
					eventChange = Q_FAST_WRITE | Q_WRITE_BLOCK;
				}
		
				while (rv > 0 && !sq.empty())
				{
					const send_queue::Element& front = sq.front();
		
					if (front.length() <= (size_t)rv)
					{
						rv -= front.length();
						sq.pop_front();
					}
					else
					{
						sq.erase_front(rv);
						rv = 0;
					}
				}
			}
			else if (rv == 0)
			{
				error = "Connection closed";
			}
			else if (SocketPool::IgnoreError())
			{
				eventChange = Q_FAST_WRITE | Q_WRITE_BLOCK;
			}
			else if (errno == EINTR)
			{
				errno = 0;
			}
			else
			{
				error = SocketPool::LastError();
			}
		}
		if (!error.empty())
		{
			SocketPool::EventSwitch(this, Q_NO_READ | Q_NO_WRITE);
		}
		else
		{
			SocketPool::EventSwitch(this, eventChange);
		}
}

bool StreamSocket::OnSetEndPoint(const engine::sockets::sockaddrs& local, const engine::sockets::sockaddrs& remote)
{
	return false;
}

void StreamSocket::AppendBuffer(const std::string &data)
{
	if (!HasFileDesc())
	{
		return;
	}

	sendq.push_back(data);
	SocketPool::EventSwitch(this, Q_ADD_WRITE_TRIAL);
}

bool SocketTimer::Run(time_t)
{
	if (SocketPool::GetReference(this->sfd) != this->sock.get())
	{
		delete this;
		return false;
	}

	if (this->sock->state == S_CONNECTING)
	{
		this->sock->OnTimeout();
		this->sock->OnError(L_ERR_TIMEOUT);
		this->sock->state = S_ERROR;

		Kernel->Reducer.Add(sock.get());
	}

	this->sock->Timeout = NULL;
	delete this;
	return false;
}

void LiveSocket::OnConnected() { }
void LiveSocket::OnTimeout() { return; }

void LiveSocket::OnPendingWrites()
{
	if (state == S_CONNECTING)
	{
		state = S_FULL_CONNECTED;
		this->OnConnected();
		if (!GetIOQueue())
			SocketPool::EventSwitch(this, Q_FAST_READ | Q_EDGE_WRITE);
	}
	this->StreamSocket::OnPendingWrites();
}

LiveSocket::~LiveSocket()
{
	this->Close();
	delete Timeout;
}

void StreamSocket::OnManageError(int errornum)
{
	if (!error.empty())
	{
		return;
	}

	if (errornum == 0)
		SetError("Connection closed");
	else
		SetError(SocketPool::GetError(errornum));

	LiveSocketError errcode = L_ERR_OTHER;
	switch (errornum)
	{
		case ETIMEDOUT:
			errcode = L_ERR_TIMEOUT;
			break;
		case ECONNREFUSED:
		case 0:
			errcode = L_ERR_CONNECT;
			break;
		case EADDRINUSE:
			errcode = L_ERR_BIND;
			break;
		case EPIPE:
		case EIO:
			errcode = L_ERR_WRITE;
			break;
	}

	CheckError(errcode);
}

void StreamSocket::OnPendingRead()
{
	if (!error.empty())
		return;

	try
	{
		StreamRead();
	}
	catch (KernelException& ex)
	{
		SetError(ex.get_reason());
	}

	CheckError(L_ERR_OTHER);
}

void StreamSocket::OnPendingWrites()
{
	if (!error.empty())
	{
		return;
	}

	QueueWrite();
	CheckError(L_ERR_OTHER);
}

void StreamSocket::CheckError(LiveSocketError errcode)
{
	if (!error.empty())
	{
		OnError(errcode);
	}
}

IOQueue* StreamSocket::GetModHook(Module* mod) const
{
	for (IOQueue* curr = GetIOQueue(); curr; curr = GetNextQueue(curr))
	{
		if (curr->prov->creator == mod)
		{
			return curr;
		}
	}

	return NULL;
}

IOQueue* StreamSocket::GetLastHook() const
{
	IOQueue* curr = GetIOQueue();
	IOQueue* last = curr;

	for (; curr; curr = GetNextQueue(curr))
	{
		last = curr;
	}

	return last;
}


void StreamSocket::AddIOQueue(IOQueue* newattach)
{
	IOQueue* curr = GetIOQueue();

	if (!curr)
	{
		ioattach = newattach;
		return;
	}

	IOQueueMiddle* lastattach;
	
	while (curr)
	{
		lastattach = IOQueueMiddle::ToMiddleQueue(curr);
	
		if (!lastattach)
		{
			return;
		}
		
		curr = lastattach->GetNextQueue();
	}

	lastattach->SetNextHook(newattach);
}

size_t StreamSocket::Getsend_queueSize() const
{
	size_t ret = sendq.bytes();
	IOQueue* curr = GetIOQueue();

	while (curr)
	{
		const IOQueueMiddle* const iohm = IOQueueMiddle::ToMiddleQueue(curr);
		
		if (!iohm)
		{
			break;
		}

		ret += iohm->Getsend_queue().bytes();
		curr = iohm->GetNextQueue();
	}

	return ret;
}

void StreamSocket::swap_internal(StreamSocket& other)
{
	if (type != other.type)
	{
		return;
	}

	EventHandler::swap_internal(other);
	std::swap(closeonempty, other.closeonempty);
	std::swap(closing, other.closing);
	std::swap(error, other.error);
	std::swap(ioattach, other.ioattach);
	std::swap(recvq, other.recvq);
	std::swap(sendq, other.sendq);
}
