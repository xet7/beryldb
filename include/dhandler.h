
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

class ExportAPI DLLHandler : public Discarder
{
	
	private:

		std::string error;
		
		void* lib;

		const std::string libname;

		void GetLastError();

	public:

		DLLHandler(const std::string& name);

		~DLLHandler();

		Module* StartMod();

		void* GetSymbol(const char* name) const;

		template <typename TReturn> TReturn* GetSymbol(const char* name) const
		{
			return static_cast<TReturn*>(GetSymbol(name));
		}

		const char* GetDescription() const 
		{ 
			return GetSymbol<const char>(MODULE_STR_VERSION); 
		}

		const std::string& LastError() const 
		{ 
			return this->error; 
		}

		const std::string& LibraryName() const 
		{ 
			return libname; 
		}
};
