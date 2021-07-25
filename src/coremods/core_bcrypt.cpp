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

/// $CompilerFlags: -Ietc_directory("bcrypt")

#include "beryl.h"
#include "modules/encrypt.h"
#include "engine.h"

#include <crypt_blowfish.c>

class BcryptServer : public HashProvider
{
 private:

        std::string MakeSalt()
        {
                char entropy[16];
                
                for (unsigned int i = 0; i < sizeof(entropy); ++i)
                {
                        entropy[i] = Kernel->Engine->generate_random_int(0xFF);
                }

                char salt[32];
                
                if (!_crypt_gensalt_blowfish_rn("$2a$", rounds, entropy, sizeof(entropy), salt, sizeof(salt)))
                {
                        throw ModuleException("Unable to generate salt for bcrypt.");
                }

                return salt;
        }
  
  public:
  
        unsigned int rounds;

        std::string Generate(const std::string& data, const std::string& salt)
        {
                char hash[64];
                _crypt_blowfish_rn(data.c_str(), salt.c_str(), hash, sizeof(hash));
                return hash;
        }

        std::string MakePassword(const std::string& data)
        {
                return Generate(data, MakeSalt());
        }

        bool Compare(const std::string& input, const std::string& hash) 
        {
                return Daemon::TimingSafeCompare(Generate(input, hash), hash);
        }

        std::string GetPrintable(const std::string& raw)
        {
                return raw;
        }

        BcryptServer(Module* parent) : HashProvider(parent, "bcrypt", 60), rounds(7)
        {
        
        }
};

class ModuleBCrypt : public Module
{
  private:
  
        BcryptServer bcrypt;

 public:

        ModuleBCrypt() : bcrypt(this)
        {
        
        }

        Version GetDescription()
        {
                return Version("Provides BCrypt support.", VF_BERYLDB|VF_CORE);
        }
};

MODULE_LOAD(ModuleBCrypt)

