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
#include <algorithm>

#include "beryl.h"
#include "engine.h"
#include "brldb/vector_handler.h"

VectorHandler::VectorHandler()
{

}

std::shared_ptr<VectorHandler> VectorHandler::Create(const std::string& load)
{
        std::shared_ptr<VectorHandler> New = std::make_shared<VectorHandler>();
        
        engine::colon_node_stream stream(load);
        std::string mask;

        while (stream.items_extract(mask))
        {
                New->Add(to_string(mask));
        }
        
        return New;
}

void VectorHandler::PopFront()
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

void VectorHandler::Erase()
{
       this->mhandler.clear();
       this->LastMsg = HANDLER_MSG_OK;
}

void VectorHandler::PopBack()
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

void VectorHandler::Add(const std::string& key)
{
      if (key.empty() || key == "")
      {
           this->LastMsg = HANDLER_MSG_INVALID;
           return;
      }
      
      this->mhandler.push_back(key);
      this->LastMsg = HANDLER_MSG_OK;
}

bool VectorHandler::Exist(const std::string& key)
{
     return (std::find(this->mhandler.begin(), this->mhandler.end(), key) != this->mhandler.end());
}

void VectorHandler::Remove(const std::string& key)
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
      this->mhandler.erase(std::find(this->mhandler.begin(), this->mhandler.end(), key));
}

std::string VectorHandler::Index(unsigned int pos)
{
      if (!this->mhandler.size())
      {
            return 0;
      }
      
      unsigned int counter = 0;
      
      for (StringVector::const_iterator i = this->mhandler.begin(); i != this->mhandler.end(); i++)      
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

bool VectorHandler::IsNumeric()
{
      for (StringVector::const_iterator i = this->mhandler.begin(); i != this->mhandler.end(); i++)
      {
               if (!is_number(*i, true))
               {
                    return false;
               }
      }
      
      return true;
}

double VectorHandler::GetSMA()
{
      double value = 0;
      
      for (StringVector::const_iterator i = this->mhandler.begin(); i != this->mhandler.end(); i++)
      {
             double item = convto_num<double>(*i);
             value += item;
      }
      
      double size = (double)this->mhandler.size();
      return value/size;
}

double VectorHandler::GetHigh()
{
      double value = 0;
      
      for (StringVector::const_iterator i = this->mhandler.begin(); i != this->mhandler.end(); i++)
      {
             double item = convto_num<double>(*i);
             
             if (item > value)
             {
                  value = item;
             }
             
      }

      return value;
}

double VectorHandler::Sum()
{
      double sum = 0;
      
      for (StringVector::const_iterator i = this->mhandler.begin(); i != this->mhandler.end(); i++)
      {
             double item = convto_num<double>(*i);
             sum += item;
      }

      return sum;
}

double VectorHandler::GetLow()
{
      double value = convto_num<double>(mhandler.front());
      
      for (StringVector::const_iterator i = this->mhandler.begin(); i != this->mhandler.end(); i++)
      {
             double item = convto_num<double>(*i);
             
             if (item < value)
             {
                  value = item;
             }
             
      }

      return value;
}

std::string VectorHandler::as_string()
{
        std::string final;
        
        unsigned int size = this->mhandler.size();
        
        unsigned int counter = 0;
        
        for (StringVector::const_iterator i = this->mhandler.begin(); i != this->mhandler.end(); ++i)      
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

void VectorHandler::Dump()
{
        for (StringVector::const_iterator i = this->mhandler.begin(); i != this->mhandler.end(); ++i)      
        {
                  std::string key = *i;
                  std::cout << "key =>   " << key << std::endl;
        }
}

std::vector<std::string> VectorHandler::Find(const std::string& key)
{
        std::vector<std::string> items;
        
        for (StringVector::const_iterator i = this->mhandler.begin(); i != this->mhandler.end(); ++i)      
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

void VectorHandler::Reverse()
{
        std::reverse(this->mhandler.begin(), this->mhandler.end());        
}

unsigned int VectorHandler::Repeats(const std::string& word)
{
      if (!this->mhandler.size())
      {
            return 0;
      }

      unsigned int counter = 0;

      for (StringVector::const_iterator i = this->mhandler.begin(); i != this->mhandler.end(); i++)
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

std::string VectorHandler::Front()
{
      if (!this->mhandler.size())
      {
            this->LastMsg = HANDLER_MSG_NOT_FOUND;
            return "";
      }

      const std::string& item = this->mhandler.front();
      this->LastMsg = HANDLER_MSG_OK;
      return item;
}

std::string VectorHandler::Back()
{
      if (!this->mhandler.size())
      {
            this->LastMsg = HANDLER_MSG_NOT_FOUND;
            return "";
      }

      const std::string& item = this->mhandler.back();
      this->LastMsg = HANDLER_MSG_OK;
      return item;
}
