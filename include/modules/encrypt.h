/*
 * BerylDB - A modular database.
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

#include "modules.h"

class HashProvider : public DataProvider
{
  public:
      
        const unsigned int out_size;
        const unsigned int block_size;
        
        HashProvider(Module* mod, const std::string& Name, unsigned int osiz = 0, unsigned int bsiz = 0) : DataProvider(mod, "hash/" + Name), out_size(osiz), block_size(bsiz)
        {
        
        }
        
        virtual std::string MakePassword(const std::string& data) = 0;
        
        virtual std::string GetPrintable(const std::string& raw)
        {
                return bin_to_hex(raw);
        }
      
        virtual bool Compare(const std::string& input, const std::string& hash)
        {
               return Daemon::TimingSafeCompare(Generate(input), hash);
        }
        
        std::string Generate(const std::string& data)
        {
                return GetPrintable(MakePassword(data));
        }

        std::string hmac(const std::string& key, const std::string& msg)
        {
                std::string hmac1, hmac2;
                std::string kbuf = key.length() > block_size ? MakePassword(key) : key;
                kbuf.resize(block_size);

                for (size_t n = 0; n < block_size; n++)
                {
                        hmac1.push_back(static_cast<char>(kbuf[n] ^ 0x5C));
                        hmac2.push_back(static_cast<char>(kbuf[n] ^ 0x36));
                }
                
                hmac2.append(msg);
                hmac1.append(MakePassword(hmac2));
                return MakePassword(hmac1);
        }

        bool IsKDF() const
        {
                return (!block_size);
        }

              
};
