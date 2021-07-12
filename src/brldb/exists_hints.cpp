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

void exists_query::Keys()
{

}

void exists_query::Multis()
{

}

void exists_query::Geos()
{

}

void exists_query::Lists()
{

}

void exists_query::Maps()
{

}

void exists_query::Vectors()
{

}

void exists_query::Run()
{
    if (this->identified.empty() || this->identified == PROCESS_NULL)
    {
          this->response = "0";
    }
    else
    {
         this->response = "1";
    }
    
    this->SetOK();
}

void exists_query::Process()
{
       user->SendProtocol(BRLD_QUERY_OK, this->response.c_str());
}
