/*
 * BerylDB - A modular database.
 * http://www.beryldb.com
 *
 * Copyright (C) 2021 Carlos F. Ferry <cferry@beryldb.com>
 * 
 * This file is part of BerylDB. BerylDB is free software: you can
 * redistribute it and/or modify it under the terms of the BSD License
 * version 3.
 *
 * More information about our licensing can be found at https://docs.beryl.dev
 */

#pragma once

class Externalize CommandHandler
{
	public:

		typedef STR1::unordered_map<std::string, Command*, engine::insensitive, engine::str_hash_comparison> CommandMap;

	private:

		void run_command(LocalUser* user, std::string& command, CommandModel::Params& parameters);

		CommandMap CommandList;

	public:

		CommandHandler();

		const CommandMap& GetCommands() const 
		{ 
			return CommandList; 
		}

		COMMAND_RESULT CallHandler(const std::string& commandname, const CommandModel::Params& parameters, User* user, Command** cmd = NULL);

		Command* GetBase(const std::string &commandname);

		static bool HasLoop(User* user, Command* handler, const CommandModel::Params& parameters, unsigned int splithere, int extra = -1, bool usemax = true);

		void ProcessBuffer(LocalUser* user, const std::string& buffer);

		bool add_command(Command *cmd);

		void Remove(Command* x);

		static void TranslateSingleParam(InterpretationType to, const std::string& item, std::string& dest, CommandModel* custom_translator = NULL, unsigned int paramnumber = 0);

		static std::string TranslateUIDs(const std::vector<InterpretationType>& to, const CommandModel::Params& source, bool prefix_final = false, CommandModel* custom_translator = NULL);
};
