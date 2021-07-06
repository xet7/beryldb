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

#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
#include <sys/sysctl.h>

namespace
{
	int SocketHandler;
	unsigned int ChangePos = 0;

	std::vector<struct kevent> klist(16);

	std::vector<struct kevent> pendinglist(8);

#if defined __NetBSD__ && __NetBSD_Version__ <= 999001400
	inline intptr_t udata_cast(EventHandler* ehandler)
	{
		return reinterpret_cast<intptr_t>(ehandler);
	}
#else
	inline void* udata_cast(EventHandler* ehandler)
	{
		return static_cast<void*>(ehandler);
	}
#endif
}

void SocketPool::Start()
{
	get_max_fdesc();
	SafeInit();
}

void SocketPool::SafeInit()
{
	SocketHandler = kqueue();

	if (SocketHandler == -1)
	{
		InitError();
	}
}

void SocketPool::CloseAll()
{
	Close(SocketHandler);
}

static struct kevent* ReqChangeKE()
{
	if (ChangePos >= pendinglist.size())
	{
		pendinglist.resize(pendinglist.size() * 2);
	}
	
	return &pendinglist[ChangePos++];
}

bool SocketPool::AddDescriptor(EventHandler* ehandler, int req_mask)
{
	int fd = ehandler->GetDescriptor();

	if (fd < 0)
	{
		return false;
	}

	if (!SocketPool::AttachFileDescRef(ehandler))
	{
		return false;
	}

	struct kevent* ke = ReqChangeKE();
	EV_SET(ke, fd, EVFILT_READ, EV_ADD, 0, 0, udata_cast(ehandler));

	ehandler->SetReqMask(req_mask);
	OnMaskReq(ehandler, 0, req_mask);
	ResizeDouble(klist);

	return true;
}

void SocketPool::DeleteDescriptor(EventHandler* ehandler)
{
	int fd = ehandler->GetDescriptor();

	if (fd < 0)
	{
		return;
	}

	struct kevent* ke = ReqChangeKE();
	EV_SET(ke, ehandler->GetDescriptor(), EVFILT_WRITE, EV_DELETE, 0, 0, NULL);

	ke = ReqChangeKE();
	EV_SET(ke, ehandler->GetDescriptor(), EVFILT_READ, EV_DELETE, 0, 0, NULL);

	SocketPool::DeleteFileDescRef(ehandler);
}

void SocketPool::OnMaskReq(EventHandler* ehandler, int old_mask, int new_mask)
{
	if ((new_mask & Q_REQ_POLL_WRITE) && !(old_mask & Q_REQ_POLL_WRITE))
	{
		struct kevent* ke = ReqChangeKE();
		EV_SET(ke, ehandler->GetDescriptor(), EVFILT_WRITE, EV_ADD, 0, 0, udata_cast(ehandler));
	}
	else if ((old_mask & Q_REQ_POLL_WRITE) && !(new_mask & Q_REQ_POLL_WRITE))
	{
		struct kevent* ke = ReqChangeKE();
		EV_SET(ke, ehandler->GetDescriptor(), EVFILT_WRITE, EV_DELETE, 0, 0, NULL);
	}
	
	if ((new_mask & (Q_FAST_WRITE | Q_SINGLE_WRITE)) && !(old_mask & (Q_FAST_WRITE | Q_SINGLE_WRITE)))
	{
		struct kevent* ke = ReqChangeKE();
		EV_SET(ke, ehandler->GetDescriptor(), EVFILT_WRITE, EV_ADD | EV_ONESHOT, 0, 0, udata_cast(ehandler));
	}
}

int SocketPool::Events()
{
	struct timespec ts;
	ts.tv_nsec = 0;
	ts.tv_sec = 1;

	int i = kevent(SocketHandler, &pendinglist.front(), ChangePos, &klist.front(), klist.size(), &ts);
	ChangePos = 0;
	Kernel->Now();

	if (i < 0)
	{
		return i;
	}

	for (int j = 0; j < i; j++)
	{
		struct kevent& kev = klist[j];
		EventHandler* ehandler = reinterpret_cast<EventHandler*>(kev.udata);

		if (!ehandler)
		{
			continue;
		}

		const int fd = ehandler->GetDescriptor();
		const short filter = kev.filter;
		
		if (fd < 0)
		{
			continue;
		}

		if (kev.flags & EV_EOF)
		{
			ehandler->OnManageError(kev.fflags);
			continue;
		}

		if (filter == EVFILT_WRITE)
		{
			const int bits_to_clr = Q_SINGLE_WRITE | Q_FAST_WRITE | Q_WRITE_BLOCK;
			ehandler->SetReqMask(ehandler->GetEventMask() & ~bits_to_clr);
			ehandler->OnPendingWrites();
		}
		else if (filter == EVFILT_READ)
		{
			ehandler->SetReqMask(ehandler->GetEventMask() & ~Q_READ_WILL_BLOCK);
			ehandler->OnPendingRead();
		}
	}

	return i;
}
