/*
 * BerylDB - A modular database.
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

#include <unordered_map>

class Externalize Session : public safecast<Session>
{
    public:
        
        /* Constrcutor. */
        
        Session();
        
        /* Associated login. */
        
        std::string login;
        
        /* root flags. */
        
        bool can_admin;

        /* can execute flags. */
        
        bool can_execute;

        /* Basic management flags. */

        bool can_manage;
};

class Externalize SessionManager : public safecast<SessionManager>
{
    public:
        
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
        
        void Join(User* skip, const std::string& login, const std::string& chan);
        void Part(User* skip, const std::string& login, const std::string& chan);
        
        /* Disconnects all users that are using a given login. */
        
        static void DisconnectAll(const std::string& login);
                
        /* 
         * Verifies if there are more users connected using this session,
         * if not, this session is destroyed, otherwise is kept. 
         */
           
        void ShouldDestroy(const std::string& login);

        /* 
         * This function is typically used when a flag change takes place,
         * users should keep an updated flag list on their side.
         */
         
        void NotifyFlags(const std::string& login, const std::string& flags);

};

/* 
 * Caches user's login and password. This class is mostly
 * used when an user is logging in.
 */
 
class Externalize LoginCache : public safecast<LoginCache>
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
        
        /* Adds a user pass key to the cache. */
        
        void AddCache(const std::string& user, const std::string& pass);

        /* Removes user from cache list. */
        
        void RemoveCache(const std::string& user);

        /* 
         * Checks whether a given user is in the cache.
         * 
         * @return:
         *         ·  1: User pass matches.
         *	   · -1: User found, but pass does not match.
         *         ·  0: User not found.  
         */            
         
        signed int InCache(const std::string& user, const std::string& pass);

        bool InCache(const std::string& user);
        
        /* Removes all logins from cache. */
        
        void ResetCache();
        
        /* Counts all items in the cache. */
        
        unsigned int CountCache()
        {
                return this->logins.size();
        }
        
        /* Removes a given user from the cache list. */
        
        void Remove(const std::string& login);
        
};

