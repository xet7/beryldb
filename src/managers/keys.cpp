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
#include "helpers.h"
#include "extras.h"
#include "managers/keys.h"

void KeyHelper::HeshVal(User* user, std::shared_ptr<QueryBase> query, const std::string& key, const std::string& val1, const std::string& val2)
{
       Helpers::make_geo_query(user, query, key);

       query->value = val1;
       query->hesh = val2;
       Kernel->Store->Push(query);
}

void KeyHelper::AddPub(User* user, std::shared_ptr<QueryBase> query, const std::string& chan, const std::string& key, const std::string& val1, const std::string& val2)
{
       Helpers::make_geo_query(user, query, key);

       query->newkey = chan;
       query->value = val1;
       query->hesh = val2;
       
       Kernel->Store->Push(query);
}

void KeyHelper::Simple(User* user, std::shared_ptr<QueryBase> query, const std::string& entry, const std::string& value, bool do_stripe)
{
       Helpers::make_query(user, query, entry);
       
       if (do_stripe)
       {
              query->value = stripe(value);
       }
       else
       {
              query->value  = value;
       }
       
       Kernel->Store->Push(query);
}

void KeyHelper::Quick(User* user, std::shared_ptr<QueryBase> query)
{
       Helpers::make_query(user, query);
       Kernel->Store->Push(query);
}

void KeyHelper::SimpleType(User* user, std::shared_ptr<QueryBase> query, const std::string& entry, QUERY_TYPE type)
{
       Helpers::make_query(user, query, entry);
       query->type = type;
       Kernel->Store->Push(query);
}

void KeyHelper::RetroFunc(User* user, std::shared_ptr<QueryBase> query, const std::string& entry, STR_FUNCTION func, bool allow)
{
       Helpers::make_query(user, query, entry, allow);
       query->function = func;
       
       Kernel->Store->Push(query);
}


void KeyHelper::Retro(User* user, std::shared_ptr<QueryBase> query, const std::string& entry, bool allow)
{
       Helpers::make_query(user, query, entry, allow);
       Kernel->Store->Push(query);
}

void KeyHelper::HeshLimits(User* user, std::shared_ptr<QueryBase> query, const std::string& entry, const std::string& value, signed int offset, signed int limit, bool allow)
{
       Helpers::make_query(user, query, entry, allow);
       query->value  = stripe(value);
       query->offset = offset;
       query->limit = limit;
       Kernel->Store->Push(query);
}

void KeyHelper::RetroLimits(User* user, std::shared_ptr<QueryBase> query, const std::string& entry, signed int offset, signed int limit, bool allow)
{
       Helpers::make_query(user, query, entry, allow);
       query->offset = offset;
       query->limit = limit;
       Kernel->Store->Push(query);
}

void KeyHelper::SimpleRetro(User* user, std::shared_ptr<QueryBase> query, const std::string& entry, const std::string& value)
{
       Helpers::make_query(user, query, entry);
       query->value = value;
       Kernel->Store->Push(query);
}

void KeyHelper::SimpleHesh(User* user, std::shared_ptr<QueryBase> query, const std::string& kmap, const std::string& entry, const std::string& value)
{
       Helpers::make_map(user, query, kmap, entry);
       query->value = stripe(value);
       
       Kernel->Store->Push(query);
}

void KeyHelper::HeshRetro(User* user, std::shared_ptr<QueryBase> query, const std::string& entry, const std::string& value)
{
       Helpers::make_map(user, query, entry, value);
       Kernel->Store->Push(query);
}

void KeyHelper::IDRetro(User* user, std::shared_ptr<QueryBase> query, const std::string& entry, const std::string& seconds)
{
       Helpers::make_query(user, query, entry);
       query->id = Kernel->Now() + convto_num<unsigned int>(seconds);
       Kernel->Store->Push(query);
}

void KeyHelper::Operation(User* user, const std::string& key, OP_TYPE type, const std::string& oper)
{
       std::shared_ptr<op_query> query = std::make_shared<op_query>();
       Helpers::make_query(user, query, key);
       query->value = oper;
       query->operation = type;
       Kernel->Store->Push(query);
}
