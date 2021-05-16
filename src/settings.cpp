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

#include "beryl.h"
#include "settings.h"
#include "helpers.h"
#include "managers/user.h"
#include "managers/settings.h"

#include "helpers.h"
#include "subscription.h"
#include "settings.h"

Settings::Settings() 
{

}

void Settings::SetDefaults()
{
      Kernel->Config->AllowNew = true;
      Kernel->Config->SyntaxHints = true;
      Kernel->Config->AutoJoin = false;
      Kernel->Config->ChanSync = false;
      
      STHelper::Set("conf", "allowchans", "true");
      STHelper::Set("conf", "syntaxhints", "true");
      STHelper::Set("conf", "autojoin", "off");
      STHelper::Set("conf", "chansync", "off");
}

bool Settings::Set(const std::string& key, const std::string& value, bool skip)
{
        if (key == "allowchans")
        {
                 Kernel->Config->AllowNew = Helpers::as_bool(value, true);       
                 
                 if (skip)
                 {
                      STHelper::Set("conf", "allowchans", value);
                 }
                 
                 return true;
        }
        else if (key == "syntaxhints")
        {
                 Kernel->Config->SyntaxHints = Helpers::as_bool(value, true);

                 if (skip)
                 {
                      STHelper::Set("conf", "syntaxhints", value);
                 }

                 return true;
        }
        else if (key == "autojoin")
        {
                 Kernel->Config->AutoJoin = Helpers::as_bool(value, true);

                 if (skip)
                 {
                      STHelper::Set("conf", "autojoin", value);
                 }

                 return true;
        }        
        else if (key == "chansync")
        {
                 Kernel->Config->ChanSync = Helpers::as_bool(value, true);

                 if (skip)
                 {
                      STHelper::Set("conf", "chansync", value);
                 }

                 return true;
        }
        

        return false;
}

std::string Settings::Get(const std::string& key)
{
        if (key == "allowchans")
        {
                 return Helpers::to_bool(Kernel->Config->AllowNew);
        }
        
        if (key == "syntaxhints")
        {
                 return Helpers::to_bool(Kernel->Config->SyntaxHints);
        }
        
        if (key == "autojoin")
        {
                 return Helpers::to_bool(Kernel->Config->AutoJoin);
        }

        if (key == "chansync")
        {
                 return Helpers::to_bool(Kernel->Config->ChanSync);
        }
        
        
        return "";
}

std::string Helpers::to_bool(bool boolean)
{
        if (boolean)
        {
                return "true";
        }
        else
        {
                return "false";
        }
}

bool Helpers::as_bool(const std::string &key, bool def)
{
        std::string result = key;

        if (key.empty())
        {
                return def;
        }

        if (stdhelpers::string::equalsci(result, "yes") || stdhelpers::string::equalsci(result, "true") || stdhelpers::string::equalsci(result, "on") || result == "1")
        {
                return true;
        }

        if (stdhelpers::string::equalsci(result, "no") || stdhelpers::string::equalsci(result, "false") || stdhelpers::string::equalsci(result, "off") || result == "0")
        {
                return false;
        }

        return def;
}

