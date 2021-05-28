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
 * ChannelManager handles all created channels. This class also provides
 * a Find and CheckRemoval functions.
 */
 
class Externalize ChannelManager : public safecast<ChannelManager>
{
    public: 

        /* Map containing all channels. */
        
        ChanMap ChannelList;

        /* Constructor. */
        
        ChannelManager();

        /* Descrutor, clears ChannelLists. */
        
        ~ChannelManager();

        /* 
         * Finds a channel.
         * 
         * @parameters:
	 *
	 *         · chan: Chan name to look for.
	 * 
         * @return:
 	 *
         *         · Channel*: Pointer to channel found. This function
         *		       will return NULL if chan is not found.
         */            
         
	Channel* Find(const std::string &chan);
   
        /* Deletes all channnels */
        
        void Reset();
                
        /* 
         * Determines whether a channel should exist or not. 
         * This function is invoked everytime a client leaves a channel,
         * and is checked against users subscribed. 
         */
        
        void CheckRemoval(Channel* chan);
        
	/* Returns a map of current channels. */
	
        ChanMap& GetSubs() 
        { 
             return ChannelList; 
        }
};