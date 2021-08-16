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
#include "core_list.h"

CommandLResize::CommandLResize(Module* Creator) : Command(Creator, "LRESIZE", 2, 2)
{
       check_key	=	0;
       group 		= 	'l';
       syntax 		= 	"<key> <size>";
}

COMMAND_RESULT CommandLResize::Handle(User* user, const Params& parameters)
{  
       const std::string& value 	= 	parameters[1];

       if (!CheckValidPos(user, value))
       {
              return FAILED;
       }
       
       KeyHelper::Simple(user, std::make_shared<lresize_query>(), parameters[0], value);
       return SUCCESS;  
}

CommandLGet::CommandLGet(Module* Creator) : Command(Creator, "LGET", 1, 3)
{
       run_conf		=	true;
       check_key        =       0;
       group 		= 	'l';
       syntax 		= 	"<key>";
}

COMMAND_RESULT CommandLGet::Handle(User* user, const Params& parameters)
{  
       KeyHelper::RetroLimits(user, std::make_shared<lget_query>(), parameters[0], this->offset, this->limit);
       return SUCCESS;  
}

CommandLKeys::CommandLKeys(Module* Creator) : Command(Creator, "LKEYS", 1, 3)
{
       run_conf		=	true;
       group 		= 	'l';
       syntax 		= 	"<%key> <offset> <limit>";
}

COMMAND_RESULT CommandLKeys::Handle(User* user, const Params& parameters)
{  
       KeyHelper::RetroLimits(user, std::make_shared<lkeys_query>(), parameters[0], this->offset, this->limit, true);
       return SUCCESS;  
}

CommandLFind::CommandLFind(Module* Creator) : Command(Creator, "LFIND", 2, 4)
{
       run_conf		=	true;
       check_key        =       0;
       group 		= 	'l';
       syntax 		= 	"<key> \"%value\" <offset> <limit>";
}

COMMAND_RESULT CommandLFind::Handle(User* user, const Params& parameters)
{  
       std::shared_ptr<lfind_query> query = std::make_shared<lfind_query>();
       query->value 			  = stripe(parameters.back());
       
       KeyHelper::RetroLimits(user, std::make_shared<lfind_query>(), parameters[0], this->offset, this->limit);
       return SUCCESS;  
}

CommandLPos::CommandLPos(Module* Creator) : Command(Creator, "LPOS", 2, 2)
{
       check_key        =       0;
       group 		= 	'l';
       syntax 		= 	"<key> <value>";
}

COMMAND_RESULT CommandLPos::Handle(User* user, const Params& parameters)
{  
       const std::string& value 	 = 	parameters.back();

       if (!CheckValidPos(user, value))
       {
              return FAILED;
       }
       
       KeyHelper::SimpleRetro(user, std::make_shared<lpos_query>(), parameters[0], value);
       return SUCCESS;  
}

CommandLRepeats::CommandLRepeats(Module* Creator) : Command(Creator, "LREPEATS", 2, 2)
{
       check_key        =       0;
       check_value	=	true;
       group 		= 	'l';
       syntax 		= 	"<key> \"value\"";
}

COMMAND_RESULT CommandLRepeats::Handle(User* user, const Params& parameters)
{  
       KeyHelper::SimpleRetro(user, std::make_shared<lrepeats_query>(), parameters[0], parameters.back());
       return SUCCESS;  
}

CommandLRop::CommandLRop(Module* Creator) : Command(Creator, "LRPOP", 1, 1)
{
       check_key        =       0;
       group 		= 	'l';
       syntax 		= 	"<key>";
}

COMMAND_RESULT CommandLRop::Handle(User* user, const Params& parameters)
{  
       KeyHelper::Retro(user, std::make_shared<lrop_query>(), parameters[0]);
       return SUCCESS;  
}

CommandFRop::CommandFRop(Module* Creator) : Command(Creator, "LFPOP", 1, 1)
{
       check_key        =       0;
       group 		= 	'l';
       syntax 		= 	"<key>";
}

COMMAND_RESULT CommandFRop::Handle(User* user, const Params& parameters)
{  
       KeyHelper::Retro(user, std::make_shared<lrop_query>(), parameters[0]);
       return SUCCESS;  
}

