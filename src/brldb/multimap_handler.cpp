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

#include <iostream>

#include "beryl.h"
#include "engine.h"
#include "brldb/multimap_handler.h"

MultiMapHandler::MultiMapHandler()
{

}

MultiMapHandler::~MultiMapHandler()
{
     this->Erase();
}

bool MultiMapHandler::Exists(const std::string& word)
{
      if (!this->mhandler.size())
      {
            return 0;
      }

      for (MultiMap::const_iterator i = this->mhandler.begin(); i != this->mhandler.end(); i++)
      {
                  std::string item = i->first;

                  if (item == word)
                  {
                       this->LastMsg = HANDLER_MSG_OK;
                       return true;
                  }
      }

      this->LastMsg = HANDLER_MSG_NOT_FOUND;
      return false;
}

bool MultiMapHandler::Delete(const std::string& key)
{
      if (!this->mhandler.size())
      {
            return 0;
      }

      for (MultiMap::const_iterator i = this->mhandler.begin(); i != this->mhandler.end(); )
      {
                  std::string item = i->first;

                  if (item == key)
                  {
                        this->mhandler.erase(i++);
                        this->LastMsg = HANDLER_MSG_OK;
                        return true;
                  }

                  i++;
      }

      this->LastMsg = HANDLER_MSG_NOT_FOUND;
      return false;
}

std::shared_ptr<MultiMapHandler> MultiMapHandler::Create(const std::string& load)
{
        std::shared_ptr<MultiMapHandler> New = std::make_shared<MultiMapHandler>();

        engine::colon_node_stream stream(load);
        std::string mask;

        while (stream.items_extract(mask))
        {
              size_t found =  mask.find_first_of("/");
              std::string path = mask.substr(0,found);
              std::string file = mask.substr(found+1);
              New->Add(to_string(path), to_string(file));
        }

        return New;
}

void MultiMapHandler::Add(const std::string& key, const std::string& value)
{
      if (key.empty() || key == "")
      {
           this->LastMsg = HANDLER_MSG_INVALID;
           return;
      }
      
      this->mhandler.insert(std::make_pair(key, value));
      this->LastMsg = HANDLER_MSG_OK;
}

std::string MultiMapHandler::Get(const std::string& key)
{
      for (MultiMap::const_iterator i = this->mhandler.begin(); i != this->mhandler.end(); )
      {
                  std::string item = i->first;
                  std::string val = i->second;

                  if (item == key)
                  {
                        this->LastMsg = HANDLER_MSG_OK;
                        return val;
                  }

                  i++;
      }
      
      this->LastMsg = HANDLER_MSG_NOT_FOUND;
      return "";
}

void MultiMapHandler::Remove(const std::string& key)
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
      this->mhandler.erase(key); 
}

std::string MultiMapHandler::as_string()
{
        std::string final;
        
        unsigned int size = this->mhandler.size();
        
        unsigned int counter = 0;
        
        for (MultiMap::const_iterator i = this->mhandler.begin(); i != this->mhandler.end(); ++i)      
        {
                  std::string key = i->first;
                  std::string value = i->second;
                  
                  if (counter != size)
                  {
                        final += to_bin(key) + "/" + to_bin(value) + ":";
                  }
                  
                  counter++;
        }
        
        return final;
}

void MultiMapHandler::Dump()
{
        for (MapMap::const_iterator i = this->mhandler.begin(); i != this->mhandler.end(); ++i)
        {
                  std::string key = i->first;
                  std::string value = i->second;
                  std::cout << "key =>  " << key <<  " value: "  << value << std::endl;
        }
}

void MultiMapHandler::Erase()
{
       this->mhandler.clear();
       this->LastMsg = HANDLER_MSG_OK;

}

unsigned int MultiMapHandler::Repeats(const std::string& key)
{
       unsigned int counter =  this->mhandler.count(key);
       this->LastMsg = HANDLER_MSG_OK;       
       return counter;
}

std::vector<std::string> MultiMapHandler::Find(const std::string& key)
{
      std::vector<std::string> results;

      for (MultiMap::const_iterator i = this->mhandler.begin(); i != this->mhandler.end(); i++)
      {
                  std::string item = i->first;
                  std::string val  = i->second;

                  if (Daemon::Match(val, key))
                  {
                         results.push_back(item);
                  }
      }

      this->LastMsg = HANDLER_MSG_OK;
      return results;
}

std::vector<std::string> MultiMapHandler::GetValues()
{
      StringVector result;

      for (MultiMap::const_iterator i = this->mhandler.begin(); i != this->mhandler.end(); i++)
      {
                  std::string val = i->second;
                  result.push_back(val);
      }

      this->LastMsg = HANDLER_MSG_OK;
      return result;
}

DualMMap MultiMapHandler::GetAll()
{
      DualMMap result;

      for (MultiMap::const_iterator i = this->mhandler.begin(); i != this->mhandler.end(); i++)
      {
                  std::string key = i->first;
                  std::string value = i->second;

                  result.insert(std::make_pair(key, value));
      }

      this->LastMsg = HANDLER_MSG_OK;
      return result;
}

std::vector<std::string> MultiMapHandler::GetVals(const std::string& key)
{
      std::vector<std::string> results;

      for (MultiMap::const_iterator i = this->mhandler.begin(); i != this->mhandler.end(); )
      {
                  std::string item = i->first;
                  std::string val = i->second;

                  if (key == item)
                  {
                      results.push_back(val);
                  }

                  i++;
      }

      this->LastMsg = HANDLER_MSG_OK;
      return results;
}

