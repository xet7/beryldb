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

class CommandLoadmodule : public Command
{
 public:
 
        CommandLoadmodule(Module* parent) : Command(parent, "LOADMODULE", 1, 1)
        {
                requires = 'r';
                syntax = "<modulename>";
        }

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

COMMAND_RESULT CommandLoadmodule::Handle(User* user, const Params& parameters)
{
        if (Kernel->Modules->Load(parameters[0]))
        {
                sfalert(user, NOTIFY_DEFAULT, "Module %s loaded.", parameters[0].c_str());
                user->SendProtocol(BRLD_MOD_LOADED, parameters[0], Daemon::Format("Module %s loaded.", parameters[0].c_str()).c_str());
                return SUCCESS;
        }
        else
        {
              user->SendProtocol(ERR_UNLOAD_MOD, parameters[0], Kernel->Modules->LastError());
              return FAILED;
        }
}

class CommandUnloadmodule : public Command
{
 public:

        CommandUnloadmodule(Module* parent) : Command(parent, "UNLOADMODULE", 1)
        {
                requires = 'r';
                syntax = "<modulename>";
        }

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

COMMAND_RESULT CommandUnloadmodule::Handle(User* user, const Params& parameters)
{
       if (Daemon::Match(parameters[0], "core_*", ascii_case_insensitive))
       {
             user->SendProtocol(ERR_UNLOAD_MOD, parameters[0], "Warning: You are unloading a core module.");
       }
       
       Module* InUse = Kernel->Modules->Find(parameters[0]);
       
       if (InUse == creator)
       {
             user->SendProtocol(ERR_UNLOAD_MOD, parameters[0], "You cannot unload this module.");
             return FAILED;
       }
       
       if (InUse && Kernel->Modules->Unload(InUse))
       {
              sfalert(user, NOTIFY_DEFAULT, "Module %s loaded.", parameters[0].c_str());
              user->SendProtocol(BRLD_MOD_UNLOAD, parameters[0], Daemon::Format("Module %s unloaded.", parameters[0].c_str()).c_str());          
       }
       else
       {
              user->SendProtocol(ERR_UNLOAD_MOD, parameters[0], (InUse ? Kernel->Modules->LastError() : "Module not found."));
              return FAILED;
       }
       
       return SUCCESS;
}

class ModuleCoreModule : public Module
{
        CommandLoadmodule cmdloadmod;
        CommandUnloadmodule cmdunloadmod;

 public:

        ModuleCoreModule() : cmdloadmod(this), cmdunloadmod(this)
        {
        
        }

        Version GetDescription() 
        {
                return Version("Provides Load and Unload module commands.", VF_BERYLDB|VF_CORE);
        }

};

MODULE_LOAD(ModuleCoreModule)
