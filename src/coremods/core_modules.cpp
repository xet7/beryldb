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
#include "notifier.h"

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
                user->SendProtocol(BRLD_OK, PROCESS_OK);
                return SUCCESS;
        }
        else
        {
              user->SendProtocol(ERR_INPUT2, ERR_UNLOAD_MOD, Kernel->Modules->LastError());
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
             sfalert(user, NOTIFY_DEFAULT, "Warning: Unloading core module %s.", parameters[0].c_str());
       }
       
       Module* InUse = Kernel->Modules->Find(parameters[0]);
       
       if (InUse == creator)
       {
             user->SendProtocol(ERR_INPUT, PROCESS_ERROR);
             return FAILED;
       }
       
       if (InUse && Kernel->Modules->Unload(InUse))
       {
              sfalert(user, NOTIFY_DEFAULT, "Module %s loaded.", parameters[0].c_str());
              user->SendProtocol(BRLD_OK, PROCESS_OK);          
       }
       else
       {
              user->SendProtocol(ERR_INPUT2, ERR_UNLOAD_MOD, (InUse ? Kernel->Modules->LastError() : NOT_FOUND));
              return FAILED;
       }
       
       return SUCCESS;
}

class ModuleCoreModule : public Module
{
  private:
  
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
