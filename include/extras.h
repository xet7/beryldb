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

#define SET_INDEX(start, end, len)         \
    if (end > len)                          \
        end = len;                          \
    else if (end < 0) {                     \
        end += len;                         \
        if (end < 0)                        \
        end = 0;                        \
    }                                       \
    if (start < 0) {                        \
        start += len;                       \
        if (start < 0)                      \
        start = 0;                      \
    }

/* 
 * Returns lowest index in a string where a substring is found.
 *
 * @parameters:
 *
 *         · string: string.
 *         · string: substring.
 *         · start: start from.
 *         · end: Found.
 * 
 * @return:
 *
 *         · int: counter.
 */ 

inline int find(const std::string & str, const std::string & sub, int start = 0, int end = MAX_32BIT_INT)
{
        SET_INDEX(start, end, (int) str.size());
        
        std::string::size_type result = str.find( sub, start );
        
        if (result == std::string::npos || (result + sub.size() > (std::string::size_type)end))
        {
             return -1;
        }
        
        return (int) result;
}

/* 
 * Returns number of occurrences of substring in a string. 
 *
 * @parameters:
 *
 *         · string: string.
 *         · string: substring.
 * 
 * @return:
 *
 *         · int: counter.
 */ 

inline int count_occur(const std::string & str, const std::string & substr, int start = 0, int end = MAX_32BIT_INT)
{
        int matches = 0;
        int cursor = start;

        while (1)
        {
            cursor = find(str, substr, cursor, end);

            if (cursor < 0)
            {
                 break;
            }

            cursor += (int) substr.size();
            matches += 1;
        }

        return matches;
}

/* 
 * Checks if string is alpha numeric and has at least
 * one character.
 * 
 * @parameters:
 *
 *         · string: text
 * 
 * @return:
 *
 *         · boolean: True if it is alpha, false if not.
 */ 

inline bool isalpha(const std::string & str)
{
        std::string::size_type len = str.size(), i;
        
        if (len == 0) 
        {	
            return false;
        }
        
        if (len == 1)
        {
            return ::isalpha( (int) str[0] );
         }

        for (i = 0; i < len; ++i)
        {
            if (!::isalpha( (int) str[i] )) return false;
        }
        return true;
}