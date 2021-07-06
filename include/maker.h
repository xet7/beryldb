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

#include "beryl.h"

inline std::vector<signed int> GetLimits(User* user, unsigned int max, const CommandModel::Params& parameters)
{
       signed int offset;
       signed int limit;

       if (parameters.size() == (max - 1))
       {
             if (!is_zero_or_great_or_mone(parameters[(max - 2)]))
             {
                 user->SendProtocol(ERR_USE, ERR_GREAT_ZERO, MUST_BE_GREAT_ZERO.c_str());
                 return { 0 };
             }
       
             limit = convto_num<signed int>(parameters[(max - 2)]); 
             offset = 0;
       }
       else if (parameters.size() == max)
       {
             if (!is_zero_or_great_or_mone(parameters[(max - 1)]) || !is_zero_or_great(parameters[(max - 2)]))
             {
                   user->SendProtocol(ERR_USE, ERR_GREAT_ZERO, MUST_BE_GREAT_ZERO.c_str());
                   return { 0 };
             }
       
             limit = convto_num<signed int>(parameters[(max - 1)]); 
             offset = convto_num<signed int>(parameters[(max - 2)]);
       }
       else
       {
            limit = -1;
            offset = 0;
       }
       
       std::vector<signed int> created = { 1, offset, limit };
       return created;
}