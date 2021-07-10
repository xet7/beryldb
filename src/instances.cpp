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
#include "brldb/dbmanager.h"
#include "brldb/database.h"
#include "brldb/query.h"
#include "managers/settings.h"
#include "managers/user.h"

std::mutex User::db_mute;

ProtocolTrigger::MessageList LocalUser::SendMsgList;

User::User(const std::string& uid, Server* srv, UserType type) :
					  Locked(false)	 
				        , Multi(false)
				        , MultiRunning(false)
				        , Paused(false)
				        , age(Kernel->Now())
					, connected(0)
					, logged(0)
					, uuid(uid)
					, server(srv)
					, session(nullptr)
					, select("1")
					, registered(REG_NONE)
                                        , usertype(type)
                                        
					
{
	client_sa.sa.sa_family = AF_UNSPEC;

	if (type != CLIENT_TYPE_SERVER)
	{
		if (!Kernel->Clients.uuid_list.insert(std::make_pair(uuid, this)).second)
		{
			throw KernelException("We have found a duplicate in the UUID hash: " + uuid);
		}
	}
	
	SetNullDB();
	
	/* By default, usersr are not quitting. */
	
        SetQuit(false);
	
	/* Any user should be allowed to process queries by default. */
		
	this->SetLock(false);
}

LocalUser::LocalUser(int myfd, engine::sockets::sockaddrs* client, engine::sockets::sockaddrs* servaddr)
	: User(Kernel->UID.GetUID(), Kernel->Clients->Global->server, CLIENT_TYPE_LOCAL)
	, usercon(this)
	, serializer(NULL)
	, lastping(true)
	, exempt(false)
	, next_ping_check(0)
	, touchbase(0)
	, already_sent(0)
{
	connected = Kernel->Now();
	instance = uuid;
	agent = uuid;
	usercon.SetFileDesc(myfd);
	memcpy(&client_sa, client, sizeof(engine::sockets::sockaddrs));
	memcpy(&server_sa, servaddr, sizeof(engine::sockets::sockaddrs));
	SetHost(GetReadableIP(), true);
}

LocalUser::LocalUser(int myfd, const std::string& uid, Serializable::Data& data) : User(uid, Kernel->Clients->Global->server, CLIENT_TYPE_LOCAL), usercon(this), already_sent(0)
{
	usercon.SetFileDesc(myfd);
	Deserialize(data);
}

User::~User()
{
        this->SetLock(false);
        
        /* We remove this user from monitoring list, if applicable. */
        
        Kernel->Monitor->Remove(this);
        Kernel->Notify->Remove(this);
        
	pending.clear();
	notifications.clear();
	
        Kernel->Logins->Sessions->DetermineLifetime(this->login);
}

void User::SetNullDB()
{
        std::lock_guard<std::mutex> lg(User::db_mute);
        this->current_db = nullptr;
}

void User::SetDatabase(const std::shared_ptr<UserDatabase>& database)
{
        std::lock_guard<std::mutex> lg(User::db_mute);
	this->current_db = database;
}

std::shared_ptr<UserDatabase> User::GetDatabase()
{
        std::lock_guard<std::mutex> lg(User::db_mute);
	return this->current_db;
}

void User::SetQuit(bool flag)
{
	this->quitting = flag;
}

bool User::IsQuitting()
{
        return this->quitting;
}

void User::SetLock(bool flag)
{
        this->Locked = flag;
}

bool User::IsLocked()
{
        return this->Locked;
}

const std::string& User::GetHostFormat()
{
	if (!this->cached_user_real_host.empty())
	{
		return this->cached_user_real_host;
	}

	this->cached_user_real_host = instance + "!" + agent + "@" + GetRealHost();
	return this->cached_user_real_host;
}

