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

#include <list>

/* Handles users' connections. */

class Externalize ClientManager : public safecast<ClientManager>
{
 public:
	
	typedef brld::node_list<LocalUser> LocalList;

	/* Removes a client from LoginHash */

	void EraseLogin(User* user);

 private:
	
	LocalList local_users;

	sent_id dispatch_sent_id;

 public:

	/* Users awaiting to be disconnected. */
	
 	UserVector AwaitingExit;
 	
	/* Constructor, initializes basic variables. */

	ClientManager();

	/* Desctrutor, disconnects all users. */

	~ClientManager();
	 
	/* LoginHash multimap, stores login and user pointer. */

	LoginHash logins;

	UserMap clientlist;

	UserMap uuid_list;
	
	void ForceExits();

	/* 
	 * Global user is created so we can talk to users as if 
	 * we were another user. 
	 */

	GlobalUser* Global;

	unsigned int not_registered_track;

	/* Runs main processes for a given user. */
	
	void Flush(time_t current);

	/* Adds an user to the clientlist. */
	
	void AddUser(int socket, BindingPort* via, engine::sockets::sockaddrs* client, engine::sockets::sockaddrs* server);

	void Disconnect(User* user, const std::string& quitreason);

	/* Disconnecs all users from a given login. */
	
	void ExitLogins(const std::string& login, const std::string& reason);

	/* Counts all logins associated with 'X' login id. */
	
	unsigned int CountLogin(const std::string& login);
	
	/* Returns the size of all clients connected (un and registered). */
		
	unsigned int ClientCount() const 
	{ 	
		return this->clientlist.size(); 
	}


	unsigned int RegisteredClientCount() 
	{ 
		return this->clientlist.size() - this->UnregisteredClientCount(); 
	}


	unsigned int UnregisteredClientCount() const 
	{ 
		return this->not_registered_track; 
	}


	unsigned int LocalClientCount() const 
	{ 
		return (this->local_users.size() - this->UnregisteredClientCount()); 
	}

	/*
	 * Returns map containing clientlist.
	 * This map stores data as User and client. 
	 */

	UserMap& GetInstances() 
	{ 
		return this->clientlist; 
	}

	/* Returns hash login. */

	LoginHash& GetLogins() 
	{ 
		return this->logins; 
	}

	const LocalList& GetLocals() const 
	{ 
		return local_users; 
	}
	
	void NotifyAll(BRLD_PROTOCOL protocol, const char* text, ...);

	void PublishAll(const char* text, ...) BERYL_PRINTF(2, 3);

	sent_id next_sentid();

	/* Finds an instance. */
	
	User* FindInstance(const std::string &instance);

	User* FindInstanceOnly(const std::string &instance);

	/* Finds an user by UID. */
	
	User* FindUUID(const std::string &uid);
	
	/* Finds all users that are logged with a given session. */
		
	UserVector FindLogin(const std::string& login, registration_state state = REG_OK);
};
