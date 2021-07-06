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

/* Only static functions. */

class ExportAPI Helpers
{
    public:
    
        /* 
         * Returns a variable as bool, for instance "no" to false.
         * 
         * @parameters:
	 *
	 *         · string key: Key to read.
	 *         · def: Default value, in case string is unable to read. (false).
	 * 
         * @return:
 	 *
         *         · Boolean.
         */        
         
         static bool as_bool(const std::string &key, bool def = false);
       
        /* 
         * Returns a boolean to string.
         * 
         * @parameters:
	 *
	 *         · bool: Boolean to read.
	 * 
         * @return:
 	 *
         *         · String: False or true.
         */           
         
         static std::string to_bool(bool boolean);
      
        /* 
         *  Defines the fundamentals of a query (keys).
         * 
         * @parameters:
	 *
	 *         · user: User requesting a given query.
	 *         · base: Base query.
	 *         · key: Key to add.
	 *         · allow: Allow an invalid key (ie with wildcards)
         */    
   
         static void make_query(User* user, std::shared_ptr<QueryBase> base, const std::string& key = "", bool allow = false);     

         static void make_map(User* user, std::shared_ptr<QueryBase> base, const std::string& key = "", const std::string& hesh = "", bool allow = false);     

         static void make_cmap(std::shared_ptr<QueryBase> base, const std::string& key = "", const std::string& hesh = "");

         static void make_cquery(std::shared_ptr<QueryBase> base, const std::string& key = "");

         static void make_mmap(User* user, std::shared_ptr<QueryBase> base, const std::string& key = "", bool allow = false);

         static void make_list(User* user, std::shared_ptr<QueryBase> base, const std::string& key = "", bool allow = false);

         static void make_geo_query(User* user, std::shared_ptr<QueryBase> base, const std::string& key = "");
       
         static std::string Format(const std::string& fmt);
       
        /* 
         * Converts an string from request base to a human readable string.
         * 
         * @parameters:
	 *
	 *         · type: Request base (1, 2, 3, ..).
	 * 
         * @return:
 	 *
         *         · string: Human readable format (MAP, GEOS, LISTS, ..).
         */    
         
         static std::string TypeString(const std::string& type);
};