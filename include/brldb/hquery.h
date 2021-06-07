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


class Externalize HQuery
{
    public:
        
        signed int offset;
        
        signed int limit;
        
        std::string contains;
    
        std::string select;

        std::string key;
        
        HQuery() : offset(0), limit(-1), select("1"), key("*")
        {
        
        }
};
