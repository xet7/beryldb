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

#include <bitset>
#include <memory>
#include <atomic>

#include "socket.h"
#include "brldsocket.h"
#include "subscription.h"
#include "login.h"
#include "brldb/query.h"
#include "brldb/hquery.h"

/* Connect class flags. */

enum class_flags 
{
	CC_ALLOW = 0,
	CC_DENY  = 1,
	CC_NAMED = 2
};

enum registration_state 
{
	REG_NONE = 0,		/* The user has not sent any identification */
	REG_USER = 1,		
	REG_LOGIN = 2,		
	REG_LOGINUSER = 3, 	
	REG_OK = 7	  	
};

enum UserType 
{
	CLIENT_TYPE_LOCAL = 1,
	CLIENT_TYPE_REMOTE = 2,
	CLIENT_TYPE_SERVER = 3
};

struct Externalize connect_config : public refcountbase
{

	reference<config_rule> config;

	char type;

	/* This connection configuration name. */

	std::string name;
	
	std::string host;
	
	/* Connecting port */
	
	brld::flat_set<int> ports;
	
	connect_config(config_rule* tag, char type, const std::string& mask);
	
	connect_config(config_rule* tag, char type, const std::string& mask, const connect_config& parent);

	
	void Update(const connect_config* newSettings);

	const std::string& GetName() 
	{ 
		return name; 
	}
	
	const std::string& GetHost() 
	{ 
		return host; 
	}

};

/*
 * Class User maintains all information of a given user.
 * 
 * Information about an user varies, but it can be: time logged in,
 * file descriptors, current database and pending queries, etc.
 */

class Externalize User : public Expandable
{
   /* 
    * DataFlush needs to access this class in order to
    * retrieve pending queries and notifications. 
    */
    
   friend class DataFlush;
   
   private:
   
	std::string cached_hostip;
	
	std::string cached_user_real_host;
	
	std::string user_ip;
		
	std::string realhost;

	/* Atomic uint for quitting status. */
	
        std::atomic<unsigned int> quitting;
        
        /* Whether this user is processing queries. */
        
        bool Locked;
	
 public:

 	bool Blocked;
 	
	class for_each_neighbor_handler
	{
 	   public:

		virtual void Execute(LocalUser* user) = 0;
	};

	typedef brld::node_list<Subscription> SubsList;
	
        bool Paused;
	
	/* Time since logged. */
	
	time_t age;
	
	/* Time at which this connection was established. */

	time_t connected;

	std::string auth;

	/* login this user is using to access the server. */

	std::string login;
	
	/* Time at which this user was logged. */

	time_t logged;
	
	std::shared_ptr<HQuery> hquery;
	
	engine::sockets::sockaddrs client_sa;

	std::string instance;
	
        /* 
         * Sets quit flag to true or false.
         * 
         * @parameters:
	 *
	 *         · flag: User is either disconnecting or not.
         */    
         	
	void SetQuit(bool flag);
	
        /* 
         * Checks whether this user is disconnecting.
	 * 
         * @return:
 	 *
         *         · True: User is disconnecting.
         *         · False: Unable to disconnect.
         */    
         	
	bool IsQuitting();
	
	void SetLock(bool flag);
	
	bool IsLocked();
	
	const std::string uuid;

	/* Agent that identifies this user. */
	
	std::string agent;

	SubsList chans;
	
	Server* server;
	
	/* User's session. */
	
	std::shared_ptr<Session> session;

        /* 
         * Checks whether user has 'r' flags (can_admin).
         *
         * @return:
 	 *
         *         · True: User has 'r' flags.
         *         · False: User is not an admin.
         */    	
	
	bool IsAdmin();

	/* Returns users' flag, if any. */
	
	int GetFirstFlag();

	bool CanPerform(unsigned char flag);
	
        std::shared_ptr<Database> current_db;
	
        std::deque<std::shared_ptr<query_base>> pending;