bool User::CanPerform(unsigned char flag)
{
	/* We cannot check privs if no session is present. */
	
	if (this->session == nullptr)
	{
		return false;
	}
	
	if (session->CanAdmin())
	{
		return true;
	}

        if (flag == 'e' && session->CanExecute())
        {
        	 return true;
        }
        
        if (flag == 'm' && session->CanManage())
        {
        	return true;
        }

	return false;
}

bool User::IsAdmin()
{
        /* We cannot check admin condition if no session is present. */
        
        if (this->session == nullptr)
        {
                return false;
        }

	if (session->CanAdmin())
	{
		return true;
	}

	return false;
}

int User::GetFirstFlag()
{
        /* We cannot check admin condition if no session is present. */

        if (this->session == nullptr)
        {
                return 0;
        }

        if (session->CanAdmin())
        {
                return 3;
        }
        
        if (session->CanExecute())
        {
        	return 2;
        }	
        
        if (session->CanManage())
        {
        	return 1;
        }
        
        return 0;
}

void InstanceStream::StreamData()
{
	if (user->IsQuitting())
	{
		return;
	}

	std::string line;

	std::string::size_type ipos;

	std::string::size_type queueindex;

	while (GetQueueSize() < ULONG_MAX)
	{
		ipos = recvq.find('\n', checked_until);

		if (ipos == std::string::npos)
		{
			checked_until = recvq.length();
			return;
		}

		line.reserve(ipos);

		for (queueindex = 0; queueindex < ipos; ++queueindex)
		{
			char c = recvq[queueindex];

			switch (c)
			{
				case '\0':
					c = ' ';
					break;
				case '\r':
					continue;
			}

			line.push_back(c);
		}

		recvq.erase(0, ipos + 1);
		checked_until = 0;

		Kernel->Commander.ProcessBuffer(user, line);

		if (user->IsQuitting())
		{
			return;
		}

		line.clear();
	}
}

void InstanceStream::swap_internal(InstanceStream& other)
{
	StreamSocket::swap_internal(other);
	std::swap(checked_until, other.checked_until);
}

bool InstanceStream::OnSetEndPoint(const engine::sockets::sockaddrs& server, const engine::sockets::sockaddrs& client)
{
	memcpy(&user->server_sa, &server, sizeof(engine::sockets::sockaddrs));
	user->set_client_ip(client);
	return !user->IsQuitting();
}

void InstanceStream::OnError(LiveSocketError sockerr)
{
	ModuleResult res;
	UNTIL_RESULT(OnConnectionFail, res, (user, sockerr));
	
	if (res != MOD_RES_OK)
	{
		Kernel->Clients->Disconnect(user, get_error());
	}
}

DiscardResult User::discard()
{
	if (!IsQuitting())
	{
		Kernel->Clients->Disconnect(this, "Discarded without Disconnect");
	}
        
	return Expandable::discard();
}

DiscardResult LocalUser::discard()
{
	usercon.discard();
	return User::discard();
}

DiscardResult GlobalUser::discard()
{
	SetQuit(true);
	return User::discard();
}

void LocalUser::check_con_conf(bool clone_count)
{
	connect_config* ac = this->assigned_class;

	if (!ac)
	{
		Kernel->Clients->Disconnect(this, "Access denied by configuration");
		return;
	}
	else if (ac->type == CC_DENY)
	{
		Kernel->Clients->Disconnect(this, ac->config->as_string("reason", "Unauthorised connection", 1));
		return;
	}

	this->next_ping_check = Kernel->Now() + PING_INTVL;
}

void LocalUser::ack_connection()
{
	Kernel->Stats.Connects++;
	this->touchbase = Kernel->Now();

	assigned_class = NULL;
	set_class();
	check_con_conf();

	if (IsQuitting())
	{
		return;
	}

	NOTIFY_MODS(OnInstanceConnect, (this));

	if (Kernel->Clients->not_registered_track)
	{
		Kernel->Clients->not_registered_track--;
	}
	
	this->registered = REG_OK;

	NOTIFY_MODS(OnPostConnect, (this));
}

