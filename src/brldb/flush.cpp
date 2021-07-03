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
#include "brldb/handler.h"
#include "brldb/database.h"
#include "brldb/datathread.h"
#include "brldb/dbnumeric.h"
#include "brldb/dbflush.h"
#include "brldb/query.h"

void DataFlush::EntryExists(User* user, std::shared_ptr<query_base> signal)
{
        Dispatcher::Smart(user, 0, ERR_QUERY, PROCESS_FALSE, signal);
}

void DataFlush::NotFound(User* user, std::shared_ptr<query_base> signal)
{
        user->SendProtocol(ERR_QUERY, PROCESS_NULL);
}

void DataFlush::StatusFailed(User* user, std::shared_ptr<query_base> signal)
{
        user->SendProtocol(ERR_QUERY, DB_NULL);
}

void DataFlush::MissArgs(User* user, std::shared_ptr<query_base> signal)
{
        user->SendProtocol(ERR_QUERY, MIS_ARGS);
}

void DataFlush::InvalidType(User* user,  std::shared_ptr<query_base> signal)
{
       user->SendProtocol(ERR_QUERY, INVALID_TYPE);
}

void DataFlush::UnableWrite(User* user, std::shared_ptr<query_base> signal)
{
     user->SendProtocol(ERR_QUERY, PROCESS_FALSE);

}

void DataFlush::Flush(User* user, std::shared_ptr<query_base> signal)
{
        signal->Process();
        signal = nullptr;
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

