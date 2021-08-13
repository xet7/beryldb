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
#include "core_hints.h"

CommandTransfer::CommandTransfer(Module* Creator) : Command(Creator, "TRANSFER", 2, 2)
{
        check_key       =       0;
        check_hash	=	1;
        group 		= 	'h';
        syntax 		= 	"<key> <dest database>";
}

COMMAND_RESULT CommandTransfer::Handle(User* user, const Params& parameters)
{  
       const std::string& key 		= 	parameters[0];
       const std::string& newkey 	= 	parameters[1];
       
       const std::shared_ptr<UserDatabase>& database = Kernel->Store->DBM->Find(newkey);
       
       if (!database)
       {
            user->SendProtocol(ERR_INPUT, PROCESS_FALSE);
            return FAILED; 
       }
       
       if (database == user->GetDatabase())
       {
            user->SendProtocol(ERR_INPUT, PROCESS_ERROR);
            return FAILED;        
       }

       KeyHelper::SimpleRetro(user, std::make_shared<transfer_query>(), key, newkey);
       return SUCCESS;
}

CommandRename::CommandRename(Module* Creator) : Command(Creator, "RENAME", 2, 2)
{
        check_key       =       0;
        check_hash	=	1;
        group  		= 	'h';
        syntax 		= 	"<key> <new key>";
}

COMMAND_RESULT CommandRename::Handle(User* user, const Params& parameters)
{  
       const std::string& key 		= 	parameters[0];
       const std::string& newkey 	= 	parameters[1];

       KeyHelper::SimpleRetro(user, std::make_shared<rename_query>(), key, newkey);
       return SUCCESS;
}

CommandRenameNX::CommandRenameNX(Module* Creator) : Command(Creator, "RENAMENX", 2, 2)
{
        check_key       =       0;
        check_hash	=	1;
        group  		= 	'h';
        syntax 		= 	"<key> <new key>";
}

COMMAND_RESULT CommandRenameNX::Handle(User* user, const Params& parameters)
{  
       const std::string& key           =       parameters[0];
       const std::string& newkey        =       parameters[1];

       KeyHelper::SimpleRetro(user, std::make_shared<renamenx_query>(), key, newkey);
       return SUCCESS;
}

CommandDel::CommandDel(Module* Creator) : Command(Creator, "DEL", 1)
{
        check_key       =       0;
        syntax 		= 	"<key>";
}

COMMAND_RESULT CommandDel::Handle(User* user, const Params& parameters)
{  
       const std::string& key = parameters[0];
       
       KeyHelper::Retro(user, std::make_shared<del_query>(), key);
       return SUCCESS;
}

CommandCopy::CommandCopy(Module* Creator) : Command(Creator, "COPY", 2, 2)
{
        check_key       =       0;
        check_hash	=	1;
        group  		= 	'h';
        syntax 		= 	"<key> <dest key>";
}

COMMAND_RESULT CommandCopy::Handle(User* user, const Params& parameters)
{  
       const std::string& key 		= 	parameters[0];
       const std::string& destkey 	= 	parameters[1];

       KeyHelper::SimpleRetro(user, std::make_shared<copy_query>(), key, destkey);
       return SUCCESS;
}

CommandExists::CommandExists(Module* Creator) : Command(Creator, "EXISTS", 1, 1)
{
        check_key       =       0;
        group  		= 	'h';
        syntax 		= 	"<key>";
}

COMMAND_RESULT CommandExists::Handle(User* user, const Params& parameters)
{  
       const std::string& key = parameters[0];

       KeyHelper::Retro(user, std::make_shared<exists_query>(), key);
       return SUCCESS;
}

CommandMove::CommandMove(Module* Creator) : Command(Creator, "MOVE", 2, 2)
{
        check_key       =       0;
        group  		=	'h';
        syntax 		= 	"<key> <select>";
}

COMMAND_RESULT CommandMove::Handle(User* user, const Params& parameters)
{  
       const std::string& key 		= 	parameters[0];
       const std::string& new_select 	= 	parameters[1];

       if (!CheckValidPos(user, new_select))
       {
              return FAILED;
       }

       if (!Daemon::CheckRange(user, new_select, INVALID_RANGE, 1, 100))
       {
               return FAILED;
       }

       KeyHelper::Simple(user, std::make_shared<move_query>(), key, new_select, false);
       return SUCCESS;
}

CommandType::CommandType(Module* Creator) : Command(Creator, "TYPE", 1, 1)
{
        check_key       =       0;
        group 		= 	'h';
}

COMMAND_RESULT CommandType::Handle(User* user, const Params& parameters)
{  
       const std::string& key = parameters[0];
       
       KeyHelper::SimpleType(user, std::make_shared<type_query>(), key, QUERY_TYPE_TYPE);
       return SUCCESS;
}

CommandTouch::CommandTouch(Module* Creator) : Command(Creator, "TOUCH", 1, 1)
{
        check_value     =       true;
        group  		= 	'h';
        syntax 		= 	"[keys]";
}

COMMAND_RESULT CommandTouch::Handle(User* user, const Params& parameters)
{  
       const std::string& keys = parameters.back();

       KeyHelper::Simple(user, std::make_shared<touch_query>(), "", keys, false);
       return SUCCESS;
}

CommandNTouch::CommandNTouch(Module* Creator) : Command(Creator, "NTOUCH", 1, 1)
{
        check_value     =       true;
        syntax 		= 	"[keys]";
        group  		= 	'h';
}

COMMAND_RESULT CommandNTouch::Handle(User* user, const Params& parameters)
{  
       const std::string& keys = parameters.back();

       KeyHelper::Simple(user, std::make_shared<ntouch_query>(), "", keys, false);
       return SUCCESS;
}

CommandClone::CommandClone(Module* Creator) : Command(Creator, "CLONE", 2, 2)
{
        check_key       =       0;
        group  		= 	'h';
        syntax 		= 	"<key> <select>";
}

COMMAND_RESULT CommandClone::Handle(User* user, const Params& parameters)
{  
       const std::string& key 		= 	parameters[0];
       const std::string& value 	= 	parameters[1];

       if (!CheckValidPos(user, value))
       {
              return FAILED;
       }

       if (!Daemon::CheckRange(user, value, INVALID_RANGE, 1, 100))
       {
               return FAILED;
       }

       KeyHelper::SimpleRetro(user, std::make_shared<clone_query>(), key, value);
       return SUCCESS;
}

CommandDiff::CommandDiff(Module* Creator) : Command(Creator, "DIFF", 2, 4)
{
        check_key       =       0;
        group  		= 	'h';
        syntax 		= 	"<key> <select> <offset> <limit>";
}

COMMAND_RESULT CommandDiff::Handle(User* user, const Params& parameters)
{  
       const std::string& key 		= 	parameters[0];
       const std::string& value 	= 	parameters[1];

       const std::vector<signed int>& lms = GetLimits(user, this->max_params, parameters);

       if (lms[0] == 0)
       {
            return FAILED; 
       }

       std::shared_ptr<diff_query> query = std::make_shared<diff_query>();
       query->value = value;

       KeyHelper::RetroLimits(user, query, key, lms[1], lms[2]);
       return SUCCESS;
}