void User::ResetCache()
{
	user_ip.clear();
	cached_hostip.clear();
	cached_user_real_host.clear();
}

bool User::SetLogin(const std::string& userlogin, time_t newts)
{
	if (this->logged)
	{       
		this->SendProtocol(ERR_ALREADY_LOGGED, ALREADY_LOGGED);
		return false;
	}

	if (IsQuitting())
	{
		return false;
	}
	
	this->logged = Kernel->Now();
	this->login = userlogin;
	
	if (!UserHelper::CheckPass(userlogin, this->auth))
	{
		this->SendProtocol(ERR_WRONG_PASS);
		Kernel->Clients->Disconnect(this, "Wrong password.");
		return false;  	
	}
	
	std::string newlogin = "I-" + this->login + "-" + this->uuid;
	User* const InUse = Kernel->Clients->FindInstanceOnly(userlogin);
	
	if (InUse == this)
	{
		if (newlogin == instance)
		{
			return true;
		}
	}
	else
	{
		if (InUse)
		{	
			Kernel->Clients->Disconnect(InUse, "Internal error.");
			return false;
		}

		age = newts ? newts : Kernel->Now();
	}

	const std::string oldlogin = instance;
	instance = newlogin;

        falert(NOTIFY_DEBUG, "User connected: %s (%s)", this->login.c_str(), this->GetRealHost().c_str());

	ResetCache();
	Kernel->Clients->clientlist.erase(oldlogin);
	Kernel->Clients->clientlist[newlogin] = this;
	Kernel->Clients->logins.insert(std::make_pair(this->login, this));

	if (registered == REG_USER)
	{
		NOTIFY_MODS(OnUserPostLogin, (this, oldlogin));
	}
	
	return true;
}

const std::string& User::GetReadableIP()
{
	if (user_ip.empty())
	{
		user_ip = client_sa.addr();

		if (user_ip[0] == ':')
		{
			user_ip.insert(user_ip.begin(),1,'0');
		}
	}

	return user_ip;
}

const std::string& User::GetRealHost() const
{
	return realhost;
}

engine::sockets::cidr_mask User::GetCompactIPMask()
{
	unsigned char range = 0;

	switch (client_sa.family())
	{
		case AF_INET6:
			range = Kernel->Config->c_ipv6_range;
			break;
		case AF_INET:
			range = Kernel->Config->c_ipv4_range;
			break;
	}

	return engine::sockets::cidr_mask(client_sa, range);
}

bool User::set_client_ip(const std::string& address)
{
	engine::sockets::sockaddrs sa;

	if (!engine::sockets::aptosa(address, client_sa.port(), sa))
	{
		return false;
	}

	User::set_client_ip(sa);
	return true;
}

void User::set_client_ip(const engine::sockets::sockaddrs& sa)
{
	const std::string oldip(GetReadableIP());
	memcpy(&client_sa, &sa, sizeof(engine::sockets::sockaddrs));
	this->ResetCache();
	
	if (GetRealHost() == oldip)
	{
		SetHost(GetReadableIP(), false);
	}
	
	SetPublicHost(GetReadableIP());
}

bool LocalUser::set_client_ip(const std::string& address)
{
	engine::sockets::sockaddrs sa;

	if (!engine::sockets::aptosa(address, client_sa.port(), sa))
	{
		return false;
	}

	LocalUser::set_client_ip(sa);
	return true;
}

void LocalUser::set_client_ip(const engine::sockets::sockaddrs& sa)
{
	if (sa == client_sa)
	{
		return;
	}

	User::set_client_ip(sa);
	this->assigned_class = NULL;
	this->set_class();
	this->check_con_conf();

	if (!IsQuitting())
	{
		NOTIFY_MODS(OnSetUserIP, (this));
	}
}

void LocalUser::Write(const ProtocolTrigger::SerializedMessage& text)
{
	if (!SocketPool::BoundsCheckFd(&usercon))
	{
		return;
	}

	usercon.AppendBuffer(text);
}

