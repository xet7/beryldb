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

#include "beryl.h"

struct ExportAPI RocksData
{
        bool loaded;
        rocksdb::Status status;
        std::string value;
        
        RocksData(bool lod) : loaded(lod)
        {
        
        }
        
        RocksData() : loaded(false)
        {
        
        }
};

struct ExportAPI MapData
{
        DBL_CODE code;
        
        std::string response;
        unsigned int counter;

        std::map<std::string, std::string> imap;
        std::vector<std::string> list;
        
        /* Constructor */
                
        MapData(DBL_CODE cde) : code(cde)
        {
        
        }
        
        MapData(DBL_CODE cde, unsigned int rsp) : code(cde), counter(rsp)
        {
        
        }
        
        MapData(DBL_CODE cde, const std::string& rsp) : code(cde), response(rsp)
        {
        
        }
        
        MapData(DBL_CODE cde, const std::vector<std::string>& lst) : code(cde), list(lst)
        {
        
        }
 
        MapData(DBL_CODE cde, const std::map<std::string, std::string>& _imap) : code(cde), imap(_imap)
        {

        }
};