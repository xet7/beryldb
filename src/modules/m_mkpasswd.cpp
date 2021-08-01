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
#include "modules/encrypt.h"

class CommandMKPasswd : public Command
{
 public:
	CommandMKPasswd(Module* Creator) : Command(Creator, "MKPASSWD", 2)
	{
		syntax = "<hashtype> <text>";
	}

	COMMAND_RESULT Handle(User* user, const Params& parameters)
	{
		if (!parameters[0].compare(0, 5, "hmac-", 5))
		{
			std::string type(parameters[0], 5);
			HashProvider* provider = Kernel->Modules->DataModule<HashProvider>("hash/" + type);
			
			if (!provider)
			{
				user->SendProtocol(ERR_INPUT, INVALID);
				return FAILED;
			}

			if (provider->IsKDF())
			{
				user->SendProtocol(ERR_INPUT, INVALID);
				return FAILED;
			}

			std::string salt = Kernel->Engine->generate_random_str(provider->out_size, false);
			std::string target = provider->hmac(salt, parameters[1]);
			std::string hashed = bin_to_base64(salt) + "$" + bin_to_base64(target, NULL, 0);

			user->SendProtocol(BRLD_OK, hashed);
			return SUCCESS;
		}

		HashProvider* provider = Kernel->Modules->DataModule<HashProvider>("hash/" + parameters[0]);
		
		if (!provider)
		{
			user->SendProtocol(ERR_INPUT, NOT_FOUND);
			return FAILED;
		}

		std::string hashed_pass = provider->Generate(parameters[1]);
		user->SendProtocol(BRLD_OK, hashed_pass);
		return SUCCESS;
	}
};

class ModulePasswordHash : public Module
{
 private:

	CommandMKPasswd cmd;

 public:

	ModulePasswordHash() : cmd(this)
	{
	
	}

	ModuleResult OnPassCompare(Expandable* ex, const std::string &data, const std::string &input, const std::string &hashtype)
	{
		if (!hashtype.compare(0, 5, "hmac-", 5))
		{
			std::string type(hashtype, 5);
			HashProvider* provider = Kernel->Modules->DataModule<HashProvider>("hash/" + type);

			if (!provider)
			{
				return MOD_RES_SKIP;
			}

			if (provider->IsKDF())
			{
				return MOD_RES_STOP;
			}

			std::string::size_type sep = data.find('$');
			
			if (sep == std::string::npos)
			{
				return MOD_RES_STOP;
			}
			
			std::string salt = base_64_to_bin(data.substr(0, sep));
			std::string target = base_64_to_bin(data.substr(sep + 1));

			if (target == provider->hmac(salt, input))
			{
				return MOD_RES_OK;
			}
			else
			{
				return MOD_RES_STOP;
			}
		}

		HashProvider* provider = Kernel->Modules->DataModule<HashProvider>("hash/" + hashtype);

		if (provider)
		{
			if (provider->Compare(input, data))
			{
				return MOD_RES_OK;
			}
			else
			{
				return MOD_RES_STOP;
			}
		}

		return MOD_RES_SKIP;
	}

	Version GetDescription()
	{
		return Version("Provides MKPASSWD command.", VF_BERYLDB|VF_OPTCOMMON); 
	}
};

MODULE_LOAD(ModulePasswordHash)
