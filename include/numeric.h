


#pragma once

#include "protocols.h"

namespace Numeric
{
	class Numeric;
}

class Numeric::Numeric
{
	
	unsigned int numeric;

	
	CommandModel::Params params;

	
	Server* sourceserver;

 public:
	
	Numeric(unsigned int num)
		: numeric(num)
		, sourceserver(NULL)
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

	CommandModel::Params& GetParams() { return params; }
};
