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

#include "event.h"

namespace Names
{
	class EventListener;
}

class Names::EventListener : public Events::ModuleEventListener
{
 public:

	EventListener(Module* mod) : ModuleEventListener(mod, "event/slist")
	{
	
	}

	virtual ModuleResult OnInstanceList(LocalUser* issuer, Subscription* memb, std::string& prefixes, std::string& instance) = 0;

};
