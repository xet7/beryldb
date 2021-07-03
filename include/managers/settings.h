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

class ExportAPI STHelper
{
    public:
    
        static bool Set(const std::string& type, const std::string& key, const std::string& value);

        static std::string Get(const std::string& type, const std::string& key);

        static void Delete(const std::string& type, const std::string& key);

        static void DeleteAll(const std::string& type);
    
};