        std::deque<std::shared_ptr<query_base>> notifications;

        std::string select;
        
	unsigned int registered:3;


        const unsigned int usertype:2;
	
	
	const std::string& GetReadableIP();


	
	const std::string& GetRealHost() const;

	
	engine::sockets::cidr_mask GetCompactIPMask();

	
	virtual bool set_client_ip(const std::string& address);

	virtual void set_client_ip(const engine::sockets::sockaddrs& sa);

	
	User(const std::string& uid, Server* srv, UserType objtype);

	
	virtual const std::string& GetHostFormat();

	
	void ResetCache();

	
	virtual void SendRemoteProtocol(const Numeric::Numeric& numeric);

	template <typename T1>
	void SendRemoteProtocol(unsigned int numeric, T1 p1)
	{
		Numeric::Numeric n(numeric);
		n.push(p1);
		SendRemoteProtocol(n);
	}

	template <typename T1, typename T2>
	void SendRemoteProtocol(unsigned int numeric, T1 p1, T2 p2)
	{
		Numeric::Numeric n(numeric);
		n.push(p1);
		n.push(p2);
		SendRemoteProtocol(n);
	}

	template <typename T1, typename T2, typename T3>
	void SendRemoteProtocol(unsigned int numeric, T1 p1, T2 p2, T3 p3)
	{
		Numeric::Numeric n(numeric);
		n.push(p1);
		n.push(p2);
		n.push(p3);
		SendRemoteProtocol(n);
	}

