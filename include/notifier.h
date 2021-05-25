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
       
        std::vector<std::string> triggers;
        
        NOTIFY_LEVEL level;
};

typedef std::vector<std::shared_ptr<NotifyStream>> StreamVector;

typedef std::map<std::string, std::vector<std::shared_ptr<NotifyStream>> > StreamMapVec;

struct Event
{
   public:
    
     std::string trigger;
     std::string data;
     NOTIFY_LEVEL level;
     
     Event()
     {
     
     }
};

class Externalize Notifier : public safecast<Notifier>
{
   private:
   
         NotifyMap NotifyList;

         StreamMapVec all;
         
         std::deque<Event> events;
            
   public: 

         Notifier();
         
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

         bool Add(NOTIFY_LEVEL lvl, User* user);

         bool Has(User* user);

         void Remove(User* user);

         void Push(NOTIFY_LEVEL lvl, const std::string& trigger, const std::string& msg);

         void AddTrigger(const std::string& trigger, std::shared_ptr<NotifyStream> stream);
         
         void Flush();
         
      
};