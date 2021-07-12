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

class ExportAPI DiffHandler
{
      public:
          
         /* Constructor */
          
         DiffHandler();  

        /* 
         * Compares two lists.
         * 
         * @parameters:
	 *
	 *         · list1: List one to compare.
	 *         · list2: List two to compare.
	 * 
         * @return:
 	 * 
         *         · ListMap: A List containing all differences.
         */    
                   
         static ListMap CompareList(const ListMap list, const ListMap list2);

        /* 
         * Compares two vectors.
         * 
         * @parameters:
	 *
	 *         · list: Vector1.
	 *         · list2: Vector2.
	 * 
         * @return:
 	 *
         *         · Args: A result containing differences.
         */    
         
         static Args CompareVector(const Args& list, const Args& list2);
         
         static Args CompareMultiMap(const MultiMap list, const MultiMap list2);

        /* 
         * Compares a map.
         * 
         * @parameters:
	 *
	 *         · list: First map to compare.
	 *         · list2: Second map to compare.
	 * 
         * @return:
 	 *
         *         · Args: Compared list.
         */    
         
         static Args CompareMap(const MapMap list, const MapMap list2);
};