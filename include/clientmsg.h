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

#include "converter.h"

namespace ProtocolTrigger
{
	namespace Messages
	{
		class Numeric;
		class Join;
		struct Part;
		struct Quit;
		struct Login;
		class Publish;
		struct Ping;
		struct Pong;
		struct Error;
	}
}

class ProtocolTrigger::Messages::Numeric : public ProtocolTrigger::Message
{
	char numericstr[4];

	void InitCommand(unsigned int number)
	{
		snprintf(numericstr, sizeof(numericstr), "%03u", number);
		SetCommand(numericstr);
	}

	void InitFromNumeric(const ::Numeric::Numeric& numeric)
	{
		InitCommand(numeric.GetNumeric());
		for (std::vector<std::string>::const_iterator i = numeric.GetParams().begin(); i != numeric.GetParams().end(); ++i)
			PushParamRef(*i);
	}

 public:
	
	Numeric(const ::Numeric::Numeric& num, User* user)
		: ProtocolTrigger::Message(NULL, (num.GetServer() ? num.GetServer() : Kernel->Clients->Global->server)->GetName())
	{
		if (user->registered & REG_LOGIN)
		{
			PushParamRef(user->instance);
		}
		else
		{
			PushParam("*");
		}
		
		InitFromNumeric(num);
	}

	
	Numeric(const ::Numeric::Numeric& num, const std::string& target)
		: ProtocolTrigger::Message(NULL, (num.GetServer() ? num.GetServer() : Kernel->Clients->Global->server)->GetName())
	{
		PushParamRef(target);
		InitFromNumeric(num);
	}

	
	Numeric(unsigned int num)
		: ProtocolTrigger::Message(NULL, Kernel->Config->GetServerName())
	{
		InitCommand(num);
		PushParam("*");
	}
};


class ProtocolTrigger::Messages::Join : public ProtocolTrigger::Message
{
	Subscription* memb;

 public:
	
	Join()
		: ProtocolTrigger::Message(convto_string(BRLD_JOIN).c_str())
		, memb(NULL)
	{
	
	}
	
	Join(Subscription* Memb)
		: ProtocolTrigger::Message(convto_string(BRLD_JOIN).c_str(), Memb->user)
	{
		SetParams(Memb);
	}

	
	Join(Subscription* Memb, const std::string& sourcestrref)
		: ProtocolTrigger::Message(convto_string(BRLD_JOIN).c_str(), sourcestrref, Memb->user)
	{
		SetParams(Memb);
	}

	
	void SetParams(Subscription* Memb)
	{
		memb = Memb;
		PushParamRef(memb->chan->name);
	}

	
	Subscription* GetMember() const 
	{ 
		return memb; 
	}
};

struct ProtocolTrigger::Messages::Part : public ProtocolTrigger::Message
{
	
	Part(Subscription* memb) : ProtocolTrigger::Message(convto_string(BRLD_PART).c_str(), memb->user)
	{
		PushParamRef(memb->chan->name);
	}
};


struct ProtocolTrigger::Messages::Quit : public ProtocolTrigger::Message
{
	Quit(User* source, const std::string& reason) : ProtocolTrigger::Message(convto_string(BRLD_QUIT).c_str(), source)
	{
		if (!reason.empty())
		{
			PushParamRef(reason);
		}
	}
};

struct ProtocolTrigger::Messages::Login : public ProtocolTrigger::Message
{
	Login(User* source, const std::string& newlogin) : ProtocolTrigger::Message("Login", source)
	{
		PushParamRef(newlogin);
	}
};


class ProtocolTrigger::Messages::Publish : public ProtocolTrigger::Message
{
	void PushTargetChan(char status, const Channel* targetchan)
	{
		if (status)
		{
			std::string rawtarget(1, status);
			rawtarget.append(targetchan->name);
			PushParam(rawtarget);
		}
		else
		{
			PushParamRef(targetchan->name);
		}
	}

	void PushTargetUser(const User* targetuser)
	{
		if (targetuser->registered & REG_LOGIN)
			PushParamRef(targetuser->instance);
		else
			PushParam("*");
	}

 public:
	
