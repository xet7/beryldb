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

#include <iterator>

namespace brld
{

struct node_list_def_tag 
{ 

};

template <typename T, typename Tag = node_list_def_tag> class node_list;
template <typename T, typename Tag = node_list_def_tag> class node_list_tail;

template <typename T, typename Tag = node_list_def_tag>
class node_list_node
{
  private:
  
	T* ptr_next;
	T* ptr_prev;

	void unlink()
	{
		if (ptr_next)
		{
			ptr_next->node_list_node<T, Tag>::ptr_prev = this->ptr_prev;
		}
		
		if (ptr_prev)
		{
			ptr_prev->node_list_node<T, Tag>::ptr_next = this->ptr_next;
		}
		
		ptr_next = ptr_prev = NULL;
	}

 public:

	node_list_node() : ptr_next(NULL), ptr_prev(NULL)
	{
	
	}

	friend class node_list<T, Tag>;
	friend class node_list_tail<T, Tag>;
};

} 
#define BERYL_node_list_NAME node_list
#include "ilist_impl.h"
#undef BERYL_node_list_NAME

#define BERYL_node_list_NAME node_list_tail
#define BERYL_node_list_HAS_TAIL
#include "ilist_impl.h"
#undef BERYL_node_list_NAME
#undef BERYL_node_list_HAS_TAIL
