/*
 * BerylDB - A modular database.
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

#pragma once

class Externalize extension_item : public ServiceProvider, public usecountbase
{
 
  public:
	
	enum ExpandableType
	{
		
		EXT_USER,

		
		EXT_CHANNEL,

		
		EXT_SUBSCRIPTION
	};

	
	const ExpandableType type;

	
	extension_item(const std::string& key, ExpandableType exttype, Module* owner);

	
	virtual ~extension_item();

	
	virtual void from_internal(Expandable* container, const std::string& value);

	
	virtual void from_network(Expandable* container, const std::string& value);

	
	virtual std::string to_human(const Expandable* container, void* item) const;
	
	virtual std::string to_internal(const Expandable* container, void* item) const ;

	
	virtual std::string to_network(const Expandable* container, void* item) const;

	
	virtual void free(Expandable* container, void* item) = 0;

	
	void RegisterService() ;


 protected:
	
	void* get_raw(const Expandable* container) const;

	
	void* set_raw(Expandable* container, void* value);

	
	void* unset_raw(Expandable* container);
};


class Externalize Expandable : public base_class, public Serializable
{
 public:
 
	typedef brld::flat_map<reference<extension_item>, void*> ExpandableCore;
	friend class extension_item;
	
 private:
	
	ExpandableCore extensions;

	
	unsigned int culled:1;
	
 public:
	
	inline const ExpandableCore& GetExtList() const 
	{ 
		return extensions; 
	}

	Expandable();
	
	DiscardResult discard();
	
	virtual ~Expandable();
	
	void UnlinkExtensions(const std::vector<reference<extension_item> >& toRemove);
	
	void free_all_items();
	
	bool Deserialize(Data& data);

	bool Serialize(Serializable::Data& data) ;
};

class Externalize ExtensionManager
{
 public:

	typedef std::map<std::string, reference<extension_item> > ExtMap;

	bool Register(extension_item* item);
	void BeginUnregister(Module* module, std::vector<reference<extension_item> >& list);
	extension_item* GetItem(const std::string& name);

	const ExtMap& GetExts() const { return types; }

 private:
 
	ExtMap types;
};


typedef extension_item LocalExtItem;

template <typename T, typename Del = stdhelpers::defaultdeleter<T> >
class SimpleExtItem : public extension_item
{
 public:
	SimpleExtItem(const std::string& Key, ExpandableType exttype, Module* parent)
		: extension_item(Key, exttype, parent)
	{
	}

	virtual ~SimpleExtItem()
	{
	}

	inline T* get(const Expandable* container) const
	{
		return static_cast<T*>(get_raw(container));
	}

	inline void set(Expandable* container, const T& value)
	{
		T* ptr = new T(value);
		T* old = static_cast<T*>(set_raw(container, ptr));
		free(container, old);
	}

	inline void set(Expandable* container, T* value)
	{
		T* old = static_cast<T*>(set_raw(container, value));
		free(container, old);
	}

	inline void unset(Expandable* container)
	{
		T* old = static_cast<T*>(unset_raw(container));
		free(container, old);
	}

	void free(Expandable* container, void* item) 
	{
		Del del;
		del(static_cast<T*>(item));
	}
};

class Externalize local_string_extension : public SimpleExtItem<std::string>
{
 public:
	local_string_extension(const std::string& key, ExpandableType exttype, Module* owner);
	virtual ~local_string_extension();
	std::string to_internal(const Expandable* container, void* item) const ;
	void from_internal(Expandable* container, const std::string& value) ;
};

class Externalize local_int_extension : public extension_item
{
 public:
	local_int_extension(const std::string& key, ExpandableType exttype, Module* owner);
	virtual ~local_int_extension();
	std::string to_internal(const Expandable* container, void* item) const ;
	void from_internal(Expandable* container, const std::string& value) ;
	intptr_t get(const Expandable* container) const;
	intptr_t set(Expandable* container, intptr_t value);
	void unset(Expandable* container) { set(container, 0); }
	void free(Expandable* container, void* item) ;
};

class Externalize string_extension_item : public extension_item
{
 public:
	string_extension_item(const std::string& key, ExpandableType exttype, Module* owner);
	virtual ~string_extension_item();
	std::string* get(const Expandable* container) const;
	std::string to_network(const Expandable* container, void* item) const ;
	void from_network(Expandable* container, const std::string& value) ;
	void set(Expandable* container, const std::string& value);
	void unset(Expandable* container);
	void free(Expandable* container, void* item) ;
};
