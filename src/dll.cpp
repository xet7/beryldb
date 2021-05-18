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
 
#include <dlfcn.h>
 
#define DYNAMIC_EXTENSION ".so"

DLLHandler::DLLHandler(const std::string& name) : lib(NULL), libname(name)
{
	static size_t extlen = strlen(DYNAMIC_EXTENSION);
	
	if (name.length() <= extlen || name.compare(name.length() - extlen, name.length(), DYNAMIC_EXTENSION))
	{
		error.assign(name + " is invalid: there is no " DYNAMIC_EXTENSION " extension.");
		return;
	}

	lib = dlopen(name.c_str(), RTLD_NOW|RTLD_LOCAL);

	if (!lib)
	{
			GetLastError();
	}
}

DLLHandler::~DLLHandler()
{
	if (!lib)
	{
		return;
	}

	dlclose(lib);
}

Module* DLLHandler::CallInit()
{
	if (!lib)
	{
		return NULL;
	}

	const unsigned long* abi = GetSymbol<const unsigned long>(MODULE_STR_ABI);

	if (!abi)
	{
		error.assign(libname + " is not a valid module.");
		return NULL;
	}
	else if (*abi != MODULE_ABI)
	{
		const char* version = GetDescription();
		error.assign(Daemon::Format("%s was compiled on %s (%lu), which is not compatible (%s version) to use with %s (%lu).", libname.c_str(), version ? version : "an unknown version", *abi, *abi < MODULE_ABI ? "older" : "newer", VERSION, MODULE_ABI));
		return NULL;
	}

	union
	{
		void* vtable_address;
		Module* (*fptr)();
	};

	vtable_address = GetSymbol(MODULE_STR_INIT);

	if (!vtable_address)
	{
		error.assign(libname + " does not contain an init.");
		return NULL;
	}

	return (*fptr)();
}

void* DLLHandler::GetSymbol(const char* name) const
{
	if (!lib)
	{
		return NULL;
	}

	return dlsym(lib, name);
	
}

void DLLHandler::GetLastError()
{
	const char* ErrorMsg = dlerror();
	error = ErrorMsg ? ErrorMsg : "Unknown error";

	std::string::size_type p;
	
	while ((p = error.find_last_of("\r\n")) != std::string::npos)
	{
		error.erase(p, 1);
	}
}