	enum NO_REPLICATE
	{
		no_replicate
	};
	
	Publish(User* source, const std::string& target, const std::string& text) : ProtocolTrigger::Message(PUBL_DEF, source)
	{
		PushParam(target);
		PushParam(text);
	}

	
	Publish(User* source, const Channel* targetchan, const std::string& text, char status = 0) : ProtocolTrigger::Message(PUBL_DEF, source)
	{
		PushTargetChan(status, targetchan);
		PushParam(text);
	}

	
	Publish(User* source, const User* targetuser, const std::string& text) : ProtocolTrigger::Message(PUBL_DEF, source)
	{
		PushTargetUser(targetuser);
		PushParam(text);
	}

	
	Publish(const std::string& source, const std::string& target, const std::string& text, char status = 0) : ProtocolTrigger::Message(PUBL_DEF, source)
	{
		if (status)
		{
			std::string rawtarget(1, status);
			rawtarget.append(target);
			PushParam(rawtarget);
		}
		else
		{
			PushParam(target);
		}

		PushParam(text);
	}

	
	Publish(const std::string& source, const Channel* targetchan, const std::string& text, char status = 0)
		: ProtocolTrigger::Message(PUBL_DEF, source)
	{
		PushTargetChan(status, targetchan);
		PushParam(text);
	}

	
	Publish(const std::string& source, const User* targetuser, const std::string& text) : ProtocolTrigger::Message(PUBL_DEF, source)
	{
		PushTargetUser(targetuser);
		PushParam(text);
	}

	
	Publish(NO_REPLICATE, User* source, const std::string& target, const std::string& text) : ProtocolTrigger::Message(PUBL_DEF, source)
	{
		PushParam(target);
		PushParamRef(text);
	}

	
	Publish(NO_REPLICATE, User* source, const Channel* targetchan, const std::string& text, char status = 0) : ProtocolTrigger::Message(PUBL_DEF, source)
	{
		PushTargetChan(status, targetchan);
		PushParamRef(text);
	}

	
	Publish(NO_REPLICATE, User* source, const User* targetuser, const std::string& text)
		: ProtocolTrigger::Message(PUBL_DEF, source)
	{
		PushTargetUser(targetuser);
		PushParamRef(text);
	}

	
	Publish(NO_REPLICATE, const std::string& source, const std::string& target, const std::string& text)
		: ProtocolTrigger::Message(PUBL_DEF, source)
	{
		PushParam(target);
		PushParamRef(text);
	}

	
	Publish(NO_REPLICATE, const std::string& source, const Channel* targetchan, const std::string& text, char status = 0)
		: ProtocolTrigger::Message(PUBL_DEF, source)
	{
		PushTargetChan(status, targetchan);
		PushParamRef(text);
	}

	
	Publish(NO_REPLICATE, const std::string& source, const User* targetuser, const std::string& text)
		: ProtocolTrigger::Message(PUBL_DEF, source)
	{
		PushTargetUser(targetuser);
		PushParamRef(text);
	}
};



struct ProtocolTrigger::Messages::Ping : public ProtocolTrigger::Message
{
	
	Ping()
		: ProtocolTrigger::Message(convto_string(BRLD_PING).c_str())
	{
		PushParamRef(Kernel->Config->GetServerName());
	}

	
	Ping(const std::string& cookie)
		: ProtocolTrigger::Message(convto_string(BRLD_PING).c_str())
	{
		PushParamRef(cookie);
	}
};


struct ProtocolTrigger::Messages::Pong : public ProtocolTrigger::Message
{
	
	Pong(const std::string& cookie, const std::string& server = "") : ProtocolTrigger::Message(convto_string(BRLD_PONG).c_str(), Kernel->Config->GetServerName())
	{
		PushParamRef(Kernel->Config->GetServerName());

		if (!server.empty())
		{
			PushParamRef(server);
		}
		
		PushParamRef(cookie);
	}
};


struct ProtocolTrigger::Messages::Error : public ProtocolTrigger::Message
{
	
	Error(const std::string& text) : ProtocolTrigger::Message(convto_string(BRLD_ERROR).c_str())
	{
		PushParam(text);
	}
};
