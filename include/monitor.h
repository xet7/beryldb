/*
 * BerylDB - A modular database.
 * http://www.beryldb.com
 *
 * Copyright (C) 2021 Carlos F. Ferry <cferry@beryldb.com>
 * 
 * This file is part of BerylDB. BerylDB is free software: you can
 * redistribute it and/or modify it under the terms of the BSD License
 * version 3.
 *
 * More information about our licensing can be found at https://docs.beryl.dev
 */

#pragma once

class CMDBuffer;

enum MONITOR_LEVEL
{
      MONITOR_DEFAULT = 5, /* Shows all found commands */
      MONITOR_DEBUG = 10   /* Shows not-found commands plus everything on Default */
};

typedef std::deque<CMDBuffer> BufferQueue;

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
        
        MonitorMap MonitorList;
        
        BufferQueue buffer;
   
   public:
   
        MonitorHandler();
        
        bool Has(User* user);
        
        bool Add(User* user, MONITOR_LEVEL level);
        
        void Remove(User* user);
        
        void Push(const std::string& instance, const std::string& cmd, MONITOR_LEVEL level, const CommandModel::Params& params);
        
        void Flush();
        
        /* Returns true if any monitor is present. */
        
        bool IsActive()
        {
            return this->MonitorList.size();
        }
};