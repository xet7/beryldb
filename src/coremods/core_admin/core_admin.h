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

#include "beryl.h"
#include "engine.h"

/* 
 * Logouts an instance from server.
 *
 * @requires 'e'.
 * 
 * @parameters:
 *
 *         · string   : Instances to disconnect. This command allows
 *		        multiple instances to be disconnected at the same time.
 *
 * @protocol:
 *
 *         · enum     :  OK.
 */ 

class CommandLogout : public Command
{
 private:
 
	std::string lastuuid;
	std::string logout_reason;
	ProtocolTrigger::EventProvider protoev;

 public:

	std::string hideinstance;
	
	CommandLogout(Module* parent);

	COMMAND_RESULT Handle(User* user, const Params& parameters);

	RouteParams GetRouting(User* user, const Params& parameters);

	void EncodeParameter(std::string& param, unsigned int index);
};

/* 
 * Finger lists all connected users to the server. 
 *
 * @requires 'e'.
 * 
 * @protocol:
 *
 *         · vector   :  List of connected users, as indicated by
 *                       instance, ip, login, agent and time logged.
 */ 

class CommandFinger : public Command
{
   public:

	CommandFinger(Module* parent);

	COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Returns a map of all logins.
 *
 * @requires 'e'.
 * 
 * @parameters:
 * 
 *         · string	: Login to disconnect.
 *
 * @protocol:
 *
 *         · vector	: Users found.
 */ 

class CommandLogins : public Command
{
   public:

        CommandLogins(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Shutdowns server. This command calls Kernel->Exit(EXIT_CODE_SHUTDOWN) to exit 
 * the server.
 *
 * @requires 'm'.
 * 
 * @parameters:
 *
 *         · string   : Reason.
 * 
 * @protocol:
 *
 *         · enum     : OK.
 */ 

class CommandShutdown : public Command
{
   public:

        CommandShutdown(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
	
};

/* 
 * Restarts server. Keep in mind that when running this command, the initial
 * PID will be remain the same.
 * 
 * @requires 'm'.
 * 
 * @protocol:
 *
 *         · enum   : OK
 */ 

class CommandRestart : public Command
{
   public:

        CommandRestart(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Finds flags for a given flag.
 *
 * @requires 'e'.
 * 
 * @parameters:
 *
 *         · string   : Flags to look for.
 * 
 * @protocol:
 *
 *         · vector  :  Users contaning given flag.
 */ 

class CommandFindFlags : public Command
{
   public:

        CommandFindFlags(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Pauses an user to process any command in the server. Keep in mind that
 * the 'PONG' command will continue working when paused.
 * 
 * @requires 'm'.
 *
 * @parameters:
 *
 *         · string	: Instance to pause.
 *         · int   	: Seconds to pause this user before automatically resuming.
 * 
 * @protocol:
 *
 *         · int   	: Time this user has not submitted any command.
 */ 

class CommandPause : public Command
{
   public:
    
        CommandPause(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Resumes an user from the 'pause' command.
 * 
 * @requires 'm'.
 *
 * @parameters:
 *
 *         · string	: Instance to pause.
 * 
 * @protocol:
 *
 *         · int   	: Time this user has not submitted any command.
 */ 

class CommandResume : public Command
{
   public:
    
        CommandResume(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Idle obtains the current idle for a given instance.
 * Keep in mind that PONG and invalid commands are not considered 
 * in the idle.
 *
 * @requires 'm'.
 * 
 * @parameters:
 *
 *         · string	: Instance to check idle on.
 * 
 * @protocol:
 *
 *         · int   	: Time this user has not submitted any command.
 */ 

class CommandIdle : public Command
{
   public:
    
        CommandIdle(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

