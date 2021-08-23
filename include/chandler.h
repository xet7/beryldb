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

/* Used to indicate the result of a given command. */

enum COMMAND_RESULT
{
	/* Command failed. */

	FAILED = 0,

	/* Command OK */
	
	SUCCESS = 1,
	
	/* Invalid result */
	
	INVALID = 2
};

/* Commands that can only be ran by a server */

const char SERVER_LIMITED = 7; 

enum InterpretationType
{
	TR_TEXT,		
	TR_INSTANCE,		
	TR_CUSTOM		
};


enum RouteType
{
	ROUTE_TYPE_LOCALONLY,
	ROUTE_TYPE_BROADCAST,
	ROUTE_TYPE_UNICAST,
	ROUTE_TYPE_MESSAGE,
	ROUTE_TYPE_OPT_BCAST,
	ROUTE_TYPE_OPT_UCAST
};

/* Routing information for a given command */

struct RouteParams
{
	/* Route type. */
	
	RouteType type;
	
	/* Destination server */
	
	std::string serverdest;
	
	/* Destination server */
	
	Server* server;
	
	/* Creates a route param. */
	
	RouteParams(RouteType t, const std::string &d) : type(t), serverdest(d), server(NULL) 
	{ 
	
	}

	RouteParams(RouteType t, Server* srv) : type(t), server(srv) 
	{ 
	
	}
};


/* Avoid routing this command. */

#define ROUTE_LOCALONLY (RouteParams(ROUTE_TYPE_LOCALONLY, ""))

/* Route to all cluster */

#define ROUTE_BROADCAST (RouteParams(ROUTE_TYPE_BROADCAST, ""))

/* Routes a command to a given server */

#define ROUTE_UNICAST(x) (RouteParams(ROUTE_TYPE_UNICAST, x))

#define ROUTE_MESSAGE(x) (RouteParams(ROUTE_TYPE_MESSAGE, x))

#define ROUTE_OPT_BCAST (RouteParams(ROUTE_TYPE_OPT_BCAST, ""))

#define ROUTE_OPT_UCAST(x) (RouteParams(ROUTE_TYPE_OPT_UCAST, x))

/*
 * Defines a command. All commands must be defined
 * through this class 
 */
 
class ExportAPI CommandModel : public ServiceProvider
{
 public:

	class Params : public std::vector<std::string>
	{
	 private:

		ProtocolTrigger::TagMap tags;

	 public:

		Params(const std::vector<std::string>& paramsref, const ProtocolTrigger::TagMap& tagsref)
			: std::vector<std::string>(paramsref)
			, tags(tagsref)
		{
		
		}

		
		template<typename Iterator> Params(Iterator first, Iterator last) : std::vector<std::string>(first, last)
		{
		
		}
		
		Params() 
		{ 
		
		}
		
		const ProtocolTrigger::TagMap& GetTags() const 
		{ 
			return tags; 
		}
		
		ProtocolTrigger::TagMap& GetTags() 
		{ 
			return tags; 
		}
	};

	/* Minimun number of parameters that this command needs. */
	
	const unsigned int min_params;

	/* Maximun number of params. */
	
	const unsigned int max_params;

	/* 
	 * True if a given command allows the empty parameter to be empty.
	 * This bool is true by default.
	 */
	 
	bool last_empty_ok;

	std::vector<InterpretationType> interpret;

	CommandModel(Module* me, const std::string& cmd, unsigned int minpara = 0, unsigned int maxpara = 0);

	virtual RouteParams GetRouting(User* user, const CommandModel::Params& parameters);
	
	virtual void EncodeParameter(std::string& parameter, unsigned int index);

	virtual ~CommandModel();
};

class ExportAPI Command : public CommandModel
{
  protected:
	
	Command(Module* me, const std::string& cmd, unsigned int minpara = 0, unsigned int maxpara = 0);

  public:
	
	/* Destructor. */
	
	~Command();
	
	std::string alias;
	
	/* Checks whether a given login exists. */
	
	bool check_exists;
	
	/* Check whether login root is allowed to be used */
	
	bool check_root;
	
	/* Determines whether GetLimtis should be ran. */
	
	bool run_conf;

        /* Whether to check a comilla value in a command */

        bool check_value;
	
	/* Offset  (only available when running run_conf */
	
	signed int offset;
	
	/* Limit */
	
	signed int limit;
	
	/* Checks for a valid login */
	
	signed check_login;
	
	/* Checks for a second key, as provided */
	
	signed int check_hash;
	
	/* Checks for a key in provided param */
	
	signed int check_key;
	
	/* Check numeric parameter */
	
	signed int check_num;
	
	/* User modes required to run this command. */
	
	unsigned char flags;
	
	/* Group this command is gropued with */
	
	unsigned char group;
	
	std::vector<unsigned char> groups;
	
	bool force_manual_route;

	/* This command's correct syntax. */

	std::string syntax;

	/* 
	 * This bool should be true if you need a command to be available
	 * before registering.
	 */
	 
	bool pre_reg_ok;
	
	/* This command will not hint after registration. */
	
	bool no_hint_until_reg;

	
	virtual COMMAND_RESULT Handle(User* user, const Params& parameters) = 0;

	
	void RegisterService();
	
	/*
	 * Notifies an user that they are missing some parameters.
	 */
	
	virtual void MissingParameters(LocalUser* user, const Params& parameters);

	/*
	 * Notifies an user that they must be logged in before executing
	 * command.
	 */
	
	virtual void NotLogged(LocalUser* user, const Params& parameters);
};

class ExportAPI MultiCommand : public Command
{
 protected:
	
	MultiCommand(Module* me, const std::string& cmd, unsigned int minpara = 0, unsigned int maxpara = 0);

 public:
	
	COMMAND_RESULT Handle(User* user, const Params& parameters);

	virtual COMMAND_RESULT HandleLocal(LocalUser* user, const Params& parameters);
	
	virtual COMMAND_RESULT HandleRemote(RemoteUser* user, const Params& parameters);
	
	virtual COMMAND_RESULT HandleServer(GlobalUser* user, const Params& parameters);
};

#define INTERPRET1(x1)	interpret.push_back(x1);
#define INTERPRET2(x1,x2)  interpret.push_back(x1);interpret.push_back(x2);
#define INTERPRET3(x1,x2,x3)  interpret.push_back(x1);interpret.push_back(x2);interpret.push_back(x3);
#define INTERPRET4(x1,x2,x3,x4)  interpret.push_back(x1);interpret.push_back(x2);interpret.push_back(x3);interpret.push_back(x4);
#define INTERPRET5(x1,x2,x3,x4,x5)  interpret.push_back(x1);interpret.push_back(x2);interpret.push_back(x3);interpret.push_back(x4);\
	interpret.push_back(x5);
#define INTERPRET6(x1,x2,x3,x4,x5,x6)  interpret.push_back(x1);interpret.push_back(x2);interpret.push_back(x3);interpret.push_back(x4);\
	interpret.push_back(x5);interpret.push_back(x6);
#define INTERPRET7(x1,x2,x3,x4,x5,x6,x7)  interpret.push_back(x1);interpret.push_back(x2);interpret.push_back(x3);interpret.push_back(x4);\
	interpret.push_back(x5);interpret.push_back(x6);interpret.push_back(x7);
#define INTERPRET8(x1,x2,x3,x4,x5,x6,x7,x8)  interpret.push_back(x1);interpret.push_back(x2);interpret.push_back(x3);interpret.push_back(x4);\
	interpret.push_back(x5);interpret.push_back(x6);interpret.push_back(x7);interpret.push_back(x8);
