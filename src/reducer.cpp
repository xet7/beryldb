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

#include "beryl.h"

void ReducerHandler::Apply()
{
	std::set<base_class*> gone;
	std::vector<base_class*> queue;
	queue.reserve(list.size() + 32);

	for (unsigned int i = 0; i < list.size(); i++)
	{
		base_class* c = list[i];

		if (gone.insert(c).second)
		{
			c->discard();
			queue.push_back(c);
		}
	}

	list.clear();
	
	for (unsigned int i = 0; i < queue.size(); i++)
	{
		base_class* c = queue[i];
		delete c;
	}

	if (!list.empty())
	{
		Apply();
	}
}

void ActionHandler::Run()
{
	for (unsigned int i = 0; i < list.size(); i++)
	{
		list[i]->Call();
	}
	
	list.clear();
}
