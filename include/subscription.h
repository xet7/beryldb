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

#include "converter.h"
 
class Externalize Subscription : public Expandable, public brld::node_list_node<Subscription>
{
  public:

	/* 
	 * boolean that indicates whether a given join is a product
	 * of the autojoin list.
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
