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

#include <mutex>

class ExportAPI ReducerHandler  : public safecast<ReducerHandler>
{
  private:

	std::vector<Discarder*> list;
	std::vector<LocalUser*> SQlist;

  public:

	void Add(Discarder* item) 
	{ 
		this->list.push_back(item); 
	}
	
	void Apply();
};

class ExportAPI PromiseAction : public Discarder
{
 public:

	virtual void Call() = 0;
};


class ExportAPI ActionHandler : public safecast<ActionHandler>
{
 private:
 
	std::vector<PromiseAction*> list;

 public:

        /* 
         * Adds a promise action to be executed during
         * next cycle. 
         *
         * @parameters:
	 *
	 *         · PromiseAction: Inheriting class.
         */    
         
	void AddAction(PromiseAction* item) 
	{ 
		this->list.push_back(item); 
	}
	
	/* Executes all pending promises. */

	void Run();
};
