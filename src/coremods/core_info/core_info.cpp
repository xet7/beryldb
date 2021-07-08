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

#include "beryl.h"
#include "core_info.h"

RouteParams ServerTargetCommand::GetRouting(User* user, const Params& parameters)
{
	if ((!parameters.empty()) && (parameters[0].find('.') != std::string::npos))
	{
		return ROUTE_UNICAST(parameters[0]);
	}
	
	return ROUTE_LOCALONLY;
}

class CoreModInfo : public Module
{
  private:
  
	CommandCommands cmdcommands;
	CommandModules cmdmodules;
	CommandCoreModules cmdcoremodules;
	CommandTime cmdtime;
	CommandVersion cmdversion;
	CommandL cmdl;
	CommandSyntax cmdsyntax;
	CommandMyChans cmdmychans;
	CommandChans cmdchans;
	CommandWhoami cmdwhoami;
	CommandAgent cmdagent;
	CommandEpoch cmdepoch;
	CommandFirstOf cmdfirstof;
	CommandID cmdid;
	CommandTotal cmdtotal;
	
  public:
 
	CoreModInfo() : cmdcommands(this), cmdmodules(this), cmdcoremodules(this), cmdtime(this),
			cmdversion(this), cmdl(this), cmdsyntax(this),
			cmdmychans(this), cmdchans(this), cmdwhoami(this),
			cmdagent(this), cmdepoch(this), cmdfirstof(this),
			cmdid(this), cmdtotal(this)
	{

	}

	void Prioritize() 
	{
		Kernel->Modules.SetPriority(this, I_OnInstanceConnect, PRIORITY_FIRST);
	}

	Version GetDescription() 
	{
		return Version("Provides COMMANDS, MODULES, TIME, VERSION and L.", VF_BERYLDB|VF_CORE);
	}
};

MODULE_LOAD(CoreModInfo)
