/*
 * BerylDB - A modular database.
 * http://www.beryldb.com
 *
 * Copyright (C) 2015-2021 Carlos F. Ferry <cferry@beryldb.com>
 * 
 * This file is part of BerylDB. BerylDB is free software: you can
 * redistribute it and/or modify it under the terms of the BSD License
 * version 3.
 *
 * More information about our licensing can be found at https://docs.beryl.dev
 */

#pragma once

namespace ProtocolTrigger
{
	namespace Events
	{
		struct Join;
	}
}

struct ProtocolTrigger::Events::Join : public ProtocolTrigger::Messages::Join, public ProtocolTrigger::Event
{
	Join() : ProtocolTrigger::Event(Kernel->GetBRLDEvents().join, *this)
	{
		
	}

	Join(Subscription* Memb) : ProtocolTrigger::Messages::Join(Memb) , ProtocolTrigger::Event(Kernel->GetBRLDEvents().join, *this)
	{
	
	}

	Join(Subscription* Memb, const std::string& Sourcestr) : ProtocolTrigger::Messages::Join(Memb, Sourcestr)
		, ProtocolTrigger::Event(Kernel->GetBRLDEvents().join, *this)
	{
	
	}
};
