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
#include "brldb/dbmanager.h"
#include "brldb/database.h"
#include "brldb/dbnumeric.h"
#include "brldb/query.h"
#include "managers/keys.h"
#include "helpers.h"
#include "extras.h"

void KeyHelper::Set(User* user, const std::string& entry, const std::string& value)
{
       std::shared_ptr<set_query> query = std::make_shared<set_query>();
       Helpers::make_query(user, query, entry);
       query->value = stripe(value);
       Kernel->Store->Push(query);
}

void KeyHelper::SetNX(User* user, const std::string& entry, const std::string& value)
{
       std::shared_ptr<setnx_query> query = std::make_shared<setnx_query>();
       Helpers::make_query(user, query, entry);
       query->value = stripe(value);
       Kernel->Store->Push(query);
}

void KeyHelper::SetTX(User* user, const std::string& entry, const std::string& value)
{
       std::shared_ptr<settx_query> query = std::make_shared<settx_query>();
       Helpers::make_query(user, query, entry);
       query->value = stripe(value);
       Kernel->Store->Push(query);
}

void KeyHelper::GetSet(User* user, const std::string& entry, const std::string& value)
{
       std::shared_ptr<getset_query> query = std::make_shared<getset_query>();
       Helpers::make_query(user, query, entry);
       query->value = stripe(value);
       Kernel->Store->Push(query);
}

void KeyHelper::Get(User* user, const std::string& key)
{
       std::shared_ptr<get_query> query = std::make_shared<get_query>();
       Helpers::make_query(user, query, key);
       Kernel->Store->Push(query);
}

void KeyHelper::GetDel(User* user, const std::string& key)
{
       std::shared_ptr<getdel_query> query = std::make_shared<getdel_query>();
       Helpers::make_query(user, query, key);
       Kernel->Store->Push(query);
}

void KeyHelper::GetOccurs(User* user, const std::string& key, const std::string& value)
{
       std::shared_ptr<get_occurs_query> query = std::make_shared<get_occurs_query>();
       Helpers::make_query(user, query, key);
       query->value = stripe(value);
       Kernel->Store->Push(query);
}

void KeyHelper::Strlen(User* user, const std::string& key)
{
       std::shared_ptr<strlen_query> query = std::make_shared<strlen_query>();
       Helpers::make_query(user, query, key);
       Kernel->Store->Push(query);
}

void KeyHelper::Count(User* user, const std::string& key)
{
       std::shared_ptr<count_query> query = std::make_shared<count_query>();
       Helpers::make_query(user, query, key, true);
       query->key = key;
       Kernel->Store->Push(query);
}

void KeyHelper::Exists(User* user, const std::string& key)
{
       std::shared_ptr<exists_query> query = std::make_shared<exists_query>();
       Helpers::make_query(user, query, key);
       Kernel->Store->Push(query);
}

void KeyHelper::WDelete(User* user, const std::string& key)
{
       std::shared_ptr<wdel_query> query = std::make_shared<wdel_query>();
       Helpers::make_query(user, query, key, true);
       
       query->key = stripe(key);
       Kernel->Store->Push(query);
}

void KeyHelper::Keys(User* user, const std::string& key, signed int offset, signed int limit)
{
       std::shared_ptr<keys_query> query = std::make_shared<keys_query>();
       Helpers::make_query(user, query, stripe(key), true);
       query->limit = limit;
       query->offset = offset;
       Kernel->Store->Push(query);
}

void KeyHelper::Search(User* user, const std::string& key, signed int offset, signed int limit)
{
       std::shared_ptr<search_query> query = std::make_shared<search_query>();
       Helpers::make_query(user, query, stripe(key), true);

       query->limit = limit;
       query->offset = offset;
       query->key = stripe(key);
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

void KeyHelper::GetSubstr(User* user, const std::string& key, signed int offset, signed int limit)
{
       std::shared_ptr<get_substr_query> query = std::make_shared<get_substr_query>();
       Helpers::make_query(user, query, key);
       query->limit = limit;
       query->offset = offset;
       Kernel->Store->Push(query);
}

void KeyHelper::Append(User* user, const std::string& key, const std::string& value)
{
       std::shared_ptr<append_query> query = std::make_shared<append_query>();
       Helpers::make_query(user, query, key);
       query->value = stripe(value);
       Kernel->Store->Push(query);
}

void KeyHelper::Random(User* user)
{
       std::shared_ptr<random_query> query = std::make_shared<random_query>();
       Helpers::make_query(user, query);
       Kernel->Store->Push(query);
}

void KeyHelper::GetExp(User* user, const std::string& entry, const std::string& seconds)
{
       std::shared_ptr<getexp_query> query = std::make_shared<getexp_query>();
       Helpers::make_query(user, query, entry);
       query->id = Kernel->Now() + convto_num<unsigned int>(seconds);      
       Kernel->Store->Push(query);
}

void KeyHelper::Alpha(User* user, const std::string& entry)
{
       std::shared_ptr<alpha_query> query = std::make_shared<alpha_query>();
       Helpers::make_query(user, query, entry);
       Kernel->Store->Push(query);
}

void KeyHelper::IsNum(User* user, const std::string& entry)
{
       std::shared_ptr<isnum_query> query = std::make_shared<isnum_query>();
       Helpers::make_query(user, query, entry);
       Kernel->Store->Push(query);
}

void KeyHelper::GetPersist(User* user, const std::string& key)
{
       std::shared_ptr<getpersist_query> query = std::make_shared<getpersist_query>();
       Helpers::make_query(user, query, key);
       Kernel->Store->Push(query);
}

void KeyHelper::IsBool(User* user, const std::string& entry)
{
       std::shared_ptr<isbool_query> query = std::make_shared<isbool_query>();
       Helpers::make_query(user, query, entry);
       Kernel->Store->Push(query);
}

