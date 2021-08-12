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

#include <cfloat>
#include <climits>
#include <sstream>
#include <string>
#include <vector>
#include <map>

#include "beryl.h"
#include "modules.h"
#include "filehandler.h"
#include "socketstream.h"
#include "socket.h"
#include "settings.h"

class ExportAPI config_rule : public refcountbase
{
  private:
  
	file_config_items items;
 
  public:

	const std::string tag;
	const std::string src_name;
	const int src_line;

	std::string as_string(const std::string& key, const std::string& def, const STR1::function<bool(const std::string&)>& validator);
	
	std::string as_string(const std::string& key, const std::string& def = "", size_t minlen = 0, size_t maxlen = UINT32_MAX);
	
	long as_int(const std::string& key, long def, long min = LONG_MIN, long max = LONG_MAX, bool force = false);
	
	unsigned long as_uint(const std::string& key, unsigned long def, unsigned long min = 0, unsigned long max = ULONG_MAX, bool force = false);
	
	double as_float(const std::string& key, double def, double min = DBL_MIN, double max = DBL_MAX);

	bool as_bool(const std::string& key, bool def = false);
	
	unsigned long get_duration(const std::string& key, unsigned long def, unsigned long min = 0, unsigned long max = ULONG_MAX);

	bool read_string(const std::string& key, std::string& value, bool allow_newline = false);

	std::string get_tag_location();

	inline const file_config_items& getItems() const 
	{ 
		return items; 
	}

	static config_rule* create(const std::string& Tag, const std::string& file, int line, file_config_items*& Items);

 private:

	config_rule(const std::string& Tag, const std::string& file, int line);
};

struct DBOptions
{
	unsigned int increaseparal;
	
        unsigned int datathread;
        
        unsigned int yieldusec;
        
        bool createim;
        
        bool pipeline;
        
};

/* Stores user-cmd line arguments. */

struct UserArguments
{
	bool nofork;

        bool forcedebug;

	bool writelog;

	bool writepid;
	
	bool asroot;
	
	int run_tests;
	
	int argc;
	
	char** argv;
};

class ExportAPI Configuration
{
  private:
  
	void CrossCheckConnectBlocks();

 public:
	
	class ServerPaths : public safecast<ServerPaths>
	{
   	   public:
		
		/* Configuration path */
		
		std::string Config;

		std::string Data;

		/* Log Path */
		
		std::string Log;
		
		/* Database path. */

		std::string DB;

		/* Module path */
		
		std::string Module;

		
		std::string Runtime;

		ServerPaths(config_rule* tag);

		std::string SetWDDB(const std::string& fn) const { return FileSystem::GetRealPath(DB, fn); }

		std::string SetWDConfig(const std::string& fn) const { return FileSystem::GetRealPath(Config, fn); }
		std::string SetWDData(const std::string& fn) const { return FileSystem::GetRealPath(Data, fn); }
		std::string SetWDLog(const std::string& fn) const { return FileSystem::GetRealPath(Log, fn); }
		std::string SetWDModule(const std::string& fn) const { return FileSystem::GetRealPath(Module, fn); }
		std::string SetWDRuntime(const std::string& fn) const { return FileSystem::GetRealPath(Runtime, fn); }
	};

	
	typedef std::vector<reference<ConfigConnect> > class_refvec;


	
	config_rule* GetConf(const std::string& tag);

	
	MultiTag GetTags(const std::string& tag);

	
	config_rule* EmptyTag;

	
	std::stringstream errstr;

	
	bool Ready;

	std::atomic<bool> KeepExpires;
	
	std::atomic<bool> KeepFutures;
	
	bool WildcardIPv6;

	
	ConfigMap config_data;

	
	cached_config Files;

	ServerPaths Paths;

	
	UserArguments usercmd;
	
	DBOptions DB;

	unsigned char c_ipv4_range;

	
	unsigned char c_ipv6_range;

	std::string ServerName;

	std::string Network;
	
	unsigned int MaxClients;

	
        bool RawLog;

	
	std::string PID;

	
	class_refvec Classes;

	
	/* Modified version of BerylDB. */

	std::string ModifiedVersion;

	
	std::string CaseMapping;


	std::string sid;

	/* Constructor */
	
	Configuration();

	/* Destructor */
	
	~Configuration();

	const std::string& GetSID() const 
	{ 
		return sid; 
	}

	/* Returns server name. */

	const std::string& GetServerName() const 
	{
		return this->ServerName;
	}

	/* Loads configuration data. */
	
	void Load();

	/* Sets all variables. */
	
	void SetAll();

	/* Fills data tags with reading vars. */
	
	void Fill();
	
	void CheckDatabases();

	
	static std::string Escape(const std::string& str, bool xml = true);

        bool printbdebug;
	
};

class ExportAPI config_status
{
	public:
		
	   bool const initial;
		
	   User* const srcuser;

	   config_status(User* user = NULL, bool isinitial = false) : initial(isinitial), srcuser(user)
	   {
		
	   }
};
