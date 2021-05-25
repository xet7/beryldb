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

//class CMDBuffer;

enum MONITOR_LEVEL
{
      MONITOR_DEBUG = 10, /* Shows all found commands */
      MONITOR_DEFAULT = 20   /* Shows not-found commands plus everything on Default */
};

typedef std::multimap<User*, MONITOR_LEVEL> MonitorMap;

/* Temporary command to deliver to monitors. */

struct CMDBuffer
{
    public:
    
       std::string instance;
       std::string command;

       MONITOR_LEVEL level;

       CommandModel::Params params;
       
       CMDBuffer(const std::string& dest, const std::string& cmd, MONITOR_LEVEL lvl, const CommandModel::Params& parameters) : instance(dest), command(cmd), level(lvl), params(parameters)
       {
       
       }
};

class Externalize MonitorHandler : public safecast<MonitorHandler>
{
   private:
        
        /* Monitor map: keeps track of all active monitors. */
        
        MonitorMap MonitorList;
        
        /* Pending buffer */
        
        std::deque<CMDBuffer> buffer;
        
   public:
   
        /* Constructor */
        
        MonitorHandler();
        
        /* 
         * Checks whether an user is monitoring.
         * 
         * @parameters:
	 *
	 *         · User: User to check.
	 * 
         * @return:
 	 *
         *         · True: User is monitoring.
         *         · False: User is not monitoring.
         */    
        
        bool Has(User* user);

        /* 
         * Adds an user to the monitor watchlist.
         * 
         * @parameters:
	 *
	 *         · user: User to add.
	 *         · level: May be Default or Debug.
	 * 
         * @return:
 	 *
         *         · True: User added. 
         *         · False: Unable to add user.
         */    
                 
        bool Add(User* user, MONITOR_LEVEL level);

        /* 
         * Removes an user from the monitor map.
         * 
         * @parameters:
	 *
	 *         · User: User to remove.
         */            
         
        void Remove(User* user);
        
        /* 
         * Pushes a new event to be delivered to users. 
         * 
         * @parameters:
	 *
	 *         · instance: Instance that is sending a given command.
	 *         · level: Who to deliver this event to: debug or default subscribers.
	 *         · params: Original parameters of sent command.
         */        
             
        void Push(const std::string& instance, const std::string& cmd, MONITOR_LEVEL level, const CommandModel::Params& params);

        /* 
         * Flushes all pending events. This function is
         * called inside the mainloop.
         */    
                 
        void Flush();

        /* 
         * Returns a MonitorMap containing all monitoring users.
         * 
         * @parameters:
	 *
	 *         · arg: Requesting all debugs or defaults.
	 * 
         * @return:
 	 *
         *         · MonitorMap map (user-level).
         */    
         
        MonitorMap GetList(const std::string& arg = "");
                        
        /* Counts all monitor size. */
        
        unsigned int Count()
        {
               return this->MonitorList.size();
        }
        
        /* Clears all monitoring users */
        
        void Reset()
        {
              this->MonitorList.clear();
              this->buffer.clear();
        }
};