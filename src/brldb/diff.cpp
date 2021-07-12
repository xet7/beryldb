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
#include "brldb/diff.h"

ListMap DiffHandler::CompareList(const ListMap list, const ListMap list2)
{
      ListMap diff;
      
      for (ListMap::const_iterator i = list.begin(); i != list.end(); i++)
      {
              std::string item = *i;
              bool found = std::find(list2.begin(), list2.end(), item) != list2.end();
              
              if (!found)
              {
                  diff.push_back(item);  
              }
      }
      
      return diff;
}

Args DiffHandler::CompareVector(const Args& list, const Args& list2)
{
      Args diff;
      
      for (Args::const_iterator i = list.begin(); i != list.end(); i++)
      {
              std::string item = *i;
              bool found = std::find(list2.begin(), list2.end(), item) != list2.end();
              
              if (!found)
              {
                  diff.push_back(item);  
              }
      }
      
      return diff;
}

Args DiffHandler::CompareMultiMap(const MultiMap list, const MultiMap list2)
{
      Args result;
      
      for (MultiMap::const_iterator i = list.begin(); i != list.end(); i++)
      {
            std::string item = i->first;
            
            bool found = false;

            for (MultiMap::const_iterator i2 = list2.begin(); i2 != list2.end(); i2++)
            {
                 std::string item2 = i2->first;
                            
                 if (item2 != item)
                 {
                      found = true;
                 }
            }
            
            if (found)
            {
                   result.push_back(item);
            }
      }
      
      return result;
}


Args DiffHandler::CompareMap(const MapMap list, const MapMap list2)
{
      Args result;
      
      for (MapMap::const_iterator i = list.begin(); i != list.end(); i++)
      {
            std::string item = i->first;

            bool found = false;
            
            for (MapMap::const_iterator i2 = list2.begin(); i2 != list2.end(); i2++)
            {
                 std::string item2 = i2->first;
                            
                 if (item2 != item)
                 {
                       found = true;
                 }
            }
            
            if (found)
            {
                 result.push_back(item);
            }
      }
      
      return result;
}
