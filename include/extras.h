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

/* 
 * Checks whether a given number is a valid longitude.
 * 
 * @parameters:
 *
 *         · int number: Number to check.
 * 
 * @return:
 *
 *         · True: Valid.
 *           False: Invalid.
 */ 

inline bool ValidLong(int number)
{
      if (number <= 180 && number >= -180)
      {
             return true;
      }
    
      return false;
}

/* 
 * Checks whether a given number is a valid latitude.
 * 
 * @parameters:
 *
 *         · int number: Number to check.
 * 
 * @return:
 *
 *         · True: Valid.
 *           False: Invalid.
 */ 

inline bool ValidLat(int number)
{
    if (number <= 90 && number >= -90)
    {
            return true;
    }

    return false;
}

/* 
 * Removes quote marks from a string. 
 * 
 * @parameters:
 *
 *         · string str: Data to check.
 * 
 * @return:
 *
 *         · string: Full string without quote marks.
 */

inline std::string stripe(const std::string& str)
{
        if (is_number(str, true))
        {
                return str;
        }

        if (str.length() == 1)
        {
                return str;
        }

        if (str.length() < 1)
        {
                return "";
        }

        std::string aux = str; 
        aux.erase(0, 1);
        aux.pop_back();

        return aux;
}
