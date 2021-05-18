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

#pragma once

#include "base.h"

class Externalize ddl_base_ref : public interfacebase, public brld::node_list_node<ddl_base_ref>
{
 public:
	class aptureAttach
	{
	 public:
		
		virtual void OnCapture() = 0;
	};

 private:
	std::string name;
	aptureAttach* attach;
	void resolve();
 protected:

	ServiceProvider* value;

 public:

	module_reference creator;
	ddl_base_ref(Module* Creator, const std::string& Name);
	~ddl_base_ref();
	inline const std::string& GetProvider() const { return name; }
	void SetProvider(const std::string& newname);

	
	void SetaptureAttach(aptureAttach* h) { attach = h; }

	void check();
	operator bool() const { return (value != NULL); }
	static void reset_all();
};

inline void ddl_base_ref::check()
{
	if (!value)
	{
		throw ModuleException("Dynamic reference to '" + name + "' failed to resolve. Are you missing a module?");
	}
}

template<typename T>
class dynamic_reference : public ddl_base_ref
{
 public:
	dynamic_reference(Module* Creator, const std::string& Name)
		: ddl_base_ref(Creator, Name) {}

	inline T* operator->()
	{
		check();
		return static_cast<T*>(value);
	}

	T* operator*()
	{
		return operator->();
	}

	const T* operator->() const
	{
		return static_cast<T*>(value);
	}

	const T* operator*() const
	{
		return operator->();
	}
};

template<typename T>
class dynamic_reference_nocheck : public ddl_base_ref
{
 public:
	dynamic_reference_nocheck(Module* Creator, const std::string& Name)
		: ddl_base_ref(Creator, Name) {}

	T* operator->()
	{
		return static_cast<T*>(value);
	}

	T* operator*()
	{
		return operator->();
	}

	const T* operator->() const
	{
		return static_cast<T*>(value);
	}

	const T* operator*() const
	{
		return operator->();
	}
};
