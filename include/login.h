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

#include "stats.h"

class ExportAPI Session : public safecast<Session>
{
    friend class SessionManager;
    
    private:

        /* Time at which this session was created */
         
        time_t created;
               
        /* Raw flags */
        
        std::string rawflags;
        
        /* Associated login. */
        
        std::string login;
        
        /* root flags. */
        
        bool can_admin;

        /* can execute flags. */
        
        bool can_execute;

        /* Basic management flags. */

        bool can_manage;
  
  public:
          
        /* Constructor. */

        Session();
        
        static std::shared_ptr<Session> SetFlags(std::shared_ptr<Session> New, const std::string& flags);
        
        void SetRaw(const std::string& flags);
        
        bool CanExecute()
        {
              return this->can_execute;
        }
        
        bool CanAdmin()
        {
             return this->can_admin;
        }
        
        bool CanManage()
        {
            return this->can_manage;
        }
        
        std::string GetFlags()
        {
              return this->rawflags;
        }
        
};

class ExportAPI SessionManager : public safecast<SessionManager>
{
    public:
        
        /* Constructor */
        
        SessionManager();
        
        /* map of login and ptr to Session. */
        
        typedef std::map<std::string, std::shared_ptr<Session>> SessionsMap;
        
        /* List of active sessions. */
        
        SessionsMap sessions;
        
        /* Finds a login based on its login. */
        
        std::shared_ptr<Session> Find(const std::string& login);
        
        /* Creates a session. */
        
        std::shared_ptr<Session> Add(const std::string& login, const std::string& flags);

        void AttachExternal(const std::string& login, const std::string& flags);
        
        void Attach(User* user, const std::string& login, const std::string& flags);
                        
        /* 
         * Verifies if there are logins connected using this session,
         * if not, this session is destroyed, otherwise is kept. 
         * 
         * @parameters:
	 *
	 *         · login: Login to check.
         */    
           
        void DetermineLifetime(const std::string& login);

        /* 
         * This function is typically used when a flag change takes place,
         * users should keep an updated flag list on their side.
         */
         
        void NotifyFlags(const std::string& login, const std::string& flags);
};

/* 
 * This class manages an user's cache by storing login and password. 
 * This class contains several functions related to user sessions, such as
 * cache removal and adding.
 */
 
class ExportAPI LoginCache 
{
    private:
    
        /* logins umap tracks of all users loggin in. */
        
        typedef std::unordered_map<std::string, std::string> LoginMap;
        
        /* Cached logins. */
        
        LoginMap logins;
        
        /* Removes last element from cache. */

        bool RemoveLastCache();

    public: 
    
        /* Session handler. */

        SessionManager Sessions;
        
        /* Constructor, does nothing. */

        LoginCache();
        
        /* 
         * Adds a user pass key to the cache.
         * 
         * @parameters:
	 *
	 *         · user: User to add.
	 *         · password: Pass to add.
         */            
        
        void Add(const std::string& user, const std::string& pass);

        /* 
         * Checks whether a given user is in the cache.
         * 
         * @return:
         *
         *      ·  1: User pass matches.
         *      · -1: User found, but pass does not match.
         *      ·  0: User not found.  
         */            
         
        signed int InCache(const std::string& user, const std::string& pass);

        /* 
         * Checks if an user is in cache.
         * 
         * @return:
         *
         *     · True: User is in cache.
         *     · False: User not present.
         */    
         
        bool InCache(const std::string& user);
        
        /* Removes all logins from cache. */
        
        void Reset();
        
        /* Counts all items in the cache. */
        
        unsigned int Count()
        {
                return this->logins.size();
        }
        
        /* 
         * Removes a given user from the cache list. 
         *
         * @parameters:
	 *
	 *         · login: Login to look for.
         */            
        
        void Remove(const std::string& login);
};

