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

void ReducerHandler::Apply()
{
	std::set<Discarder*> gone;
	std::vector<Discarder*> queue;
	queue.reserve(list.size() + 32);

	for (unsigned int i = 0; i < list.size(); i++)
	{
		Discarder* c = list[i];

		if (gone.insert(c).second)
		{
			c->discard();
			queue.push_back(c);
		}
	}

	list.clear();
	
	for (unsigned int i = 0; i < queue.size(); i++)
	{
		Discarder* c = queue[i];
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

