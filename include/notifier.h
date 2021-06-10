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
      NOTIFY_DEFAULT = 30
};

struct Event
{
    public:

       /* User to skip while showing this event. */
           
       User* skip;
    
       std::string command;

       NOTIFY_LEVEL level;

       Event(User* usr, const std::string& cmd, NOTIFY_LEVEL lvl) : skip(usr), command(cmd), level(lvl)
       {
       
       }
};

typedef std::map<User*, NOTIFY_LEVEL> NotifyMap;

class Externalize Notifier : public safecast<Notifier>
{
   private:
         
         /* Pending events to be flushed */
         
         std::deque<Event> events;
         
         NotifyMap NotifyList;
            
   public: 

         /* Constructor */
         
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

        /* 
         * Checks if provided user is in NotifyList.
         * 
         * @parameters:
	 *
	 *         · user: User to check:
	 * 
         * @return:
 	 *
         *         · True: User is in NotifyList.
         *         · False: User is not.
         */    
         
         bool Has(User* user);

        /* 
         * Removes an user from NotifyList.
         * 
         * @parameters:
	 *
	 *         · user: Uset to remove.
         */    
         
         void Remove(User* user);

        /* 
         * Adds an user to the Skipping event list.
         * 
         * @parameters:
	 *
	 *         · skip: User to skip.
	 *         · level: Level of events this event is from.
	 *         · buff: Message.
         */    
         
         void SPush(User* skip, NOTIFY_LEVEL level, const std::string& buff);

         void SPush(User* skip, NOTIFY_LEVEL level, const char *fmt, ...) BERYL_PRINTF(4, 5);

        /* 
         * Adds an event to the event queue.
         * 
         * @parameters:
         *
         *         · level: Log Level.
         *         · uff: Message.
         */    
         
         void Push(NOTIFY_LEVEL level, const std::string& buff);

         void Push(NOTIFY_LEVEL level, const char *fmt, ...) BERYL_PRINTF(3, 4);

        /* 
         * Flushes all pending events. This function is
         * called inside the mainloop.
         */

         void Flush();

        /* 
         * Resets NotifyList and pending events.
         * 
         * This function should be called before exiting Beryl.
         */    
                   
         void Reset()
         {
                this->events.clear();
                this->NotifyList.clear();
         }
};