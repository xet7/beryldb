/*
 * BerylDB - A modular database.
 * http://www.beryldb.com
 *
 * Copyright (C) 2015-2021 Carlos F. Ferry <cferry@beryldb.com>
 * 
 * This file is part of BerylDB. BerylDB is free software: you can
 * redistribute it and/or modify it under the terms of the BSD License
 * version 3.
 *
 * More information about our licensing can be found at https://docs.beryl.dev
 */

#include "exit.h"
#include "beryl.h"

#include <iostream>

std::vector<EventHandler*> SocketPool::ref;

size_t SocketPool::FDSize = 0;

std::set<int> SocketPool::trials;

size_t SocketPool::set_size_limit = 0;

EventHandler::EventHandler()
{
	fd = -1;
	req_mask = 0;
}

void EventHandler::swap_internal(EventHandler& other)
{
	std::swap(fd, other.fd);
	std::swap(req_mask, other.req_mask);
}

void EventHandler::SetFileDesc(int FD)
{
	this->fd = FD;
}

void EventHandler::OnPendingWrites()
{
}

void EventHandler::OnManageError(int errornum)
{
}

void SocketPool::InitError()
{
	bprint(ERROR, "Error with socket stream: %s", strerror(errno));
	exit(EXIT_CODE_SOCKETSTREAM);
}

void SocketPool::get_max_fdesc()
{
	struct rlimit limits;

	if (!getrlimit(RLIMIT_NOFILE, &limits))
	{
		set_size_limit = limits.rlim_cur;
	}

	limits.rlim_cur = limits.rlim_max;

	if (!setrlimit(RLIMIT_NOFILE, &limits))
	{
		set_size_limit = limits.rlim_cur;
	}
}

void SocketPool::EventSwitch(EventHandler* ehandler, int change)
{
	int old_m = ehandler->req_mask;
	int new_m = old_m;

	if (change & Q_REQ_READ_MASK)
	{
		new_m &= ~Q_REQ_READ_MASK;
	}

	if (change & Q_REQ_WRITE_MASK)
	{
		new_m &= ~Q_REQ_WRITE_MASK;
	}

	if (change & Q_TRIAL_NOTE_MASK && !(old_m & Q_TRIAL_NOTE_MASK))
	{
		trials.insert(ehandler->GetDescriptor());
	}

	new_m |= change;

	if (new_m == old_m)
	{
		return;
	}

	ehandler->req_mask = new_m;
	OnMaskReq(ehandler, old_m, new_m);
}

void SocketPool::Writes()
{
	std::vector<int> pendinglist;
	pendinglist.reserve(trials.size());
	pendinglist.assign(trials.begin(), trials.end());
	trials.clear();
	
	for (unsigned int i=0; i < pendinglist.size(); i++)
	{
		int fd = pendinglist[i];
		EventHandler* ehandler = GetReference(fd);

		if (!ehandler)
		{
			continue;
		}

		int mask = ehandler->req_mask;
		ehandler->req_mask &= ~(Q_ADD_TRIAL_READ | Q_ADD_WRITE_TRIAL);

		if ((mask & (Q_ADD_TRIAL_READ | Q_READ_WILL_BLOCK)) == Q_ADD_TRIAL_READ)
		{
			ehandler->OnPendingRead();
		}
		
		if ((mask & (Q_ADD_WRITE_TRIAL | Q_WRITE_BLOCK)) == Q_ADD_WRITE_TRIAL)
		{
			ehandler->OnPendingWrites();
		}
	}
}

bool SocketPool::AttachFileDescRef(EventHandler* ehandler)
{
	int fd = ehandler->GetDescriptor();

	if (HasFileDesc(fd))
	{
		return false;
	}

	while (static_cast<unsigned int>(fd) >= ref.size())
	{
		ref.resize(ref.empty() ? 1 : (ref.size() * 2));
	}
	
	ref[fd] = ehandler;
	FDSize++;
	return true;
}

void SocketPool::DeleteFileDescRef(EventHandler *ehandler)
{
	int fd = ehandler->GetDescriptor();

	if (GetReference(fd) == ehandler)
	{
		ref[fd] = NULL;
		FDSize--;
	}
}

bool SocketPool::HasFileDesc(int fd)
{
	return GetReference(fd) != NULL;
}

EventHandler* SocketPool::GetReference(int fd)
{
	if (fd < 0 || static_cast<unsigned int>(fd) >= ref.size())
	{
		return NULL;
	}
	
	return ref[fd];
}

bool SocketPool::BoundsCheckFd(EventHandler* ehandler)
{
	return ehandler && ehandler->HasFileDesc();
}

int SocketPool::Accept(EventHandler* fd, sockaddr *addr, socklen_t *addrlen)
{
	return accept(fd->GetDescriptor(), addr, addrlen);
}

int SocketPool::Close(EventHandler* ehandler)
{
	DeleteDescriptor(ehandler);
	int ret = Close(ehandler->GetDescriptor());
	ehandler->SetFileDesc(-1);
	return ret;
}

int SocketPool::Close(int fd)
{
	return close(fd);
}

int SocketPool::Blocking(int fd)
{
	int flags = fcntl(fd, F_GETFL, 0);
	return fcntl(fd, F_SETFL, flags & ~O_NONBLOCK);
}

int SocketPool::NonBlocking(int fd)
{
	int flags = fcntl(fd, F_GETFL, 0);
	return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

void SocketPool::SetReusable(int fd)
{
	int on = 1;
	setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char*)&on, sizeof(on));
}

int SocketPool::RecvFrom(EventHandler* fd, void *buf, size_t len, int flags, sockaddr *from, socklen_t *fromlen)
{
	return recvfrom(fd->GetDescriptor(), (char*)buf, len, flags, from, fromlen);
}

int SocketPool::Send(EventHandler* fd, const void *buf, size_t len, int flags)
{
	return send(fd->GetDescriptor(), (const char*)buf, len, flags);
}

int SocketPool::Recv(EventHandler* fd, void *buf, size_t len, int flags)
{
	return recv(fd->GetDescriptor(), (char*)buf, len, flags);
}

int SocketPool::SendTo(EventHandler* fd, const void* buf, size_t len, int flags, const engine::sockets::sockaddrs& address)
{
	return sendto(fd->GetDescriptor(), (const char*)buf, len, flags, &address.sa, address.sa_size());
}

int SocketPool::WriteV(EventHandler* fd, const IOVector* iovec, int count)
{
	return writev(fd->GetDescriptor(), iovec, count);
}


int SocketPool::Connect(EventHandler* fd, const engine::sockets::sockaddrs& address)
{
	return connect(fd->GetDescriptor(), &address.sa, address.sa_size());
}

int SocketPool::Shutdown(EventHandler* fd, int how)
{
	return shutdown(fd->GetDescriptor(), how);
}

int SocketPool::Bind(int fd, const engine::sockets::sockaddrs& addr)
{
	return bind(fd, &addr.sa, addr.sa_size());
}

int SocketPool::Listen(int sockfd, int backlog)
{
	return listen(sockfd, backlog);
}

int SocketPool::Shutdown(int fd, int how)
{
	return shutdown(fd, how);
}

std::string SocketPool::LastError()
{
	return strerror(errno);
}

std::string SocketPool::GetError(int errnum)
{
	return strerror(errnum);
}

