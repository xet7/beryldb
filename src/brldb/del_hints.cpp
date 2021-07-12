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

#include "beryl.h"
#include "brldb/database.h"
#include "brldb/query.h"
#include "brldb/dbnumeric.h"
#include "brldb/expires.h"
#include "helpers.h"

void del_query::Keys()
{
     if (this->IsExpiring() > 0)
     {
            this->DelExpire();
     }
}

void del_query::Lists()
{

}

void del_query::Multis()
{

}

void del_query::Maps()
{

}

void del_query::Geos()
{

}

void del_query::Vectors()
{

}

void del_query::Run()
{
    this->Delete(this->dest);

    if (this->identified == INT_KEY)
    {
          this->Keys();
    } 
    else if (this->identified == INT_MAP)
    {
          this->Maps();
    }
    else if (this->identified == INT_GEO)
    {
          this->Geos();
    }
    else if (this->identified == INT_MMAP)
    {
          this->Multis();
    }
    else if (this->identified == INT_LIST)
    {
         this->Lists();
    }
    else if (this->identified == INT_VECTOR)
    {    
          this->Vectors();
    }   

}

void del_query::Process()
{
       user->SendProtocol(BRLD_QUERY_OK, PROCESS_OK);
}
