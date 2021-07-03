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
#include "brldb/dbmanager.h"
#include "brldb/database.h"
#include "brldb/dbnumeric.h"
#include "brldb/query.h"
#include "managers/keys.h"
#include "helpers.h"
#include "extras.h"

void KeyHelper::Copy(User* user, const std::string& entry, const std::string& value)
{
       std::shared_ptr<copy_query> query = std::make_shared<copy_query>();
       Helpers::make_query(user, query, entry);
       query->value = value;
       Kernel->Store->Push(query);
}

void KeyHelper::Rename(User* user, const std::string& entry, const std::string& value)
{
       std::shared_ptr<rename_query> query = std::make_shared<rename_query>();
       Helpers::make_query(user, query, entry);
       query->value = value;
       Kernel->Store->Push(query);
}

void KeyHelper::Set(User* user, const std::string& entry, const std::string& value)
{
       std::shared_ptr<set_query> query = std::make_shared<set_query>();
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

void KeyHelper::Strlen(User* user, const std::string& key)
{
       std::shared_ptr<strlen_query> query = std::make_shared<strlen_query>();
       Helpers::make_query(user, query, key);
       Kernel->Store->Push(query);
}

void KeyHelper::Count(User* user, const std::string& key)
{
       std::shared_ptr<count_query> query = std::make_shared<count_query>();
       Helpers::make_query(user, query);
       query->key = key;
       Kernel->Store->Push(query);
}

void KeyHelper::Exists(User* user, const std::string& key)
{
       std::shared_ptr<exists_query> query = std::make_shared<exists_query>();
       Helpers::make_query(user, query, key);
       Kernel->Store->Push(query);
}

void KeyHelper::Delete(User* user, const std::string& key)
{
       std::shared_ptr<del_query> query = std::make_shared<del_query>();
       Helpers::make_query(user, query, key);
       Kernel->Store->Push(query);
}

void KeyHelper::Delete(User* user, std::shared_ptr<Database> database, const std::string& where, const std::string& key, bool quiet)
{
       std::shared_ptr<del_query> query = std::make_shared<del_query>();

       query->user = user;
       query->database = database;
       query->select_query = where;
       query->quiet = quiet;
       query->key = key;
       query->dest = to_bin(query->key) + ":" + query->select_query + ":" + query->base_request;
       
       Kernel->Store->Push(query);
}

void KeyHelper::Search(User* user, const std::string& key, signed int offset, signed int limit)
{
       std::shared_ptr<search_query> query = std::make_shared<search_query>();
       Helpers::make_query(user, query, stripe(key));
       query->limit = limit;
       query->offset = offset;
       Kernel->Store->Push(query);
}

void KeyHelper::Find(User* user, const std::string& key, signed int offset, signed int limit)
{
       std::shared_ptr<find_query> query = std::make_shared<find_query>();
       Helpers::make_query(user, query);

       query->limit = limit;
       query->offset = offset;
       query->key = stripe(key);
       Kernel->Store->Push(query);
}

/*void //KeyHelper::Append(User* user, std::shared_ptr<Database> database, const std::string& where, const std::string& key, const std::string& value)
{
       std::shared_ptr<append_query> query = std::make_shared<append_query>();
              

       query->database = database;
       query->user = user;
       query->value = stripe(value);
       query->select_query = where;
       query->int_keys = INT_KEYS;
       query->key = key;
       
       query->format = query->int_keys + query->select_query + ":" + to_bin(query->key);
       
       Kernel->Store->Push(query);
}

void //KeyHelper::Touch(User* user, std::shared_ptr<Database> database, const std::string& where, const std::string& key, const std::string& value, QUERY_TYPE type)
{
       std::shared_ptr<touch_query> query = std::make_shared<touch_query>();
              

       query->database = database;
       query->user = user;
       query->value = stripe(value);
       query->select_query = where;
       query->qtype = type;
       query->int_keys = INT_KEYS;
       query->key = key;
       
       query->format = query->int_keys + query->select_query + ":" + to_bin(query->key);
       
       Kernel->Store->Push(query);
}

void //KeyHelper::GetRange(User* user, std::shared_ptr<Database> database, const std::string& where, const std::string& key, int from, int to)
{
       std::shared_ptr<advget_query> query = std::make_shared<advget_query>();
              

       query->database = database;
       query->select_query = where;
       query->int_keys = INT_KEYS;
       query->user = user;
       query->qtype = TYPE_GETRANGE;
       query->key = key;
       
       query->from = from;
       query->to = to;

       query->format = query->int_keys + query->select_query + ":" + to_bin(query->key);

       Kernel->Store->Push(query);
}

void //KeyHelper::AdvSet(User* user, std::shared_ptr<Database> database, const std::string& where, const std::string& key, const std::string& value, QUERY_TYPE type)
{
       std::shared_ptr<advget_query> query = std::make_shared<advget_query>();
              

       query->database = database;
       query->select_query = where;
       
       if (type == TYPE_GETSET)
       {
               query->value = stripe(value);
       }
       else
       {
              query->value = value;
       }
       
       
       query->int_keys = INT_KEYS;
       query->user = user;
       query->qtype = type;
       query->key = key;

       query->format = query->int_keys + query->select_query + ":" + to_bin(query->key);

       Kernel->Store->Push(query);
}

void //KeyHelper::AdvancedGET(User* user, std::shared_ptr<Database> database, const std::string& where, const std::string& key, QUERY_TYPE type)
{
       std::shared_ptr<advget_query> query = std::make_shared<advget_query>();
              

       query->database = database;
       query->select_query = where;
       query->int_keys = INT_KEYS;
       query->user = user;
       query->qtype = type;
       query->key = key;

       query->format = query->int_keys + query->select_query + ":" + to_bin(query->key);

       Kernel->Store->Push(query);
}

void //KeyHelper::Move(User* user, std::shared_ptr<Database> database, const std::string& where, const std::string& entry, const std::string& new_select)
{
       std::shared_ptr<move_query> query = std::make_shared<move_query>();
              

       query->database = database;
       query->user = user;
       query->select_query = where;
       query->int_keys = INT_KEYS;
       query->key = entry;
       query->format = query->int_keys + query->select_query + ":" + to_bin(query->key);
       query->newkey = new_select;
       
       Kernel->Store->Push(query);
}

void //KeyHelper::Count(User* user, std::shared_ptr<Database> database, const std::string& where, const std::string& key, const std::string& customreply)
{
       std::shared_ptr<find_query> query = std::make_shared<find_query>();
              
       
       query->customreply = customreply;

       query->database = database;
       query->user = user;
       query->select_query = where;
       query->int_keys = INT_KEYS;
       query->key = key;
       
       query->format = query->int_keys + query->select_query + ":" + to_bin(query->key);
       query->limit = 0;
       query->offset = 0;

       query->qtype = TYPE_COUNT_RECORDS;
       
       Kernel->Store->Push(query);
}

void //KeyHelper::Find(User* user, std::shared_ptr<Database> database, const std::string& where, const std::string& key, signed int offset, signed int limit, QUERY_TYPE type)
{
       std::shared_ptr<find_query> query = std::make_shared<find_query>();
              

       query->database = database;
       query->user = user;
       query->limit = limit;
       query->offset = offset;
       query->qtype = type;
       query->select_query = where;
       query->int_keys = INT_KEYS;
       query->key = key;
       
       Kernel->Store->Push(query);
}

void //KeyHelper::Expire(User* user, std::shared_ptr<Database> database, const std::string& where, const std::string& key, QUERY_TYPE type, unsigned int usig, std::string value)
{
       if (type == TYPE_EXPIREAT)
       {
              /* Epoch time already passed. */
  /*            
              if (usig < Kernel->Now())
              {
                     return;
              }
       }
       
       std::shared_ptr<get_query> query = std::make_shared<get_query>();
              

       query->database = database;
       query->select_query = where;
       query->int_keys = INT_KEYS;
       query->user = user;
       query->qtype = type;
       query->key = key;
       
       if (type != TYPE_EXPIREAT)
       {
              query->value = stripe(value);
       }
       
       query->id = usig;

       query->format = query->int_keys + query->select_query + ":" + to_bin(query->key);

       Kernel->Store->Push(query);
}

void //KeyHelper::Get(User* user, std::shared_ptr<Database> database, const std::string& where, const std::string& key, QUERY_TYPE type)
{
       std::shared_ptr<get_query> query = std::make_shared<get_query>();
              
       
       query->database = database;
       query->select_query = where;
       query->int_keys = INT_KEYS;
       query->user = user;
       query->qtype = type;
       query->key = key;
       
       query->format = query->int_keys + query->select_query + ":" + to_bin(query->key);
       
       Kernel->Store->Push(query);
}

void //KeyHelper::Set(User* user, std::shared_ptr<Database> database, const std::string& where, const std::string& entry, const std::string& value)
{
       std::shared_ptr<set_query> query = std::make_shared<set_query>();
       query->database = database;
       query->user = user;
       query->qtype = type;
       query->select_query = where;
       query->key = entry;
       query->value = stripe(value);
       query->format = query->select_query + ":" + to_bin(query->key);
       Kernel->Store->Push(query);
}

void //KeyHelper::Operation(User* user, std::shared_ptr<Database> database, const std::string& where, const std::string& key, OP_TYPE type, const std::string& oper)
{
       std::shared_ptr<op_query> query = std::make_shared<op_query>();
              

       query->database = database;
       query->operation = type;
       query->user = user;
       
       query->select_query = where;
       query->int_keys = INT_KEYS;
       query->key = key;
       
       Args arglist2;
       
       if (type == OP_ADD || type == OP_MIN || type == OP_AVG || type == OP_DIV || type == OP_MULT)
       {
              arglist2.push_back(oper);
       }

       query->format = query->int_keys + query->select_query + ":" + to_bin(query->key);

       query->q_args = arglist2;
       Kernel->Store->Push(query);
}


*/