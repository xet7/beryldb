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
#include "engine.h"

/* 
 * Checks whether a given number is valid and, positive.
 * 
 * @parameters:
 *
 *         · string: String to check.
 *
 * @return:
 *
 *         · True: Valid number.
 *         · False: Invalid number.
 */

inline bool CheckValidPos(User* user, const std::string& number)
{
       if (!is_number(number))
       {
                 user->SendProtocol(ERR_INPUT, MUST_BE_NUMERIC);
                 return false;
       }

       if (!is_positive_number(number))
       {
                user->SendProtocol(ERR_INPUT, MUST_BE_POSIT);
                return false;
       }
       
       return true;
}

/* 
 * Checks whether provided value is numeric.
 * 
 * @parameters:
 *
 *         · number: Number to check.
 * 
 * @return:
 *
 *         · True: Is numeric.
 */    
 
inline bool CheckValid(User* user, const std::string& number)
{
       if (!is_number(number))
       {
                 user->SendProtocol(ERR_INPUT, MUST_BE_NUMERIC);
                 return false;
       }
       
       return true;
}

/* 
 * Returns a vector containing limits.
 * 
 * @parameters:
 *
 *         · max: Max parameters.
 *         · Params: Parameters as given by the original command.,
 *
 * @return:
 *
 *         · Vector: Returns { 0 } if invalid.
 */


inline std::vector<signed int> GetLimits(User* user, unsigned int max, const CommandModel::Params& parameters)
{
       signed int offset;
       signed int limit;

       if (parameters.size() == (max - 1))
       {
             if (!is_zero_or_great_or_mone(parameters[(max - 2)]))
             {
                 user->SendProtocol(ERR_INPUT, MUST_BE_GREAT_ZERO);
                 return { 0 };
             }
       
             limit = convto_num<signed int>(parameters[(max - 2)]); 
             offset = 0;
       }
       else if (parameters.size() == max)
       {
             if (!is_zero_or_great_or_mone(parameters[(max - 1)]) || !is_zero_or_great(parameters[(max - 2)]))
             {
                   user->SendProtocol(ERR_INPUT, MUST_BE_GREAT_ZERO);
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

/* 
 * Checks the format of a string (ie "string query").
 * 
 * @parameters:
 *
 *         · value: String to check.
 *         · notify: Whether this function notifies to the requesting user.
 *
 * @return:
 *
 *         · True: Valid query.
 *         · False: Invalid query.
 */

inline bool CheckFormat(User* user, const std::string& value, bool notify = true)
{
        if (value.size() == 0)
        {
            if (notify)
            {
                 user->SendProtocol(ERR_INPUT, INVALID_TYPE);
            }

            return false;
        }

        if (value.size() == 1)
        {
             return true;
        }

        if (is_number(value, true))
        {
             return true;
        }

        if (is_correct(value))
        {
             return true;
        }

        if (notify)
        {
                user->SendProtocol(ERR_INPUT, INVALID_VALUE);
        }

        return false;
}

/* 
 * Checks whether a given key is valid.
 * 
 * @parameters:
 *
 *         · string: Key to check.
 *
 * @return:
 *
 *         · True: Valid key.
 */

inline bool CheckKey(User* user, const std::string& value, bool notify = true)
{
        if (!value.size())
        {
              if (notify)
              {
                  user->SendProtocol(ERR_INPUT, INVALID_KEY);
              }

              return false;
        }
        
        if (Kernel->Engine->ValidKey(value))
        {
              return true;
        }
        
        if (notify)
        {
              user->SendProtocol(ERR_INPUT, INVALID_KEY);
        }
        
        return false;
}