	template <typename T1, typename T2, typename T3, typename T4>
	void SendRemoteProtocol(unsigned int numeric, T1 p1, T2 p2, T3 p3, T4 p4)
	{
		Numeric::Numeric n(numeric);
		n.push(p1);
		n.push(p2);
		n.push(p3);
		n.push(p4);
		SendRemoteProtocol(n);
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5>
	void SendRemoteProtocol(unsigned int numeric, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5)
	{
		Numeric::Numeric n(numeric);
		n.push(p1);
		n.push(p2);
		n.push(p3);
		n.push(p4);
		n.push(p5);
		SendRemoteProtocol(n);
	}

	void SendProtocol(const Numeric::Numeric& numeric);

	template <typename T1>
	void SendProtocol(unsigned int numeric, T1 p1)
	{
		Numeric::Numeric n(numeric);
		n.push(p1);
		SendProtocol(n);
	}

	template <typename T1, typename T2>
	void SendProtocol(unsigned int numeric, T1 p1, T2 p2)
	{
		Numeric::Numeric n(numeric);
		n.push(p1);
		n.push(p2);
		SendProtocol(n);
	}

	template <typename T1, typename T2, typename T3>
	void SendProtocol(unsigned int numeric, T1 p1, T2 p2, T3 p3)
	{
		Numeric::Numeric n(numeric);
		n.push(p1);
		n.push(p2);
		n.push(p3);
		SendProtocol(n);
	}

	template <typename T1, typename T2, typename T3, typename T4>
	void SendProtocol(unsigned int numeric, T1 p1, T2 p2, T3 p3, T4 p4)
	{
		Numeric::Numeric n(numeric);
		n.push(p1);
		n.push(p2);
		n.push(p3);
		n.push(p4);
		SendProtocol(n);
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5>
	void SendProtocol(unsigned int numeric, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5)
	{
		Numeric::Numeric n(numeric);
		n.push(p1);
		n.push(p2);
		n.push(p3);
		n.push(p4);
		n.push(p5);
		SendProtocol(n);
	}

	
	void SendCommonRaw(ProtocolTrigger::Event& protoev, bool include_self = true);

	void for_each_neighbor(for_each_neighbor_handler& handler, bool include_self = true);

	
	bool CommonSubscription(User *other);

	
	bool SetPublicHost(const std::string& host);

	
	void SetHost(const std::string& host, bool resetdisplay);

	
	bool SetAgent(const std::string& newagent);

	
	bool SetLogin(const std::string& userlogin, time_t newts = 0);

	
	void CheckEmptyChannels();

	
	virtual ~User();
	DiscardResult discard() ;

	
	bool Deserialize(Data& data) ;

	
	bool Serialize(Serializable::Data& data) ;
};

class Externalize InstanceStream : public StreamSocket
{
 private:

	 size_t checked_until;

 public:

	LocalUser* const user;

	InstanceStream(LocalUser* me) : StreamSocket(StreamSocket::SS_USER), checked_until(0), user(me)
	{

	}

	void StreamData() ;
	bool OnSetEndPoint(const engine::sockets::sockaddrs& local, const engine::sockets::sockaddrs& remote) ;
	void OnError(LiveSocketError error) ;

	void swap_internal(InstanceStream& other);
};

typedef unsigned int sent_id;


class Externalize LocalUser : public User, public brld::node_list_node<LocalUser>
{
  private:
  
	void Write(const ProtocolTrigger::SerializedMessage& serialized);
	
	void Send(ProtocolTrigger::Event& protoev, ProtocolTrigger::MessageList& msglist);
	
	static ProtocolTrigger::MessageList SendMsgList;

 public:

	LocalUser(int fd, engine::sockets::sockaddrs* client, engine::sockets::sockaddrs* server);

	LocalUser(int fd, const std::string& uuid, Serializable::Data& data);

	DiscardResult discard();

	InstanceStream usercon;

	ProtocolTrigger::Serializer* serializer;
	
	reference<connect_config> assigned_class;

	
	connect_config* GetClass() const { return assigned_class; }

	
	void check_con_conf(bool clone_count = true);

	
	engine::sockets::sockaddrs server_sa;

	unsigned int lastping:1;

	unsigned int exempt:1;

	/* Expected next ping. */
	
	time_t next_ping_check;

	/* Time at which this user sent his/her last message. */

	time_t touchbase;

	sent_id already_sent;

	/* Function used to successfuly connect an user. */

	void ack_connection();

	/* 
	 * Sets a connection class where this user should be bound to.
	 */

	void set_class(const std::string &classname = "");

	/* Sets this user's IP address */

	bool set_client_ip(const std::string& address) ;

	void set_client_ip(const engine::sockets::sockaddrs& sa) ;

	
	void WriteRemoteNotice(const std::string& text) ;

	
	void Send(ProtocolTrigger::Event& protoev);

	
	void Send(ProtocolTrigger::EventProvider& protoevprov, ProtocolTrigger::Message& msg);

	
	bool Deserialize(Data& data) ;

	
	bool Serialize(Serializable::Data& data) ;
};

class RemoteUser : public User
{
   public:
   
	RemoteUser(const std::string& uid, Server* srv) : User(uid, srv, CLIENT_TYPE_REMOTE)
	{

	}
};

class Externalize GlobalUser : public User
{
   public:

	GlobalUser(const std::string& uid, Server* srv) : User(uid, srv, CLIENT_TYPE_SERVER)	
	{
		instance = srv->GetName();
	}

	GlobalUser(const std::string& uid, const std::string& sname) : User(uid, new Server(uid, sname), CLIENT_TYPE_SERVER)
	{
		instance = sname;
	}

	DiscardResult discard();
	
	const std::string& GetHostFormat();
};

inline LocalUser* IS_LOCAL(User* usr)
{
	return (usr != NULL && usr->usertype == CLIENT_TYPE_LOCAL) ? static_cast<LocalUser*>(usr) : NULL;
}

inline RemoteUser* IS_REMOTE(User* usr)
{
	return (usr != NULL && usr->usertype == CLIENT_TYPE_REMOTE) ? static_cast<RemoteUser*>(usr) : NULL;
}

inline GlobalUser* IS_SERVER(User* usr)
{
	return (usr != NULL && usr->usertype == CLIENT_TYPE_SERVER) ? static_cast<GlobalUser*>(usr) : NULL;
}



