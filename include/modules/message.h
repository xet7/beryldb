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

class MessageWrapper
{
  private:
  
        std::string prefix;
        std::string suffix;
        bool fixed;

  public:
        
        void Wrap(const std::string& message, std::string& out);
        
        void ConfigReading(const char* prefixname, const char* suffixname, const char* fixedname);
};
