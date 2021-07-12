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

enum DBL_CODE
{
        DBL_NONE			=		0,
        DBL_STATUS_FAILED		=		2,
        DBL_STATUS_OK			=		3,
        DBL_MISS_ARGS			=		4,
        DBL_NOT_FOUND			=		5,
        DBL_ENTRY_EXISTS		=		7,
        DBL_MANAGER_OK			=		8,
        DBL_STATUS_BROKEN		=		16,
        DBL_NOT_NUM			=		23,
        DBL_QUERY_LARGE			=		32,
        DBL_INTERRUPT			=		33,
        DBL_INVALID_TYPE		=		56,
        DBL_UNABLE_WRITE		=		57,
        DBL_INVALID_FORMAT		=		58,
        DBL_INVALID_RANGE		=		59,
        DBL_ENTRY_EXPIRES               =               60,
        DBL_NOT_EXPIRING        	=		61	
};
