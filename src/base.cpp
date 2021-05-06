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

#include <time.h>

#include "beryl.h"
#include "base.h"

base_class::base_class()
{

}

DiscardResult base_class::discard()
{
	return DiscardResult();
}

base_class::~base_class()
{

}

DiscardResult::DiscardResult()
{

}

static void* last_iter = NULL;

void* refcountbase::operator new(size_t size)
{
	last_iter = ::operator new(size);
	return last_iter;
}

void refcountbase::operator delete(void* obj)
{
	if (last_iter == obj)
	{
		last_iter = NULL;
	}
	
	::operator delete(obj);
}

refcountbase::refcountbase() : refcount(0)
{
	if (this != last_iter)
	{
		throw KernelException("Reference allocate on the stack!");
	}
}

refcountbase::~refcountbase()
{

}

usecountbase::~usecountbase()
{

}

ServiceProvider::~ServiceProvider()
{

}

void ServiceProvider::RegisterService()
{

}

extension_item::extension_item(const std::string& Key, ExpandableType exttype, Module* mod)
	: ServiceProvider(mod, Key, SERVICE_STREAM)
	, type(exttype)
{

}

extension_item::~extension_item()
{
}

void* extension_item::get_raw(const Expandable* container) const
{
	Expandable::ExpandableCore::const_iterator i = container->extensions.find(const_cast<extension_item*>(this));
	
	if (i == container->extensions.end())
	{
		return NULL;
	}
	
	return i->second;
}

void* extension_item::set_raw(Expandable* container, void* value)
{
	std::pair<Expandable::ExpandableCore::iterator,bool> rv = container->extensions.insert(std::make_pair(this, value));
	
	if (rv.second)
	{
		return NULL;
	}
	else
	{
		void* old = rv.first->second;
		rv.first->second = value;
		return old;
	}
}

void* extension_item::unset_raw(Expandable* container)
{
	Expandable::ExpandableCore::iterator i = container->extensions.find(this);

	if (i == container->extensions.end())
	{
		return NULL;
	}
	
	void* rv = i->second;
	container->extensions.erase(i);
	return rv;
}

void extension_item::RegisterService()
{
	if (!Kernel->Extensions.Register(this))
	{
		throw ModuleException("Extension already exists: " + name);
	}
}

bool ExtensionManager::Register(extension_item* item)
{
	return types.insert(std::make_pair(item->name, item)).second;
}

void ExtensionManager::BeginUnregister(Module* module, std::vector<reference<extension_item> >& list)
{
	ExtMap::iterator i = types.begin();
	
	while (i != types.end())
	{
		ExtMap::iterator me = i++;
		extension_item* item = me->second;
	
		if (item->creator == module)
		{
			list.push_back(item);
			types.erase(me);
		}
	}
}

extension_item* ExtensionManager::GetItem(const std::string& name)
{
	ExtMap::iterator i = types.find(name);
	
	if (i == types.end())
	{
		return NULL;
	}
	
	return i->second;
}

void Expandable::UnlinkExtensions(const std::vector<reference<extension_item> >& toRemove)
{
	for(std::vector<reference<extension_item> >::const_iterator i = toRemove.begin(); i != toRemove.end(); ++i)
	{
		extension_item* item = *i;
		ExpandableCore::iterator e = extensions.find(item);
	
		if (e != extensions.end())
		{
			item->free(this, e->second);
			extensions.erase(e);
		}
	}
}

Expandable::Expandable() : culled(false)
{

}

DiscardResult Expandable::discard()
{
	free_all_items();
	culled = true;
	return base_class::discard();
}

void Expandable::free_all_items()
{
	for (ExpandableCore::iterator i = extensions.begin(); i != extensions.end(); ++i)
	{
		i->first->free(this, i->second);
	}

	extensions.clear();
}

Expandable::~Expandable()
{

}

void extension_item::from_internal(Expandable* container, const std::string& value)
{
	from_network(container, value);
}

void extension_item::from_network(Expandable* container, const std::string& value)
{

}

std::string extension_item::to_human(const Expandable* container, void* item) const
{
	std::string ret = to_network(container, item);

	if (ret.empty())
	{
		ret = to_internal(container, item);
	}

	return ret;
}

std::string extension_item::to_internal(const Expandable* container, void* item) const
{
	return to_network(container, item);
}

std::string extension_item::to_network(const Expandable* container, void* item) const
{
	return std::string();
}


local_string_extension::local_string_extension(const std::string& Key, ExpandableType exttype, Module* Owner)
	: SimpleExtItem<std::string>(Key, exttype, Owner)
{

}

local_string_extension::~local_string_extension()
{

}

std::string local_string_extension::to_internal(const Expandable* container, void* item) const
{
	return item ? *static_cast<std::string*>(item) : std::string();
}

void local_string_extension::from_internal(Expandable* container, const std::string& value)
{
	set(container, value);
}

local_int_extension::local_int_extension(const std::string& Key, ExpandableType exttype, Module* mod)
	: extension_item(Key, exttype, mod)
{

}

local_int_extension::~local_int_extension()
{

}

std::string local_int_extension::to_internal(const Expandable* container, void* item) const
{
	return convto_string(reinterpret_cast<intptr_t>(item));
}

void local_int_extension::from_internal(Expandable* container, const std::string& value)
{
	set(container, convto_num<intptr_t>(value));
}

intptr_t local_int_extension::get(const Expandable* container) const
{
	return reinterpret_cast<intptr_t>(get_raw(container));
}

intptr_t local_int_extension::set(Expandable* container, intptr_t value)
{
	if (value)
		return reinterpret_cast<intptr_t>(set_raw(container, reinterpret_cast<void*>(value)));
	else
		return reinterpret_cast<intptr_t>(unset_raw(container));
}

void local_int_extension::free(Expandable* container, void* item)
{
}

string_extension_item::string_extension_item(const std::string& Key, ExpandableType exttype, Module* mod)
	: extension_item(Key, exttype, mod)
{
}

string_extension_item::~string_extension_item()
{
}

std::string* string_extension_item::get(const Expandable* container) const
{
	return static_cast<std::string*>(get_raw(container));
}

std::string string_extension_item::to_network(const Expandable* container, void* item) const
{
	return item ? *static_cast<std::string*>(item) : std::string();
}

void string_extension_item::from_network(Expandable* container, const std::string& value)
{
	if (value.empty())
	{
		unset(container);
	}
	else
	{
		set(container, value);
	}
}

void string_extension_item::set(Expandable* container, const std::string& value)
{
	void* old = set_raw(container, new std::string(value));
	free(container, old);
}

void string_extension_item::unset(Expandable* container)
{
	void* old = unset_raw(container);
	free(container, old);
}

void string_extension_item::free(Expandable* container, void* item)
{
	delete static_cast<std::string*>(item);
}

ModuleException::ModuleException(const std::string &message, Module* who) : KernelException(message, who ? who->SourceFile : "A Module")
{

}
