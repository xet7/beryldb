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

class ExportAPI UserHelper
{
    public:
    
        static std::string Find(const std::string& key, const std::string& value);

        /* 
         * Creates a new user.
         * 
         * @parameters:
	 *
	 *         · user: User to add.
	 *         · pass: Password to use.
	 * 
         * @return:
 	 *
         *         · True: User added.
         *         · False: Unable to add user.
         */    
         
        static bool Add(const std::string& user, const std::string& pass);

        static bool Remove(const std::string& user);

        /* 
         * Checks whether a given user is present in the system.
         * 
         * @parameters:
	 *
	 *         · user: User to check.
	 * 
         * @return:
 	 *
         *         · True: User is present.
         *         · False: Unable to locate user.
         */    
         
        static bool Exists(const std::string& user);

        static bool SetFlags(const std::string& user, const std::string& flags);

        static bool DeleteFlags(const std::string& user);

        static bool CheckPass(const std::string& user, const std::string& key);

        static std::string CheckFlags(const std::string& user);

        static bool HasFlags(const std::string& user);


        /* Changes an user password. */
        
        static bool ChangePass(const std::string& user, const std::string& pass);

};