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

class ExportAPI GroupManager : public safecast<GroupManager>
{
     private:
        
          GroupMap GroupList;
        
     public:
         
         /* Constructor */
                  
         GroupManager();

         /* Destructor */
                  
         ~GroupManager();
         
         bool Add(const std::string& name, const std::string& flags);
         
         std::shared_ptr<Group> Find(const std::string& name);

         bool Delete(const std::string& name);

        /* 
         * Resets flags for this group.
         * 
         * @parameters:
	 *
	 *         · name: Name of group to reset.
	 * 
         * @return:
 	 *
         *         · True: Flags updated.
         *         · False: Unable to update.
         */    
         
         bool ResetFlags(const std::string& name);
         
         GroupMap& GetList()
         {
              return this->GroupList;
         }
         
};

class ExportAPI Group
{
     friend class GroupManager;
     
     private:
 
         std::string name;
         std::string rawflags;
         
     public:
 
         /* Constructor */
    
         Group();

         /* Destructor */
    
         ~Group();

         bool UpdateFlags(const std::string& flags);
         
         void Init();

         void SetFlags(const std::string& flags);
         
         bool CanDo(unsigned char flag);
          
         bool can_do_all;
         
         bool can_keys;
         
         bool can_list;
         
         bool can_geo;
         
         bool can_vector;
         
         bool can_expire;
         
         bool can_future;
         
         bool can_map;
         
         bool can_mmap;
         
         bool can_publish_chan;
         
         bool can_publish_clients;
         
         bool can_hints;
         
         bool can_dual_exec;

        /* 
         * Returns rawflags.
         * 
         * @return:
 	 *
         *         · string: flags.
         */    
         
         std::string GetFlags()
         {
              return this->rawflags;
         }
         
         std::string GetName()
         {
              return this->name;
         }
};


