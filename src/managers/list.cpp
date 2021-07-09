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
#include "extras.h"
#include "brldb/dbmanager.h"
#include "brldb/database.h"
#include "brldb/dbnumeric.h"
#include "brldb/query.h"
#include "managers/lists.h"
#include "helpers.h"

void ListHelper::Push(User* user, const std::string& entry, const std::string& value)
{
       std::shared_ptr<lpush_query> query = std::make_shared<lpush_query>();
       Helpers::make_list(user, query, entry);
       query->value = stripe(value);
       Kernel->Store->Push(query);
}

void ListHelper::Get(User* user, const std::string& entry, signed int offset, signed int limit)
{
       std::shared_ptr<lget_query> query = std::make_shared<lget_query>();
       Helpers::make_list(user, query, entry);
       
       query->offset = offset;
       query->limit = limit;
       
       Kernel->Store->Push(query);
}

void ListHelper::PopFront(User* user, const std::string& entry)
{
       std::shared_ptr<lpop_front_query> query = std::make_shared<lpop_front_query>();
       Helpers::make_list(user, query, entry);
       Kernel->Store->Push(query);
}

void ListHelper::PopBack(User* user, const std::string& entry)
{
       std::shared_ptr<lpop_back_query> query = std::make_shared<lpop_back_query>();
       Helpers::make_list(user, query, entry);
       Kernel->Store->Push(query);
}

void ListHelper::Count(User* user, const std::string& entry)
{
       std::shared_ptr<lcount_query> query = std::make_shared<lcount_query>();
       Helpers::make_list(user, query, entry);
       Kernel->Store->Push(query);
}

void ListHelper::PopAll(User* user, const std::string& entry, const std::string& value)
{
       std::shared_ptr<lpopall_query> query = std::make_shared<lpopall_query>();
       Helpers::make_list(user, query, entry);
       query->value = stripe(value);
       Kernel->Store->Push(query);
}

void ListHelper::Reverse(User* user, const std::string& entry)
{
       std::shared_ptr<lreverse_query> query = std::make_shared<lreverse_query>();
       Helpers::make_list(user, query, entry);
       Kernel->Store->Push(query);
}

void ListHelper::Find(User* user, const std::string& entry, const std::string& value, signed int offset, signed int limit)
{
       std::shared_ptr<lfind_query> query = std::make_shared<lfind_query>();
       Helpers::make_list(user, query, entry);
       query->offset = offset;
       query->limit = limit;
       query->value = stripe(value);
       Kernel->Store->Push(query);
}

void ListHelper::Keys(User* user, const std::string& entry, signed int offset, signed int limit)
{
       std::shared_ptr<lkeys_query> query = std::make_shared<lkeys_query>();
       Helpers::make_list(user, query, entry, true);
       query->offset = offset;
       query->limit = limit;
       Kernel->Store->Push(query);
}

void ListHelper::Resize(User* user, const std::string& entry, const std::string& value)
{
       std::shared_ptr<lresize_query> query = std::make_shared<lresize_query>();
       Helpers::make_list(user, query, entry);
       query->value = value;
       Kernel->Store->Push(query);
}

void ListHelper::Sort(User* user, const std::string& entry)
{
       std::shared_ptr<lsort_query> query = std::make_shared<lsort_query>();
       Helpers::make_list(user, query, entry);
       Kernel->Store->Push(query);
}

void ListHelper::Index(User* user, const std::string& key, const std::string& entry)
{
       std::shared_ptr<lpos_query> query = std::make_shared<lpos_query>();
       Helpers::make_list(user, query, key);
       query->value = entry;
       Kernel->Store->Push(query);
}

void ListHelper::Exist(User* user, const std::string& entry, const std::string& value)
{
       std::shared_ptr<lexist_query> query = std::make_shared<lexist_query>();
       Helpers::make_list(user, query, entry);
       query->value = stripe(value);
       Kernel->Store->Push(query);
}

void ListHelper::Del(User* user, const std::string& entry, const std::string& value)
{
       std::shared_ptr<ldel_query> query = std::make_shared<ldel_query>();
       Helpers::make_list(user, query, entry);
       query->value = stripe(value);
       Kernel->Store->Push(query);
}

void ListHelper::Repeats(User* user, const std::string& entry, const std::string& value)
{
       std::shared_ptr<lrepeats_query> query = std::make_shared<lrepeats_query>();
       Helpers::make_list(user, query, entry);
       query->value = stripe(value);
       Kernel->Store->Push(query);
}

void ListHelper::Rop(User* user, const std::string& entry)
{
       std::shared_ptr<lrop_query> query = std::make_shared<lrop_query>();
       Helpers::make_list(user, query, entry);
       Kernel->Store->Push(query);
}

void ListHelper::FRop(User* user, const std::string& entry)
{
       std::shared_ptr<lrfront_query> query = std::make_shared<lrfront_query>();
       Helpers::make_list(user, query, entry);
       Kernel->Store->Push(query);
}

