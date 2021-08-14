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

#include "brldb/database.h"

class ExportAPI KeyHelper
{
    public:

        static void HeshLimits(User* user, std::shared_ptr<QueryBase> query, const std::string& entry, const std::string& value, signed int offset, signed int limit, bool allow = false);

        static void SimpleType(User* user, std::shared_ptr<QueryBase> query, const std::string& entry, QUERY_TYPE type);

        static void HeshVal(User* user, std::shared_ptr<QueryBase> query, const std::string& key, const std::string& val1, const std::string& val2);

        static void AddPub(User* user, std::shared_ptr<QueryBase> query, const std::string& chan, const std::string& key, const std::string& val1, const std::string& val2);

        static void HeshRetro(User* user, std::shared_ptr<QueryBase> query, const std::string& entry, const std::string& value);

        /* 
         * Adds a query to a map.
         * 
         * @parameters:
	 *
         *         · query: Query type to create.
         *         · kmap: Maps' key.
         *         · key: The key of this map.
         *         · value: Adding value.
         */            
         
        static void SimpleHesh(User* user, std::shared_ptr<QueryBase> query, const std::string& kmap, const std::string& entry, const std::string& value);

	static void Quick(User* user, std::shared_ptr<QueryBase> query);

        static void RetroLimits(User* user, std::shared_ptr<QueryBase> query, const std::string& entry, signed int offset, signed int limit, bool allow = false);

        /* 
         * Posts a simple job to the thread manager.
         * 
         * @parameters:
	 *
	 *         · query: Query type to create.
	 *         · entry: Key to submit.
	 *         · value: Adding value.
         */    
         
        static void Simple(User* user, std::shared_ptr<QueryBase> query,  const std::string& entry, const std::string& value, bool do_stripe = true);

        static void IDRetro(User* user, std::shared_ptr<QueryBase> query, const std::string& entry, const std::string& seconds);

        static void Retro(User* user, std::shared_ptr<QueryBase> query,  const std::string& entry, bool allow = false);

        static void SimpleRetro(User* user, std::shared_ptr<QueryBase> query,  const std::string& entry, const std::string& value);

        /* 
         * Posts an operation to the job manager.
         * 
         * @parameters:
	 *
	 *         · key: Key in which to run a given operation.
	 *         · OP_TYPE: Type of job to run.
	 *         · Operator: This is an optional argument. An operator may be, for instance
	 *                     a second number to run an operation. The operation is required
	 *                     when running jobs such as INCRBY, where a value is required.
	 * 
         * @return:
 	 *
         *         · 
         */    
        static void Operation(User* user, const std::string& key, OP_TYPE type, const std::string& oper = "");
};


