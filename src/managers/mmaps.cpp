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

#include <tuple>

#include "beryl.h"
#include "extras.h"
#include "helpers.h"
#include "managers/mmaps.h"

void MMapsHelper::Set(User* user, const std::string& entry, const std::string& hesh, const std::string& value)
{
       std::shared_ptr<mset_query> query = std::make_shared<mset_query>();
       Helpers::make_map(user, query, entry, hesh);

       query->value = stripe(value);
       Kernel->Store->Push(query);
}

void MMapsHelper::SetNX(User* user, const std::string& entry, const std::string& hesh, const std::string& value)
{
       std::shared_ptr<msetnx_query> query = std::make_shared<msetnx_query>();
       Helpers::make_map(user, query, entry, hesh);

       query->value = stripe(value);
       Kernel->Store->Push(query);
}

void MMapsHelper::Get(User* user, const std::string& entry, signed int offset, signed int limit)
{
       std::shared_ptr<mget_query> query = std::make_shared<mget_query>();
       Helpers::make_mmap(user, query, entry);
       
       query->offset = offset;
       query->limit = limit;
       Kernel->Store->Push(query);
}

void MMapsHelper::Keys(User* user, const std::string& entry, signed int offset, signed int limit)
{
       std::shared_ptr<mkeys_query> query = std::make_shared<mkeys_query>();
       Helpers::make_mmap(user, query, entry, true);
       
       query->offset = offset;
       query->limit = limit;
       Kernel->Store->Push(query);
}

void MMapsHelper::Count(User* user, const std::string& entry)
{
       std::shared_ptr<mget_query> query = std::make_shared<mget_query>();
       Helpers::make_mmap(user, query, entry);
       query->flags = QUERY_FLAGS_COUNT;
       
       Kernel->Store->Push(query);
}

void MMapsHelper::Del(User* user, const std::string& entry, const std::string& hesh)
{
       std::shared_ptr<mdel_query> query = std::make_shared<mdel_query>();
       Helpers::make_mmap(user, query, entry);

       query->value = hesh;
       Kernel->Store->Push(query);
}

void MMapsHelper::Seek(User* user, const std::string& entry, const std::string& hesh, signed int offset, signed int limit)
{
       std::shared_ptr<mseek_query> query = std::make_shared<mseek_query>();
       Helpers::make_mmap(user, query, entry, true);
       query->value = stripe(hesh);
       query->offset = offset;
       query->limit = limit;
       Kernel->Store->Push(query);
}

void MMapsHelper::Repeats(User* user, const std::string& key, const std::string& entry)
{
       std::shared_ptr<mrepeats_query> query = std::make_shared<mrepeats_query>();
       Helpers::make_mmap(user, query, key);

       query->value = entry;
       Kernel->Store->Push(query);
}

void MMapsHelper::Vals(User* user, const std::string& entry, signed int offset, signed int limit)
{
       std::shared_ptr<mvals_query> query = std::make_shared<mvals_query>();
       Helpers::make_map(user, query, entry);
       
       query->key = entry;
       query->offset = offset;
       query->limit = limit;
       
       Kernel->Store->Push(query);
}

void MMapsHelper::GetAll(User* user, const std::string& entry, signed int offset, signed int limit)
{
       std::shared_ptr<mgetall_query> query = std::make_shared<mgetall_query>();
       Helpers::make_map(user, query, entry);

       query->key = entry;
       query->offset = offset;
       query->limit = limit;

       Kernel->Store->Push(query);
}

void MMapsHelper::ITER(User* user, const std::string& key, const std::string& entry, signed int offset, signed int limit)
{
       std::shared_ptr<miter_query> query = std::make_shared<miter_query>();
       Helpers::make_mmap(user, query, key);
       query->offset = offset;
       query->limit = limit;


       query->value = entry;
       Kernel->Store->Push(query);
}
