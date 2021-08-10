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
#include "brldb/map_handler.h"

MapHandler::MapHandler()
{

}

MapHandler::~MapHandler()
{
     this->Erase();
}

bool MapHandler::Exists(const std::string& word)
{
      if (!this->mhandler.size())
      {
            this->LastMsg = HANDLER_MSG_NOT_FOUND;
            return false;
      }

      for (MapMap::const_iterator i = this->mhandler.begin(); i != this->mhandler.end(); i++)
      {
                  std::string item = i->first;

                  if (word == item)
                  {
                       this->LastMsg = HANDLER_MSG_OK;
                       return true;
                  }
      }

      this->LastMsg = HANDLER_MSG_NOT_FOUND;
      return false;
}

bool MapHandler::Delete(const std::string& key)
{
      if (!this->mhandler.size())
      {
            return 0;
      }

      for (MapMap::const_iterator i = this->mhandler.begin(); i != this->mhandler.end(); )
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

std::shared_ptr<MapHandler> MapHandler::Create(const std::string& load)
{
        std::shared_ptr<MapHandler> New = std::make_shared<MapHandler>();

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

void MapHandler::Add(const std::string& key, const std::string& value)
{
      if (key.empty() || key == "")
      {
           this->LastMsg = HANDLER_MSG_INVALID;
           return;
      }
      
      this->mhandler.erase(key); 
      this->mhandler.insert(std::make_pair(key, value));
      this->LastMsg = HANDLER_MSG_OK;
}

std::string MapHandler::Get(const std::string& key)
{
      for (MapMap::const_iterator i = this->mhandler.begin(); i != this->mhandler.end(); )
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

unsigned int MapHandler::Strlen(const std::string& key)
{
      return this->Get(key).length();
}

void MapHandler::Remove(const std::string& key)
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

std::string MapHandler::as_string()
{
        std::string final;
        
        unsigned int size = this->mhandler.size();
        
        unsigned int counter = 0;
        
        for (MapMap::const_iterator i = this->mhandler.begin(); i != this->mhandler.end(); ++i)      
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

void MapHandler::Dump()
{
        for (MapMap::const_iterator i = this->mhandler.begin(); i != this->mhandler.end(); ++i)
        {
                  std::string key = i->first;
                  std::string value = i->second;
                  std::cout << "key =>  " << key <<  " value: "  << value << std::endl;
        }
}

void MapHandler::Erase()
{
       this->mhandler.clear();
       this->LastMsg = HANDLER_MSG_OK;

}

DualMap MapHandler::GetAll()
{
      DualMap result;

      for (MapMap::const_iterator i = this->mhandler.begin(); i != this->mhandler.end(); i++)
      {
                  std::string key = i->first;
                  std::string value = i->second;
                  
                  result.insert(std::make_pair(key, value));
      }
      
      this->LastMsg = HANDLER_MSG_OK;
      return result;
}

std::vector<std::string> MapHandler::GetValues()
{
      Args result;

      for (MapMap::const_iterator i = this->mhandler.begin(); i != this->mhandler.end(); i++)
      {
                  std::string val = i->second;
                  result.push_back(val);
      }
      
      this->LastMsg = HANDLER_MSG_OK;
      return result;
}
