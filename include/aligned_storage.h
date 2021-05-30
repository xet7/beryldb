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

namespace brld
{
	template <typename T> class aligned_storage;
}

template <typename T> class brld::aligned_storage
{

  private:
  
	mutable typename STR1::aligned_storage<sizeof(T), STR1::alignment_of<T>::value>::type data;

 public:

	aligned_storage()
	{
	
	}

	aligned_storage(const aligned_storage& other)
	{
	
	}

	T* operator->() const
	{
		return static_cast<T*>(static_cast<void*>(&data));
	}

	operator T*() const
	{
		return operator->();
	}
};
