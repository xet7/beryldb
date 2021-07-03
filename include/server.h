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


class ExportAPI Server : public base_class
{

 protected:

	/* Unique id for this server */

	const std::string id;

	/* Name of this server */

	const std::string name;

 public:

	Server(const std::string& srvid, const std::string& srvname) : id(srvid), name(srvname)
	{

	}
	
	/* Returns server ID */

	const std::string& GetID() const 
	{ 
		return this->id; 
	}
	
	/* Returns server name */
	 
	const std::string& GetName() const 
	{ 
		return this->name; 
	}
};
