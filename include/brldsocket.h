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

#pragma once

#include "timer.h"

class IOQueue;

enum LiveSocketState
{
	/* Socket disconnected. */

	S_DISCONNECTED,

	/* Socket connecting. */

	S_CONNECTING,

	/* Socket connected. */

	S_FULL_CONNECTED,

	/* An error occured with socket. */	

	S_ERROR
};

/* Potential errors a socket may experience. */

enum LiveSocketError
{
	
	L_ERR_NONE,
	
	L_ERR_DISCONNECT,
	
	L_ERR_TIMEOUT,
	
	L_ERR_SOCKET,
	
	L_ERR_CONNECT,
	
	L_ERR_BIND,
	
	L_ERR_WRITE,
	
	L_ERR_NOMOREFDS,
	
	L_ERR_OTHER
};


class LiveSocket;

/* Used to timeout socket connections. */

class ExportAPI SocketTimer : public Timer
{
 private:
	
	std::shared_ptr<LiveSocket> sock;
	
	int sfd;

 public:
	
	SocketTimer(int fd, std::shared_ptr<LiveSocket> thesock, unsigned int add_secs);
	
	bool Run(time_t now);
};


class ExportAPI StreamSocket : public EventHandler
{
 public:
	
	class send_queue
	{
  	  public:
		
		typedef std::string Element;

		
		typedef std::deque<Element> Container;

		
		typedef Container::const_iterator const_iterator;

		send_queue() : nbytes(0) { }

		
		bool empty() const { return (nbytes == 0); }

		
		Container::size_type size() const { return data.size(); }

		
		size_t bytes() const { return nbytes; }

		
		const Element& front() const { return data.front(); }

		
		const_iterator begin() const { return data.begin(); }

		
		const_iterator end() const { return data.end(); }

		
		void pop_front()
		{
			nbytes -= data.front().length();
			data.pop_front();
		}

		
		void erase_front(Element::size_type n)
		{
			nbytes -= n;
			data.front().erase(0, n);
		}

		
		void push_front(const Element& newdata)
		{
			data.push_front(newdata);
			nbytes += newdata.length();
		}

		
		void push_back(const Element& newdata)
		{
			data.push_back(newdata);
			nbytes += newdata.length();
		}

		
		void clear()
		{
			data.clear();
			nbytes = 0;
		}

		void moveall(send_queue& other)
		{
			nbytes += other.bytes();
			data.insert(data.end(), other.data.begin(), other.data.end());
			other.clear();
		}

	 private:
	 	
		Container data;

		
		size_t nbytes;
	};

	
	enum Type
	{
		SS_UNKNOWN,
		SS_USER
	};

 private:
	
	bool closeonempty;

	
	bool closing;

	
	IOQueue* ioattach;

	
	send_queue sendq;

	
	std::string error;

	
	void CheckError(LiveSocketError err);

	
	void StreamRead();

	
	void Dispatchsend_queue(send_queue& sq);

	
	int ProcessRevQueue(std::string& rq);

	
	int QueueChainRead(IOQueue* attach, std::string& rq);

 protected:
	
	std::string recvq;

	
	void swap_internal(StreamSocket& other);

 public:
	const Type type;
	
	StreamSocket(Type sstype = SS_UNKNOWN)
		: closeonempty(false)
		, closing(false)
		, ioattach(NULL)
		, type(sstype)
	{
	
	}
	
	IOQueue* GetIOQueue() const;
	void AddIOQueue(IOQueue* attach);
	void DelIOQueue();

	
	void QueueWrite();

	
	void OnPendingRead() ;

	
	void OnPendingWrites() ;

	
	void OnManageError(int errcode) ;

	
	void SetError(const std::string& err) { if (error.empty()) error = err; }

	
	const std::string& get_error() const 
	{ 
		return error; 
	}

	
	virtual void StreamData() = 0;
	
	virtual void OnError(LiveSocketError e) = 0;

	
	virtual bool OnSetEndPoint(const engine::sockets::sockaddrs& local, const engine::sockets::sockaddrs& remote);

	
	void AppendBuffer(const std::string& data);
	
	bool find_next_line(std::string& line, char delim = '\n');
	
	size_t GetQueueSize() const;

	send_queue& Getsend_queue() { return sendq; }

	
	virtual void Close();

	
	void Close(bool writeblock);

	
	DiscardResult discard() ;

	
	IOQueue* GetModuleLink(Module* mod) const;

	
	IOQueue* GetLastHook() const;
};


class ExportAPI LiveSocket : public StreamSocket
{
 public:
	
	SocketTimer* Timeout;

	/* 
	 * The state for given socket. A socket state may be
	 * either listening, connected, error or connecting.
	 */
	 
	LiveSocketState state;

	LiveSocket();
	
	LiveSocket(int newfd);

	
	void InitConnection(const engine::sockets::sockaddrs& dest, const engine::sockets::sockaddrs& bind, unsigned int maxtime);

	
	virtual void OnConnected();

	
	void StreamData()  = 0;

	
	virtual void OnTimeout();

	virtual ~LiveSocket();
 
 protected:
 
	void OnPendingWrites() ;
	LiveSocketError TryLink(const engine::sockets::sockaddrs& dest, const engine::sockets::sockaddrs& bind, unsigned int timeout);
};

inline IOQueue* StreamSocket::GetIOQueue() const 
{ 
	return ioattach; 
}

inline void StreamSocket::DelIOQueue() 
{ 
	ioattach = NULL; 
}
