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
 
class ExportAPI Subscription : public Expandable, public brld::node_list_node<Subscription>
{
  public:

	/* 
	 * from_connect: boolean that indicates whether this 
	 * join is the result of the autojoin helper.
	 */
	 
	bool from_connect;
	
	typedef uint64_t Id;

	/* User subscribed to a channel. */
	
	User* const user;
	
	/* Channel this subscription is based on. */
	
	Channel* const chan;

	/* Constructor. */
		
	Subscription(User* usr, Channel* cptr) : from_connect(false), user(usr), chan(cptr) 
	{
	
	}
};
