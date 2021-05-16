/*
 * BerylDB - A modular database.
 * http://www.beryldb.com
 *
 * Copyright (C) 2021 Carlos F. Ferry <cferry@beryldb.com>
 * 
 * This file is part of BerylDB. BerylDB is free software: you can
 * redistribute it and/or modify it under the terms of the BSD License
 * version 3.
 *
 * More information about our licensing can be found at https://docs.beryl.dev
 */

#pragma once

class StreamSocket;

class QueueProvider : public refcountbase, public ServiceProvider
{
  private:
  
 	const bool middleattach;

  public:
  
	enum Type
	{
		IOH_UNKNOWN,
		IOH_SSL
	};

	const Type type;

	
	QueueProvider(Module* mod, const std::string& Name, Type attachtype = IOH_UNKNOWN, bool middle = false)
		: ServiceProvider(mod, Name, SERVICE_QUEUE), middleattach(middle), type(attachtype) { }

	
	bool IsMiddle() const 
	{ 
		return middleattach; 
	}

	
	virtual void OnAccept(StreamSocket* sock, engine::sockets::sockaddrs* client, engine::sockets::sockaddrs* server) = 0;

	
	virtual void OnConnect(StreamSocket* sock) = 0;
};

class IOQueue : public base_class
{
 public:
	
	reference<QueueProvider> prov;

	
	IOQueue(QueueProvider* provider): prov(provider) 
	{
	
	}
	
	virtual int OnStreamSocketWrite(StreamSocket* sock, StreamSocket::send_queue& sendq) = 0;

	
	virtual void OnStreamSocketClose(StreamSocket* sock) = 0;

	
	virtual int OnStreamSocketRead(StreamSocket* sock, std::string& recvq) = 0;
};

class IOQueueMiddle : public IOQueue
{
	
	StreamSocket::send_queue sendq;

	
	std::string precvq;

	
	IOQueue* nextattach;

 protected:
	
	std::string& GetRecvQ() { return precvq; }

	/*
	 * Obtain all queued data that is still awaiting
	 * to be passed onto the queue chain.
	 */
	
	StreamSocket::send_queue& Getsend_queue() 
	{ 
		return sendq; 
	}

 public:
	
	IOQueueMiddle(QueueProvider* provider) : IOQueue(provider), nextattach(NULL)
	{

	}

	
	const StreamSocket::send_queue& Getsend_queue() const 
	{ 
		return sendq; 
	}
	
	IOQueue* GetNextQueue() const 
	{ 
		return nextattach; 
	}

	
	void SetNextHook(IOQueue* attach) { nextattach = attach; }

	
	static IOQueueMiddle* ToMiddleQueue(IOQueue* attach)
	{
		if (attach->prov->IsMiddle())
		{
			return static_cast<IOQueueMiddle*>(attach);
		}
		
		return NULL;
	}

	friend class StreamSocket;
};