void LocalUser::Send(ProtocolTrigger::Event& protoev)
{
	if (!serializer)
	{
		return;
	}

	if (SendMsgList.empty())
	{
		Send(protoev, SendMsgList);
		SendMsgList.clear();
	}
	else
	{
		ProtocolTrigger::MessageList msglist;
		Send(protoev, msglist);
	}
}

void LocalUser::Send(ProtocolTrigger::Event& protoev, ProtocolTrigger::MessageList& msglist)
{
	protoev.get_messagesForUser(this, msglist);

	for (ProtocolTrigger::MessageList::const_iterator i = msglist.begin(); i != msglist.end(); ++i)
	{
		ProtocolTrigger::Message& curr = **i;
		Write(serializer->SerializeForUser(this, curr));
	}
}

void User::SendProtocol(const Numeric::Numeric& numeric)
{
	LocalUser* const localuser = IS_LOCAL(this);

	if (!localuser)
	{
		return;
	}

	ProtocolTrigger::Messages::Numeric numericmsg(numeric, localuser);
	localuser->Send(Kernel->GetBRLDEvents().numeric, numericmsg);
}


namespace
{
	class WriteSimpleRaw : public User::for_each_neighbor_handler
	{
		ProtocolTrigger::Event& ev;

		void Execute(LocalUser* user) 
		{
			user->Send(ev);
		}

	 public:

		WriteSimpleRaw(ProtocolTrigger::Event& protoev) : ev(protoev)
		{

		}
	};
}

void User::SendCommonRaw(ProtocolTrigger::Event& protoev, bool include_self)
{
	WriteSimpleRaw handler(protoev);
	for_each_neighbor(handler, include_self);
}

void User::for_each_neighbor(for_each_neighbor_handler& handler, bool include_self)
{
	SubscriptionList include_chans(chans.begin(), chans.end());
	std::map<User*, bool> exceptions;
	exceptions[this] = include_self;
	NOTIFY_MODS(OnNeighborBuild, (this, include_chans, exceptions));

	const sent_id newerid = Kernel->Clients->next_sentid();

	for (std::map<User*, bool>::const_iterator i = exceptions.begin(); i != exceptions.end(); ++i)
	{
		LocalUser* curr = IS_LOCAL(i->first);

		if (curr)
		{
			curr->already_sent = newerid;

			if ((i->second) && (!curr->IsQuitting()))
			{
				handler.Execute(curr);
			}
		}
	}

	for (SubscriptionList::const_iterator i = include_chans.begin(); i != include_chans.end(); ++i)
	{
		Channel* chan = (*i)->chan;
		const Channel::SubscriptionMap& subscribedlist = chan->GetInstances();

		for (Channel::SubscriptionMap::const_iterator j = subscribedlist.begin(); j != subscribedlist.end(); ++j)
		{
			LocalUser* curr = IS_LOCAL(j->first);

			if ((curr) && (curr->already_sent != newerid))
			{
		                if (!curr->IsAdmin())
		                {	
                                     continue;
				}
                                
				curr->already_sent = newerid;
				handler.Execute(curr);
			}
		}
	}
}

void User::SendRemoteProtocol(const Numeric::Numeric& numeric)
{
	SendProtocol(numeric);
}

bool User::CommonSubscription(User *other)
{
	for (User::SubsList::iterator i = this->chans.begin(); i != this->chans.end(); ++i)
	{
		if ((*i)->chan->IsSubscribed(other))
		{
			return true;
		}
	}
	
	return false;
}

bool User::SetPublicHost(const std::string& shost)
{
	LocalUser* luser = IS_LOCAL(this);

	if (luser)
	{
		ModuleResult MOD_RESULT;
		UNTIL_RESULT(OnPreChangeHost, MOD_RESULT, (luser, shost));

		if (MOD_RESULT == MOD_RES_STOP)
		{
			return false;
		}
	}

	NOTIFY_MODS(OnHostSet, (this, shost));

	this->ResetCache();
	return true;
}

