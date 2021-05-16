/*
 * BerylDB - A modular database.
 * http://www.beryldb.com
 *
 * Copyright (C) 2021 Carlos F. Ferry <cferry@beryldb.com>
 * 
 * This file is part of BerylDB. BerylDB is free software: you can
 * redistribute it and/or modify it under the terms of the BSD License
 * version 3.
 *
 * More information about our licensing can be found at https://docs.beryl.dev
 */

#pragma once

enum HQUERY_TYPE
{
     HQUERY_REPEAT_COUNT = 0,
};

class Externalize HQuery
{
    public:
        
        HQuery();
        
        HQUERY_TYPE type;
                
        unsigned int offset;
        
        unsigned int limit;
        
        std::string contains;
    
        std::string field;
        
        std::string sort;

        std::string key;
        
        
};
