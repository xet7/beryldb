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

class ExportAPI ClientManager : public safecast<ClientManager>
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

        /* Disconnects all users that are using a given login. */

        static unsigned int DisconnectAll(const std::string& login, const std::string& msg);

	/* Adds an user to the clientlist. */
	
	void AddUser(int socket, BindingPort* via, engine::sockets::sockaddrs* client, engine::sockets::sockaddrs* server);

        /* 
         * Disconnects an user from the server.
         * 
         * @parameters:
	 *
	 *         · User: User to disconnect.
	 *         · quitreason: Disconnecting reason.
         */    
         
	void Disconnect(User* user, const std::string& quitreason);

        /* 
         * Disconnects all users that have a common login.
         * 
         * @parameters:
	 *
	 *         · login: Login to disconnect.
	 *         · reason: Reason to use.
         */    
         
	void ExitLogins(const std::string& login, const std::string& reason);

        /* 
         * Joins all users sharing a given login to a given channel.
         * 
         * @parameters:
	 *
	 *         · skip: Skip providing user (User already joined).
	 *         · login: Login to find.
	 *         · chan: Chan to join.
         */    
         
        void Join(User* skip, const std::string& login, const std::string& chan);

        /* 
         * Part all users sharing a given login to a given channel.
         * 
         * @parameters:
         *
         *         · skip: Skip providing user.
         *         · login: Login to find.
         *         · chan: Chan to leave.
         */

        void Part(User* skip, const std::string& login, const std::string& chan);

	/* Counts all logins associated with 'X' login id. 
         *
         * 
         * @parameters:
	 *
	 *         · login: Common login to find.
	 * 
         * @return:
 	 *
         *         · unsigned int: Counter.
         */    
         	
	unsigned int CountLogin(const std::string& login);
	
	/* 
	 * Returns the size of all clients connected (un and registered). 
         *
         * @return:
         *
         *         · unsigned int: Counter.
         */

		
	unsigned int ClientCount() const 
	{ 	
		return this->clientlist.size(); 
	}

        /* 
         * Counts all registered (logged in) users.
         *
         * @return:
 	 *
         *         · unsigned int: Counter.
         */    

	unsigned int RegisteredClientCount() 
	{ 
		return this->clientlist.size() - this->UnregisteredClientCount(); 
	}

        /* 
         * Counts all unregistered (not logged in) users.
         *
         * @return:
         *
         *         · unsigned int: Counter.
         */

	unsigned int UnregisteredClientCount() const 
	{ 
		return this->not_registered_track; 
	}

        /* 
         * Counts all local users.
         *
         * @return:
         *
         *         · unsigned int: Counter.
         */

	unsigned int LocalClientCount() const 
	{ 
		return (this->local_users.size() - this->UnregisteredClientCount()); 
	}

	/*
	 * Returns map containing clientlist.
         * 
         * @return:
 	 *
         *         · UserMap: User uuid keyed with User class pointer.
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

        /* 
         * Returns a user vector containing all users with provided flags.
         * 
         * @parameters:
	 *             · flag: Flag to find.
	 *
         * @return:
         *             · UserVector: All users with 'flag' flags. 
         */    
         	
	static UserVector FindPrivs(const std::string& flag);

	sent_id next_sentid();

	/* Finds an instance. */
	
	User* FindInstance(const std::string &instance);

	User* FindInstanceOnly(const std::string &instance);

	/* 
	 * Finds an user by UID. 
         *
         * @parameters:
	 *
	 *         · Finds an user from its unique id.
	 * 
         * @return:
 	 *
         *         · User: User class. Returns NULL if no user is found.
         */    	
         
	User* FindUUID(const std::string &uid);
	
	/* 
	 * Finds all users that are logged with a given session. 
         * 
         * @parameters:
	 *
	 *         · login: Common login to find.
	 *         · registration_state: Registration state to find, by default 
	 *                               this function looks for logged-in (REG_OK) users.
	 * 
         * @return:
 	 *
         *         · UserVector: A vector containing all common users.
         */    	
		
	UserVector FindLogin(const std::string& login, registration_state state = REG_OK);

        /* 
         * Returns first instance using provided login.
         * 
         * @return:
 	 *
         *         · User: Instance found.
         */    	
	
	User* FirstLogin(const std::string& login, registration_state state = REG_OK);
};