void User::SetHost(const std::string& host, bool resetdisplay)
{
	const bool changehost = (realhost != host);

	if (!changehost && !resetdisplay)
	{
		return;
	}
	
	if (!changehost)
	{
		return;
	}
	
	if (!realhost.empty())
	{
		NOTIFY_MODS(OnSetHost, (this, host));
	}

	realhost = host;
	this->ResetCache();
}

bool User::SetAgent(const std::string& newagent)
{
	if (this->agent == newagent)
	{
		return true;
	}

	NOTIFY_MODS(OnSetAgent, (this, newagent));

	this->agent.assign(newagent, 0, 200);
	this->ResetCache();
	return true;
}

void LocalUser::set_class(const std::string &classname)
{
	connect_config *found = NULL;

	if (!classname.empty())
	{
		for (Configuration::class_refvec::const_iterator i = Kernel->Config->Classes.begin(); i != Kernel->Config->Classes.end(); ++i)
		{
			connect_config* c = *i;

			if (classname == c->name)
			{
				found = c;
			}
		}
	}
	else
	{
		for (Configuration::class_refvec::const_iterator i = Kernel->Config->Classes.begin(); i != Kernel->Config->Classes.end(); ++i)
		{
			connect_config* c = *i;

			ModuleResult MOD_RESULT;
			UNTIL_RESULT(OnSetConnectConfig, MOD_RESULT, (this,c));
		
			if (MOD_RESULT == MOD_RES_STOP)
			{
				continue;
			}

			if (MOD_RESULT == MOD_RES_OK)
			{

				found = c;
				break;
			}

			if (c->type == CC_NAMED)
			{

				continue;
			}

			bool regdone = (registered != REG_NONE);

			if (c->config->as_bool("registered", regdone) != regdone)
			{
				continue;
			}

			
			if (!Daemon::MatchCompactIP(this->GetReadableIP(), c->GetHost(), NULL))
			{
				continue;
			}

			
			if (!c->ports.empty() && !c->ports.count(this->server_sa.port()))
			{

				continue;
			}

			found = c;
			break;
		}
	}

	if (found)
	{
		assigned_class = found;
	}
}

void User::CheckEmptyChannels()
{
	for (User::SubsList::iterator i = this->chans.begin(); i != this->chans.end(); )
	{
		Channel* c = (*i)->chan;
		++i;
		c->DeleteUser(this);
	}
}

const std::string& GlobalUser::GetHostFormat()
{
	return server->GetName();
}

connect_config::connect_config(config_rule* tag, char t, const std::string& mask)
			: config(tag)
			, type(t)
			, name("undefined")
			, host(mask)
{

}

connect_config::connect_config(config_rule* tag, char t, const std::string& mask, const connect_config& parent)
{
	Update(&parent);
	name = "undefined";
	type = t;
	host = mask;

	file_config_items* items = NULL;
	config = config_rule::create(tag->tag, tag->src_name, tag->src_line, items);

	const file_config_items& parentkeys = parent.config->getItems();
	
	for (file_config_items::const_iterator piter = parentkeys.begin(); piter != parentkeys.end(); ++piter)
	{
		if (stdhelpers::string::equalsci(piter->first, "name") || stdhelpers::string::equalsci(piter->first, "parent"))
		{
			continue;
		}
		
		(*items)[piter->first] = piter->second;
	}

	const file_config_items& childkeys = tag->getItems();

	for (file_config_items::const_iterator citer = childkeys.begin(); citer != childkeys.end(); ++citer)
	{
		(*items)[citer->first] = citer->second;
	}
}

void connect_config::Update(const connect_config* src)
{
		config = src->config;
		type = src->type;
		name = src->name;
		host = src->host;
		ports = src->ports;

}
