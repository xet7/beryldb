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

#include "beryl.h"
#include "brldb/handler.h"
#include "brldb/database.h"
#include "brldb/datathread.h"
#include "brldb/dbnumeric.h"
#include "brldb/dbflush.h"
#include "brldb/query.h"

void DataFlush::Flush(User* user, std::shared_ptr<query_base> signal)
{
        switch (signal->type)
        {
                    case DBL_TYPE_SET:
                    {
                           Flusher::Set(user, signal);      
                           break; 
                    }

                    case DBL_TYPE_GET:
                    {
                           Flusher::Get(user, signal);      
                           break; 
                    }
                    
                    case DBL_TYPE_DEL:
                    {
                           Flusher::Del(user, signal);
                           break;
                    }
                    
                    case DBL_TYPE_HSET:
                    {
                           Flusher::HSet(user, signal);      
                           break; 
                    }

                    case DBL_TYPE_HGET:
                    {
                           Flusher::HGet(user, signal);      
                           break; 
                    }

                    case DBL_TYPE_HDEL:
                    {
                           Flusher::HDel(user, signal);
                           break;
                    }


                    case DBL_TYPE_LGET:
                    {
                           Flusher::LGet(user, signal);
                           break;
                    }

                    case DBL_TYPE_LPUSH:
                    {
                           Flusher::LPush(user, signal);
                           break;
                    }

                    case DBL_TYPE_LPOP:
                    {
                           Flusher::LPop(user, signal);
                           break;
                    }
                    
                    case DBL_TYPE_OP:
                    {
                           Flusher::Operation(user, signal);
                           break;
                    }
                    
                    case DBL_TYPE_FIND:
                    {
                           Flusher::Find(user, signal);
                           break;
                    }

                    case DBL_TYPE_SEARCH:
                    {
                           Flusher::HSearch(user, signal);
                           break;
                    }

                    case DBL_TYPE_MOVE:
                    {
                           Flusher::Move(user, signal);
                           break;
                    }
                    
                    case DBL_TYPE_HDEL_ALL:
                    {
                          Flusher::HDelAll(user, signal);
                          break;
                    }
                    
                    case DBL_TYPE_HSEARCH:
                    {
                          Flusher::HSearch_Hesh(user, signal);
                          break;
                    }
                    
                    case DBL_TYPE_HKEYS:
                    {
                         Flusher::HKeys(user, signal);
                         break;
                    }

                    case DBL_TYPE_DBSIZE:
                    {
                         Flusher::DBSize(user, signal);
                         break;
                    }

                    case DBL_TYPE_ADVGET:
                    {
                         Flusher::AdvGet(user, signal);
                         break;
                    }
                    
                    case DBL_TYPE_HMOVE:
                    {
                         Flusher::HMove(user, signal);
                         break;
                    }

                    case DBL_TYPE_TOUCH:
                    {
                         Flusher::Touch(user, signal);
                         break;
                    }
                    
                    case DBL_TYPE_APPEND:
                    {
                         Flusher::Append(user, signal);
                         break;
                    }
                    
                    case DBL_TYPE_LMOVE:
                    {
                         Flusher::LMove(user, signal);
                         break;
                    }
                    
                    case DBL_TYPE_SFLUSH:
                    {
                         Flusher::SFlush(user, signal);
                         break;
                    }
                    
                    case DBL_TYPE_KSEARCH:
                    {
                         Flusher::Search(user, signal);
                         break;
                    }
                    
        };      
}


ListHandler::ListHandler()
{

}

bool ListHandler::Has(const std::string& entry)
{
     engine::comma_node_stream CMD(this->data);
     std::string server;
     std::string find = to_bin(entry);

     while (CMD.items_extract(server))
     {
           if (server == find)
           {
                      return true;
           }
     }

     return false;
}

unsigned int ListHandler::CountItems()
{
     engine::comma_node_stream CMD(this->data);
     std::string server;
     unsigned int counter = 0;

      while (CMD.items_extract(server))
      {
               counter++;
      }

      return counter;
     
}

unsigned int ListHandler::Count(const std::string& entry)
{
      engine::comma_node_stream CMD(this->data);
      std::string server;
      std::string find = to_bin(entry);
      unsigned int counter = 0;

      while (CMD.items_extract(server))
      {
                  if (server == find)
                  {
                       counter++;
                  }
      }

      return counter;
}

void ListHandler::Remove(const std::string& entry, bool onlyfirst)
{
    engine::comma_node_stream CMD(this->data);
    std::string server;
    std::string removing = to_bin(entry);
    std::string newdata;

    unsigned int counter = 0;
    
    while (CMD.items_extract(server))
    {
                  if (server == removing)
                  {
                       counter++;

                       if (onlyfirst)
                       {
                            if (counter == 1)
                            {
                                    continue;
                            }
                       }
                       else
                       {
                            continue;
                      }
                  }

                  newdata.append(server + ",");
    }
    
    this->data = newdata;
}

std::vector<std::string> ListHandler::Divide()
{
       std::vector<std::string> returning;
       engine::comma_node_stream CMD(this->data);
       std::string server;

       while (CMD.items_extract(server))
       {
            std::string conv = to_string(server);
            returning.push_back(conv);
       }

       return returning;
}

std::vector<std::string> ListHandler::Divide(unsigned int offset, unsigned int limit)
{
       std::vector<std::string> returning;
       engine::comma_node_stream CMD(this->data);
       std::string server;
       
       unsigned int counter = 0;
       unsigned int aux_counter = 0;

       while (CMD.items_extract(server))
       {
            if (counter >= offset)
            {
                  if (aux_counter <= limit)
                  {
                          std::string conv = to_string(server);
                          returning.push_back(conv);
                  }
                  else
                  {
                          break; 
                  }
                  
                  aux_counter++;
            }
            
            counter++;
       }

       return returning;
}

