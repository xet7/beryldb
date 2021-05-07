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

enum DBL_CODE
{
    DBL_TYPE_HSET 		=		1,
    DBL_STATUS_FAILED		=		2,
    DBL_STATUS_OK		=		3,
    DBL_MISS_ARGS		=		4,
    DBL_NOT_FOUND		=		5,
    DBL_TYPE_HGET		=		6,
    DBL_ENTRY_EXISTS		=		7,
    DBL_MANAGER_OK		=		8,
    DBL_TYPE_LGET		=		9,
    DBL_TYPE_LPUSH		=		10,
    DBL_TYPE_LPOP		=		11,    
    DBL_TYPE_HLIST    		=		12,
    DBL_TYPE_HDEL		=		13,
    DBL_TYPE_GET		=		14,
    DBL_TYPE_SET		=		15,
    DBL_STATUS_BROKEN		=		16,
    DBL_TYPE_DEL		=		17,
    DBL_TYPE_LLIST		=		21,
    DBL_TYPE_OP                 =               22,
    DBL_NOT_NUM			=		23,
    DBL_TYPE_FIND               =               24,
    DBL_TYPE_SEARCH		=		25,
    DBL_TYPE_MOVE               =               26,
    DBL_TYPE_HDEL_ALL		=		27,
    DBL_TYPE_HSEARCH    	=		28,
    DBL_TYPE_HKEYS   		=		29,
    DBL_TYPE_HSEARCH_HESH	=		30,
    DBL_TYPE_DBSIZE		=		31,
    DBL_QUERY_LARGE		=		32,
    DBL_INTERRUPT		=		33,
    DBL_NO_ENTRY		=		34,
    DBL_TYPE_LPOP_ALL		=		35,
    DBL_TYPE_ADVGET		=		36,
    DBL_TYPE_GETDEL		=		37,
    DBL_TYPE_HMOVE    		=		38,
    DBL_TYPE_TOUCH		=		39,
    DBL_TYPE_APPEND		=		40,
    DBL_TYPE_LMOVE		=		41
};
