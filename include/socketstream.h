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

#pragma once

#include <vector>
#include <string>
#include <map>
#include "config.h"
#include "socket.h"
#include "base.h"

#include <sys/uio.h>

#ifndef iov_max
#define iov_max 128
#endif

enum EventMask
{
	
	Q_NO_READ = 0x1,
	
	/*  */
	
	Q_REQ_POOL_READ = 0x2,
	
	Q_FAST_READ = 0x4,
	
	Q_EDGE_READ = 0x8,

	/* Read events */
	
	Q_REQ_READ_MASK = 0x0F,

	Q_NO_WRITE = 0x10,
	
	Q_REQ_POLL_WRITE = 0x20,
	
	Q_FAST_WRITE = 0x40,
	
	Q_EDGE_WRITE = 0x80,
	
	Q_SINGLE_WRITE = 0x100,

	
	Q_REQ_WRITE_MASK = 0x1F0,

	
	Q_ADD_TRIAL_READ  = 0x1000,
	
	Q_READ_WILL_BLOCK = 0x2000,

	
	Q_ADD_WRITE_TRIAL = 0x4000,
	
	Q_WRITE_BLOCK = 0x8000,

	
	Q_TRIAL_NOTE_MASK = 0x5000
};


class Externalize EventHandler : public base_class
{
 private:
	
	int req_mask;

	void SetReqMask(int mask) { req_mask = mask; }

 protected:
	
	int fd;

	
	void swap_internal(EventHandler& other);

 public:
	
	inline int GetDescriptor() const 
	{ 
		return fd; 
	}
	
	inline bool HasFileDesc() const 
	{ 
		return fd >= 0; 
	}

	inline int GetEventMask() const 
	{ 
		return req_mask; 
	}

	
	void SetFileDesc(int FD);

	
	EventHandler();

	
	virtual ~EventHandler() {}

	
	virtual void OnPendingRead() = 0;

	
	virtual void OnPendingWrites();

	
	virtual void OnManageError(int errornum);

	friend class SocketPool;
};



class Externalize SocketPool
{

 private:
	 	
	static std::vector<EventHandler*> ref;
	
	static size_t FDSize;
	
	static size_t set_size_limit;

	static std::set<int> trials;

	static void get_max_fdesc();
	
	static void InitError();

	static void OnMaskReq(EventHandler* ehandler, int old_mask, int new_mask);

	static bool AttachFileDescRef(EventHandler* ehandler);

	static void DeleteFileDescRef(EventHandler* ehandler);

	template <typename T>
	static void ResizeDouble(std::vector<T>& vect)
	{
		if (SocketPool::FDSize > vect.size())
		{
			vect.resize(SocketPool::FDSize * 2);
		}
	}

   public:

	typedef iovec IOVector;

	
	static void Start();

	static void CloseAll();

	static bool AddDescriptor(EventHandler* ehandler, int req_mask);

	static void EventSwitch(EventHandler* ehandler, int req_mask);
	
	static size_t GetMaxFds() 
	{ 
		return set_size_limit; 
	}
	
	static size_t GetUsedFds() 
	{ 
		return FDSize; 
	}

	
	static void DeleteDescriptor(EventHandler* ehandler);
	
	static bool HasFileDesc(int fd);

	
	static EventHandler* GetReference(int fd);

	static int Events();

	
	static void Writes();

	
	static bool BoundsCheckFd(EventHandler* eh);

	
	static int Accept(EventHandler* fd, sockaddr *addr, socklen_t *addrlen);

	
	static int Close(EventHandler* eh);

	
	static int Close(int fd);

	
	static int Send(EventHandler* fd, const void *buf, size_t len, int flags);

	
	static int WriteV(EventHandler* fd, const IOVector* iov, int count);

	static int Recv(EventHandler* fd, void *buf, size_t len, int flags);

	static int RecvFrom(EventHandler* fd, void *buf, size_t len, int flags, sockaddr *from, socklen_t *fromlen);

	
	static int SendTo(EventHandler* fd, const void* buf, size_t len, int flags, const engine::sockets::sockaddrs& address);


	static int Connect(EventHandler* fd, const engine::sockets::sockaddrs& address);


	static int Blocking(int fd);

	
	static int NonBlocking(int fd);

	
	static int Shutdown(EventHandler* fd, int how);

	
	static int Shutdown(int fd, int how);

	
	static int Bind(int fd, const engine::sockets::sockaddrs& addr);

	
	static int Listen(int sockfd, int backlog);

	
	static void SetReusable(int sockfd);


	static void SafeInit();


	
	static bool IgnoreError();

	
	static std::string LastError();

	
	static std::string GetError(int errnum);
};

inline bool SocketPool::IgnoreError()
{
	if ((errno == EAGAIN) || (errno == EWOULDBLOCK))
	{
		return true;
	}

	return false;
}
