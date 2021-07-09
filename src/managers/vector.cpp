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
#include "managers/vectors.h"
#include "helpers.h"

void VectorHelper::Push(User* user, const std::string& entry, const std::string& value)
{
       std::shared_ptr<vpush_query> query = std::make_shared<vpush_query>();
       Helpers::make_list(user, query, entry);
       query->value = stripe(value);
       Kernel->Store->Push(query);
}

void VectorHelper::Get(User* user, const std::string& entry, signed int offset, signed int limit)
{
       std::shared_ptr<vget_query> query = std::make_shared<vget_query>();
       Helpers::make_list(user, query, entry);
       query->offset = offset;
       query->limit = limit;
       Kernel->Store->Push(query);
}

void VectorHelper::Count(User* user, const std::string& entry)
{
       std::shared_ptr<vcount_query> query = std::make_shared<vcount_query>();
       Helpers::make_list(user, query, entry);
       Kernel->Store->Push(query);
}

void VectorHelper::Index(User* user, const std::string& key, const std::string& entry)
{
       std::shared_ptr<vpos_query> query = std::make_shared<vpos_query>();
       Helpers::make_list(user, query, key);
       query->value = entry;
       Kernel->Store->Push(query);
}

void VectorHelper::Resize(User* user, const std::string& entry, const std::string& value)
{
       std::shared_ptr<vresize_query> query = std::make_shared<vresize_query>();
       Helpers::make_list(user, query, entry);
       query->value = value;
       Kernel->Store->Push(query);
}

void VectorHelper::Keys(User* user, const std::string& entry, signed int offset, signed int limit)
{
       std::shared_ptr<vkeys_query> query = std::make_shared<vkeys_query>();
       Helpers::make_list(user, query, entry, true);
       query->offset = offset;
       query->limit = limit;
       Kernel->Store->Push(query);
}

void VectorHelper::Exist(User* user, const std::string& entry, const std::string& value)
{
       std::shared_ptr<vexist_query> query = std::make_shared<vexist_query>();
       Helpers::make_list(user, query, entry);
       query->value = stripe(value);
       Kernel->Store->Push(query);
}

void VectorHelper::PopFront(User* user, const std::string& entry)
{
       std::shared_ptr<vpop_front_query> query = std::make_shared<vpop_front_query>();
       Helpers::make_list(user, query, entry);
       Kernel->Store->Push(query);
}

void VectorHelper::PopBack(User* user, const std::string& entry)
{
       std::shared_ptr<vpop_back_query> query = std::make_shared<vpop_back_query>();
       Helpers::make_list(user, query, entry);
       Kernel->Store->Push(query);
}

void VectorHelper::Del(User* user, const std::string& entry, const std::string& value)
{
       std::shared_ptr<vdel_query> query = std::make_shared<vdel_query>();
       Helpers::make_list(user, query, entry);
       query->value = stripe(value);
       Kernel->Store->Push(query);
}

void VectorHelper::EraseFrom(User* user, const std::string& entry, const std::string& value)
{
       std::shared_ptr<verase_from_query> query = std::make_shared<verase_from_query>();
       Helpers::make_list(user, query, entry);
       query->value = value;
       Kernel->Store->Push(query);
}

void VectorHelper::Reverse(User* user, const std::string& entry)
{
       std::shared_ptr<vreverse_query> query = std::make_shared<vreverse_query>();
       Helpers::make_list(user, query, entry);
       Kernel->Store->Push(query);
}

void VectorHelper::Repeats(User* user, const std::string& entry, const std::string& value)
{
       std::shared_ptr<vrepeats_query> query = std::make_shared<vrepeats_query>();
       Helpers::make_list(user, query, entry);
       query->value = stripe(value);
       Kernel->Store->Push(query);
}

void VectorHelper::Sort(User* user, const std::string& entry)
{
       std::shared_ptr<vsort_query> query = std::make_shared<vsort_query>();
       Helpers::make_list(user, query, entry);
       Kernel->Store->Push(query);
}