CommandLPush::CommandLPush(Module* Creator) : Command(Creator, "LPUSH", 2, 2)
{
       check_key        =       0;
       check_value	=	true;
       group 		= 	'l';
       syntax 		= 	"<key> \"value\"";
}

COMMAND_RESULT CommandLPush::Handle(User* user, const Params& parameters)
{  
        KeyHelper::Simple(user, std::make_shared<lpush_query>(), parameters[0], parameters.back());
        return SUCCESS;  
}

CommandLExist::CommandLExist(Module* Creator) : Command(Creator, "LEXISTS", 2, 2)
{
         check_key        =       0;
         check_value	  =	  true;
         group 		  =  	  'l';
         syntax 	  = 	  "<key> \"value\"";
}

COMMAND_RESULT CommandLExist::Handle(User* user, const Params& parameters)
{  
       KeyHelper::Simple(user, std::make_shared<lexist_query>(), parameters[0], parameters.back());
       return SUCCESS;  
}

CommandLCount::CommandLCount(Module* Creator) : Command(Creator, "LCOUNT", 1, 1)
{
      check_key =       0;
      group 	= 	'l';
      syntax 	= 	"<key>";
}

COMMAND_RESULT CommandLCount::Handle(User* user, const Params& parameters)
{  
       KeyHelper::Retro(user, std::make_shared<lcount_query>(), parameters[0]);
       return SUCCESS;  
}

CommandLBack::CommandLBack(Module* Creator) : Command(Creator, "LBACK", 1, 1)
{
      check_key = 	0;
      group 	= 	'l';
      syntax 	= 	"<key>";
}

COMMAND_RESULT CommandLBack::Handle(User* user, const Params& parameters)
{  
       KeyHelper::Retro(user, std::make_shared<lback_query>(), parameters[0]);
       return SUCCESS;  
}

CommandLFront::CommandLFront(Module* Creator) : Command(Creator, "LFRONT", 1, 1)
{
       check_key	=	0;
       group 		= 	'l';
       syntax 		= 	"<key>";
}

COMMAND_RESULT CommandLFront::Handle(User* user, const Params& parameters)
{  
       KeyHelper::Retro(user, std::make_shared<lfront_query>(), parameters[0]);
       return SUCCESS;  
}

CommandLPushNX::CommandLPushNX(Module* Creator) : Command(Creator, "LPUSHNX", 2, 2)
{
       check_key	=	0;
       check_value	=	true;
       group 		= 	'v';
       syntax 		= 	"<key> \"value\"";
}

COMMAND_RESULT CommandLPushNX::Handle(User* user, const Params& parameters)
{  
        KeyHelper::Simple(user, std::make_shared<lpushnx_query>(), parameters[0], parameters.back());
        return SUCCESS;  
}

CommandLAvg::CommandLAvg(Module* Creator) : Command(Creator, "LAVG", 1, 1)
{
        check_key	=	0;
        group 		= 	'v';
        syntax 		= 	"<key>";
}

COMMAND_RESULT CommandLAvg::Handle(User* user, const Params& parameters)
{  
       KeyHelper::Retro(user, std::make_shared<lavg_query>(), parameters[0]);
       return SUCCESS;  
}

CommandLHigh::CommandLHigh(Module* Creator) : Command(Creator, "LHIGH", 1, 1)
{
       check_key        =       0;
       group 		= 	'v';
       syntax 		= 	"<key>";
}

COMMAND_RESULT CommandLHigh::Handle(User* user, const Params& parameters)
{  
       KeyHelper::Retro(user, std::make_shared<lhigh_query>(), parameters[0]);
       return SUCCESS;  
}

CommandLLow::CommandLLow(Module* Creator) : Command(Creator, "LLOW", 1, 1)
{
       check_key	=	0;
       group 		= 	'v';
       syntax 		= 	"<key>";
}

COMMAND_RESULT CommandLLow::Handle(User* user, const Params& parameters)
{  
       KeyHelper::Retro(user, std::make_shared<llow_query>(), parameters[0]);
       return SUCCESS;  
}

