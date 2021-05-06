
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

#pragma once

#include "forwards.h"

typedef STR1::unordered_multimap<std::string, User*, engine::insensitive, engine::str_hash_comparison> LoginHash;

typedef STR1::unordered_map<std::string, User*, engine::insensitive, engine::str_hash_comparison> UserMap;
typedef STR1::unordered_map<std::string, Channel*, engine::insensitive, engine::str_hash_comparison> ChanMap;
typedef STR1::unordered_map<std::string, std::shared_ptr<Database>, engine::insensitive, engine::str_hash_comparison> DataMap;

typedef std::vector<Subscription*> SubscriptionList;

typedef std::vector<std::string> file_cache;

typedef brld::flat_map<std::string, std::string, engine::insensitive_swo> file_config_items;

/* Keeps copy of the entire config file. */

typedef std::multimap<std::string, reference<config_rule>, engine::insensitive_swo> ConfigMap;

typedef ConfigMap::const_iterator config_iterator;

typedef std::pair<config_iterator, config_iterator> MultiTag;

typedef std::map<std::string, file_cache> cached_config;

typedef std::set<User*> DiscardList;

typedef std::pair<std::string, std::string> IdentHostPair;

namespace Status
{
	class Context;
}

