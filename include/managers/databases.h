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

class Externalize DBHelper
{
    public:
    
        /* Creates a new map hash */
        
        static bool Add(const std::string& name, const std::string& path);
        
        /* Finds a database. */
        
        static std::string Find(const std::string& name, const std::string& key);
    
        /* Returns all matches for a given key. */
        
        static std::vector<std::string> GetAll(const std::string& name);

        /* Flushes default database. */
        
        static bool FlushDB(bool notify = false);

        /* Calculate db size */
        
        static void DBSize(User* user, std::shared_ptr<Database> database);
        
        static void SFlush(User* user, std::shared_ptr<Database> database, const std::string& select);
        
    
};