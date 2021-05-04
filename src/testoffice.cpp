/*
 * BerylDB - A modular database.
 * http://www.beryldb.com
 *
 * Copyright (C) 2015-2021 Carlos F. Ferry <cferry@beryldb.com>
 * 
 * This file is part of BerylDB. BerylDB is free software: you can
 * redistribute it and/or modify it under the terms of the BSD License
 * version 3.
 *
 * More information about our licensing can be found at https://docs.beryl.dev
 */

#include "beryl.h"

#ifdef ENABLE_TESTOFFICE

#include <iostream>

#include "exit.h"

#include "testoffice.h"
#include "managers/lists.h"
#include "managers/keys.h"
#include "managers/maps.h"
#include "brldb/database.h"
#include "managers/user.h"

TestOffice::TestOffice()
{

}

TestOffice::~TestOffice()
{

}

void TestOffice::Run()
{
        
        /* Let's verify if a given string is in the right format. */
        
        const std::string& test = "\"hello world\"";

        std::cout << "is_correct: " << is_correct(test) << std::endl;
        std::cout << "stripe: " << stripe(test) << std::endl;
        
        /* Simulate an auto join list. */
                
        ListHelper::Add(TABLE_AUTOJOIN, "root", "#test1");
        ListHelper::Add(TABLE_AUTOJOIN, "root", "#test2");
        ListHelper::Add(TABLE_AUTOJOIN, "root", "$test3");
        
        VectorTuple tpl = ListHelper::Get(TABLE_AUTOJOIN, "root");
        std::vector<std::string> lst = std::get<1>(tpl);
        
        for (std::vector<std::string>::iterator i = lst.begin(); i != lst.end(); i++)
        {
               std::string channel = *i;
               
               /* Let's verify if channel is properly formatted. */
               
               if (!Kernel->Engine->ValidChannel(channel))
               {
                       bprint(ERROR, "%s is invalid", channel.c_str());
                       continue;
               }
               
               bprint(DONE, "%s is a valid channel.", channel.c_str());
        
        }
        
        bprint(INFO, "Removing channels.");

        ListHelper::Delete(TABLE_AUTOJOIN, "root", "#test1");
        ListHelper::Delete(TABLE_AUTOJOIN, "root", "#test2");
        ListHelper::Delete(TABLE_AUTOJOIN, "root", "$test3");
}

#endif