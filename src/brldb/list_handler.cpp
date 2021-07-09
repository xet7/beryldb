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
#include "brldb/list_handler.h"

ListHandler::ListHandler()
{

}

std::shared_ptr<ListHandler> ListHandler::Create(const std::string& load)
{
        std::shared_ptr<ListHandler> New = std::make_shared<ListHandler>();
        
        engine::colon_node_stream stream(load);
        std::string mask;

        while (stream.items_extract(mask))
        {
                New->Add(to_string(mask));
        }
        
        return New;
}

void ListHandler::PopFront()
{
      if (!this->mhandler.size())
      {
           this->LastMsg = HANDLER_MSG_OK;
            return;
      }
      
      if (this->mhandler.size() == 1)
      {
            this->Erase();
            this->LastMsg = HANDLER_MSG_OK;
            return;
      }

      this->mhandler.erase(this->mhandler.begin());
      this->LastMsg = HANDLER_MSG_OK;
}

void ListHandler::Erase()
{
       this->mhandler.clear();
       this->LastMsg = HANDLER_MSG_OK;
}

void ListHandler::PopBack()
{
      if (!this->mhandler.size())
      {
            this->LastMsg = HANDLER_MSG_NOT_FOUND;
            return;
      }

      if (this->mhandler.size() == 1)
      {
            this->Erase();
            this->LastMsg = HANDLER_MSG_OK;
            return;
      }
      
      this->mhandler.pop_back();
      this->LastMsg = HANDLER_MSG_OK;
}

bool ListHandler::Exist(const std::string& key)
{
     return (std::find(this->mhandler.begin(), this->mhandler.end(), key) != this->mhandler.end());
}

void ListHandler::Add(const std::string& key)
{
      if (key.empty() || key == "")
      {
           this->LastMsg = HANDLER_MSG_INVALID;
           return;
      }
      
      this->mhandler.push_back(key);
      this->LastMsg = HANDLER_MSG_OK;
}

void ListHandler::Remove(const std::string& key)
{
      if (key.empty() || key == "")
      {
           this->LastMsg = HANDLER_MSG_INVALID;
           return;
      }

      if (this->mhandler.size() == 1)
      {
            this->Erase();
            this->LastMsg = HANDLER_MSG_OK;
            return;
      }

      this->LastMsg = HANDLER_MSG_OK;
      this->mhandler.remove(key); 
}

std::string ListHandler::Index(unsigned int pos)
{
      if (!this->mhandler.size())
      {
            return 0;
      }
      
      unsigned int counter = 0;
      
      for (ListMap::const_iterator i = this->mhandler.begin(); i != this->mhandler.end(); i++)      
      {
                  std::string item = *i;
                  
                  if (counter == pos)
                  {
                        this->LastMsg = HANDLER_MSG_OK;
                        return item; 
                  }
                  
                  counter++;
      }
      
      this->LastMsg = HANDLER_MSG_NOT_FOUND;
      return "";
}

unsigned int ListHandler::Repeats(const std::string& word)
{
      if (!this->mhandler.size())
      {
            return 0;
      }

      unsigned int counter = 0;

      for (ListMap::const_iterator i = this->mhandler.begin(); i != this->mhandler.end(); i++)
      {
                  std::string item = *i;

                  if (item == word)
                  {
                       counter++;
                  }
      }

      this->LastMsg = HANDLER_MSG_OK;
      return counter;
}

void ListHandler::PopAll(const std::string& key)
{
      if (!this->mhandler.size())
      {
            return;
      }
        
      for (ListMap::const_iterator i = this->mhandler.begin(); i != this->mhandler.end(); )      
      {
                  std::string item = *i;
                  
                  if (item == key)
                  {
                        this->mhandler.erase(i++);
                        continue;
                  }
                  
                  i++;
      }
      
      if (!this->mhandler.size())
      {
            this->Erase();
      }

      this->LastMsg = HANDLER_MSG_OK;
}

std::string ListHandler::as_string()
{
        std::string final;
        
        unsigned int size = this->mhandler.size();
        
        unsigned int counter = 0;
        
        for (ListMap::const_iterator i = this->mhandler.begin(); i != this->mhandler.end(); ++i)      
        {
                  std::string key = *i;
                  
                  if (counter != size)
                  {
                        final += to_bin(key) + ":";
                  }
                  
                  counter++;
        }
        
        return final;
}

void ListHandler::Dump()
{
        for (ListMap::const_iterator i = this->mhandler.begin(); i != this->mhandler.end(); ++i)      
        {
                  std::string key = *i;
                  std::cout << "key =>   " << key << std::endl;
        }
}

std::vector<std::string> ListHandler::Find(const std::string& key)
{
        std::vector<std::string> items;
        
        for (ListMap::const_iterator i = this->mhandler.begin(); i != this->mhandler.end(); ++i)      
        {
                std::string item = *i;
                
                if (!Daemon::Match(item, key))    
                {
                        continue;
                }
                
                items.push_back(item);
        }
  
        this->LastMsg = HANDLER_MSG_OK;
        return items;
}

std::string ListHandler::RPOP()
{
      if (!this->mhandler.size())
      {
            this->LastMsg = HANDLER_MSG_NOT_FOUND;
            return "";
      }

      std::string item = this->mhandler.back();
      this->mhandler.pop_back();
      this->LastMsg = HANDLER_MSG_OK;
      return item;
}
