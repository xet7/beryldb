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

#include "protocols.h"

namespace Numeric
{
	class Numeric;
}

class Numeric::Numeric
{
  private:
  
	unsigned int numeric;
	
	CommandModel::Params params;
	
	Server* sourceserver;

  public:
	
	Numeric(unsigned int num) : numeric(num), sourceserver(NULL)
	{

	}
	
	template <typename T>
	Numeric& push(const T& x)
	{
		params.push_back(convto_string(x));
		return *this;
	}

	void SetServer(Server* server) 
	{ 
		sourceserver = server; 
	}
	
	Server* GetServer() const 
	{ 
		return sourceserver; 
	}
	
	unsigned int GetNumeric() const 
	{ 
		return numeric; 
	}
	
	const CommandModel::Params& GetParams() const 
	{ 
		return params; 
	}

	CommandModel::Params& GetParams() 
	{ 
		return params; 
	}
};
