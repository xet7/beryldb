
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

#include <thread>

class Externalize Daemon : public safecast<Daemon>
{
    friend class Beryl;

    private:

        void SnapshotStats();

        void RunAs();
        
        static void VoidSignalManager(int);

        static bool ConfigFileExists(std::string& path);

        void DaemonFork();

        void SetCoreLimits();
        
        static void AssignSignals();


    public:
    
    	std::thread::id main_threadid;

        Daemon();  
        
        static void print_newline(unsigned int loops);

        static std::string Welcome(const std::string& msg);

        static std::string Welcome(std::string& msg);
        
        void TellThat(std::string& who, const std::string& msg, int rpl);      

        void DeletePID();


        STR1::function<bool(const std::string&)> ValidChannel;

        static bool ChannelValidator(const std::string& channel);

        static void DefaultGenRandom(char* output, size_t max);
        

        STR1::function<void(char*, size_t)> GenRandom;
        

        STR1::function<bool(const std::string&)> ValidLogin;

        

        static bool LoginValidator(const std::string& login);

        

        STR1::function<bool(const std::string&)> IsAgent;

        

        static bool AgentValidator(const std::string& agent);

        
        static bool ValidHost(const std::string& host);

        
        static bool Valid_SID(const std::string& sid);
        
        
        
        std::string generate_random_str(unsigned int length, bool printable = true);

        
        unsigned long generate_random_int(unsigned long max);


        void CheckRoot();

        std::string get_real_path(const char* path);

        void SavePID(bool exitonfail = true);

        
       
        static std::string Format(const char* TempString, ...) BERYL_PRINTF(1, 2);
        static std::string Format(va_list& vaList, const char* TempString) BERYL_PRINTF(2, 0);

        

        static std::string HumanEpochTime(time_t curtime, const char* format = NULL, bool utc = false);


        
        
        static bool TimingSafeCompare(const std::string& one, const std::string& two);

        

        ProtocolInterface DefaultProtocolInterface;

        
        ProtocolInterface* PI = NULL;


        
        static bool Match(const std::string& str, const std::string& mask, unsigned const char* map = NULL);
        static bool Match(const char* str, const char* mask, unsigned const char* map = NULL);

        
        static bool MatchCompactIP(const std::string& str, const std::string& mask, unsigned const char* map = NULL);
        static bool MatchCompactIP(const char* str, const char* mask, unsigned const char* map = NULL);

        
        static bool MatchMask(const std::string& masks, const std::string& hostname, const std::string& ipaddr);

        
        static unsigned long Duration(const std::string& str);

        
        static bool Duration(const std::string& str, unsigned long& duration);

        
        static bool IsValidDuration(const std::string& str);

        
        static std::string duration_as_string(time_t duration);

        static bool CheckFormat(User* user, const std::string& value, bool notify = true);
        
        static bool CheckRange(User* user, const std::string& value, const std::string& reason, int min, int max);
        
        bool PassCompare(Expandable* ex, const std::string& data, const std::string& input, const std::string& hashtype);

        void printb(const int type, const std::string& buff);

        void printb(const int type, const char *fmt, ...) BERYL_PRINTF(3, 4);
};

class Externalize Dispatcher : public safecast<Dispatcher>
{
   public:
    
     static void SmartDiv(User* user, BRLD_PROTOCOL brld, const std::string& key, const std::string& msg, const std::string& div);

     static void Smart(User* user, int status, BRLD_PROTOCOL brld, const std::string& msg, const std::string& key, DBL_CODE dbl = DBL_NONE, QUERY_TYPE type = TYPE_NONE);
};
