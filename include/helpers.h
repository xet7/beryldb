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
    
       static bool as_bool(const std::string &key, bool def = false);
       
       static std::string to_bool(bool boolean);
      
       static void make_query(User* user, std::shared_ptr<query_base> base, const std::string& key = "");     

       static void make_map(User* user, std::shared_ptr<query_base> base, const std::string& key = "", const std::string& hesh = "");     
       
       static std::string Format(const std::string& fmt);
       
       static std::string TypeString(const std::string& type);
};