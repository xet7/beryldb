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

enum NOTIFY_LEVEL
{
      NOTIFY_DEBUG = 10,  
      NOTIFY_VERBOSE = 20,
      NOTIFY_DEFAULT = 30, 
      NOTIFY_NONE  = 50
};


typedef std::multimap<User*, NOTIFY_LEVEL> NotifyMap;

class Externalize NotifyStream : public safecast<NotifyStream>
{
    public: 
       
        std::string name;
        
        UserVector users;   
        
        NOTIFY_LEVEL level;

        /* 
         * Adds an user to the notification list.
         * 
         * @parameters:
         *
         *         · User: user to add.
         *         · level: notification level.
         * 
         * @return:
         *
         *         · bool: User added successfuly.
         */

         bool Add(User* user);

         bool Has(User* user);
  
         void Remove(User* user);
         
};

class Externalize Notifier : public safecast<Notifier>
{
   private:
   
         NotifyMap NotifyList;

         std::map<std::string, std::vector<NotifyStream *> > ActiveStreams;
   
   public: 
   
         Notifier();

         void AttachType(const std::string &type);

         void Create(const std::string& name);
      
      void Flush();
      
      
};