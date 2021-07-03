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

#include "dbnumeric.h"

enum MANAGER_TYPE
{
    MANAGER_TYPE_NONE  = 0,
    MANAGER_TYPE_LIST = 1,
    MANAGER_TYPE_SINGLE = 2
};

enum QUERY_TYPE
{
    TYPE_NONE = 0,
    TYPE_COUNT_RECORDS = 1,
    TYPE_FIND = 2,
    TYPE_EXIST  = 3,
    TYPE_LENGTH = 4,
    TYPE_EXPIRE = 5,
    TYPE_SETEX	= 6,
    TYPE_GETDEL = 7,
    TYPE_GETSET = 8,
    TYPE_GETRANGE  = 9,
    TYPE_RENAME = 10,
    TYPE_COPY = 11,
    TYPE_RAKEY = 12,
    TYPE_EXPIREAT = 13,
    TYPE_NTOUCH = 14,
    TYPE_TCOUNT = 15,
    TYPE_CONCAT =  16,
    TYPE_SETNX	= 18,
    TYPE_SETTX = 19,
    TYPE_LPOS = 20,
    TYPE_RENAMENX = 21
    
};

enum OP_TYPE
{
    OP_NONE 	=   0,
    OP_INCR	=   1,
    OP_DECR	=   2,
    OP_ADD	=   3,
    OP_MIN	=   4,
    OP_AVG	=   5,
    OP_DIV	=   6,
    OP_MULT	=   7
};

class HQuery;

class ExportAPI query_base
{
    public:
        
        bool check;
        
        bool write;
        
        bool iter;
        
        bool remove;
        
        DBL_CODE type;
        
        bool reverted;
        
        bool multi;
        
        bool key_required;
        
        std::string identified;
        
        std::string dest;
        
        std::string base_request;
        
        std::shared_ptr<HQuery> hquery;
        
        MANAGER_TYPE mtype;
        
        std::atomic<bool> Lock;
        
        time_t started;
        
        bool core;
        
        unsigned int subresult;
        
        bool partial;
        
        std::string customreply;

        std::string hesh;
            
        std::string format;
        
        std::string newkey;
        
        std::string key;
        
        std::string value;
       
        std::string length;
        
        QUERY_TYPE qtype;
             
        signed int offset;
        
        signed int limit;
        
        DualMMap mlist;
        
        std::string select_query;
        
        unsigned int id;

        /* Will not return any result */

        bool quiet;

        /* True if this query has finished. */
        
        bool finished;
        
        /* Only first element */
        
        bool onlyfirst;
        
        /* q_ stands for query */
        
        std::string response;
        
        bool all;

        Args q_args;
        
        Args VecData;
        
        std::multimap<std::string, std::string> mmap;
        
        std::shared_ptr<Database> database;
        
        std::vector<std::string> list;

        User* user;
        
        DBL_CODE access;
        
        OP_TYPE operation;
        
        unsigned int counter;
        
        unsigned int counter2;
        
        signed int data;
        
        double size;

        bool exists;
        
        int from;
        
        int to;
        
        void access_set(DBL_CODE status)
        {
            this->access = status;
        }
        
        void SetOK()
        {
            this->finished  = true;
        }
        
        query_base(const std::string& brequest, bool krequired = false, bool writeq = false) : check(false), write(writeq), iter(false), remove(false), reverted(false), multi(false), key_required(krequired), base_request(brequest), hquery(nullptr), mtype(MANAGER_TYPE_NONE), Lock(false), started(0), core(false), 
                                    subresult(0), partial(false), qtype(TYPE_NONE), 
                                    offset(0), limit(0), quiet(false), finished(false), onlyfirst(false), user(NULL), 
                                    operation(OP_NONE), counter(0), counter2(0), data(0), size(0.0), exists(false), from(0), to(0)
        {
        
        }
        
        query_base()
        {
        
        }
        
        bool Check();
        
        bool CheckKey();
        
        void DelExpire();
        
        std::string GetRegistry(const std::string& select, const std::string& key);
        
        void SetRegistry(const std::string& select, const std::string& base, const std::string& key);

        void RemoveRegistry(const std::string& select, const std::string& base, const std::string& key);
        
        virtual void Run() = 0;
        
        virtual void Process() = 0;
        
        virtual ~query_base()
        {
        
        }
};

class ExportAPI get_query  : public query_base
{
    public:

        get_query() : query_base(INT_KEYS, true)
        {
        
        }
        
        void Run();
        
        void Process();
};

class ExportAPI getset_query  : public query_base
{
    public:

        getset_query() : query_base(INT_KEYS, true)
        {

        }

        void Run();

        void Process();
};

class ExportAPI rename_query  : public query_base
{
    public:

        rename_query() : query_base(INT_KEYS, true)
        {
              this->check = true;
        }

        void Run();

        void Process();
};

class ExportAPI copy_query  : public query_base
{
    public:

        copy_query() : query_base(INT_KEYS, true)
        {
              this->check = true;
        }

        void Run();

        void Process();
};

class ExportAPI exists_query  : public query_base
{
    public:

        exists_query() : query_base(INT_KEYS, true)
        {
             this->check = true;
        }

        void Run();

        void Process();
};

class ExportAPI strlen_query  : public query_base
{
    public:

        strlen_query() : query_base(INT_KEYS, true)
        {
            
        }

        void Run();

        void Process();
};

class ExportAPI set_query  : public query_base
{
    public:

        set_query() : query_base(INT_KEYS, true, true)
        {

        }

        void Run();

        void Process();

};

class ExportAPI search_query : public query_base
{
    public:

        search_query() : query_base(INT_KEYS, true)
        {
               this->iter = true;
        }

        void Run();
        
        void Process();
};


class ExportAPI getdel_query  : public query_base
{
    public:

        getdel_query() : query_base(INT_KEYS, true)
        {
                this->check = true;
        }

        void Run();

        void Process();

};

class ExportAPI op_query  : public query_base
{
    public:

        op_query() : query_base(INT_KEYS, true)
        {
                this->type = DBL_TYPE_OP;
        }

        void Run();

        void Process();

};

class ExportAPI hset_query : public query_base
{
    public:
        
        hset_query() : query_base(INT_MAP, true, true)
        {
        
        }
        
        void Run();
        
        void Process();
};

class ExportAPI type_query : public query_base
{
    public:
        
        type_query() : query_base(INT_REG, true)
        {
        
        }
        
        void Run();
        
        void Process();
};

class ExportAPI hget_query : public query_base
{
    public:

        hget_query() : query_base(INT_MAP, true)
        {

        }

        void Run();
        
        void Process();
};

class ExportAPI del_query : public query_base
{
    public:

        del_query() : query_base(INT_KEYS, true)
        {
              this->remove = true;
        }

        void Run();
        
        void Process();
};

class ExportAPI hexists_query : public query_base
{
    public:

        hexists_query() : query_base(INT_MAP, true)
        {
               this->check = true;
        }

        void Run();

        void Process();
};

class ExportAPI count_query : public query_base
{
    public:

        count_query() : query_base(INT_KEYS, true)
        {
               this->iter = true;
        }

        void Run();

        void Process();
};


class ExportAPI hdel_query : public query_base
{
    public:

        hdel_query() : query_base(INT_MAP, true)
        {
              this->remove = true;
        }

        void Run();
        
        void Process();
};

class ExportAPI expire_del_query : public query_base
{
    public:

        expire_del_query() : query_base(INT_EXPIRE, true)
        {

        }

        void Run();
        
        void Process();
};

class ExportAPI test_dump_query : public query_base
{
    public:

        test_dump_query() : query_base("0", false)
        {

        }

        void Run();
        
        void Process();
};

class ExportAPI expire_query : public query_base
{
    public:

        expire_query() : query_base(INT_EXPIRE, true, false)
        {

        }

        void Run();
        
        void Process();
};

class ExportAPI expire_list_query : public query_base
{
    public:

        expire_list_query() : query_base(INT_EXPIRE, false, false)
        {
             this->iter = true;
        }

        void Run();
        
        void Process();
};

class ExportAPI expireat_query : public query_base
{
    public:

        expireat_query() : query_base(INT_EXPIRE, true, false)
        {

        }

        void Run();

        void Process();
};

class ExportAPI find_query : public query_base
{
    public:

        find_query() : query_base(INT_KEYS, true)
        {
               this->iter = true;
        }

        void Run();

        void Process();
};

class ExportAPI hkeys_query : public query_base
{
    public:

        hkeys_query() : query_base(INT_MAP, true)
        {
               this->iter = true;
        }

        void Run();

        void Process();
};

class ExportAPI dbsize_query : public query_base
{
    public:

        dbsize_query() : query_base("0", true)
        {
               this->iter = true;
        }

        void Run();

        void Process();
};




/*
class ExportAPI swapdb_query : public query_base
{
    public:

        swapdb_query() : query_base(DBL_TYPE_SWAPDB)
        {

        }

        void Run();
        
};



class ExportAPI search_query : public query_base
{
    public:

        search_query() : query_base(DBL_TYPE_KSEARCH)
        {

        }

        void Run();
};


class ExportAPI lmove_query : public query_base
{
    public:

        lmove_query() : query_base(DBL_TYPE_LMOVE)
        {

        }

        void Run();
};

class ExportAPI lpush_query : public query_base
{
    public:	
    
        lpush_query() : query_base(DBL_TYPE_LPUSH)
        {

        }

        void Run();
};

class ExportAPI lget_query : public query_base
{
    public:

        lget_query() : query_base(DBL_TYPE_LGET)
        {

        }

        void Run();
};

class ExportAPI lpop_query : public query_base
{
    public:

        lpop_query() : query_base(DBL_TYPE_LPOP)
        {

        }

        void Run();
};

class ExportAPI  llist_query : public query_base
{
    public:

        DualMap dmap;

        llist_query() : query_base(DBL_TYPE_LLIST)
        {

        }

        void Run();
};



class ExportAPI del_query  : public query_base
{
    public:

        del_query() : query_base(DBL_TYPE_DEL)
        {

        }

        void Run();
};


class ExportAPI find_query  : public query_base
{
    public:

        find_query() : query_base(DBL_TYPE_FIND)
        {

        }

        void Run();
};

class ExportAPI hsearch_query  : public query_base
{
    public:

        hsearch_query() : query_base(DBL_TYPE_SEARCH)
        {

        }

        void Run();
};

class ExportAPI geofind_query  : public query_base
{
    public:

        geofind_query() : query_base(DBL_TYPE_GEOFIND)
        {

        }

        void Run();
};


class ExportAPI move_query  : public query_base
{
    public:

        move_query() : query_base(DBL_TYPE_MOVE)
        {

        }

        void Run();
};


class ExportAPI hdel_all_query  : public query_base
{
    public:

        hdel_all_query() : query_base(DBL_TYPE_HDEL_ALL)
        {

        }

        void Run();
};

class ExportAPI hsearch_hesh_query  : public query_base
{
    public:

        hsearch_hesh_query() : query_base(DBL_TYPE_HSEARCH)
        {

        }

        void Run();
};


class ExportAPI hkeys_query  : public query_base
{
    public:

        hkeys_query() : query_base(DBL_TYPE_HKEYS)
        {

        }

        void Run();
};

class ExportAPI advget_query  : public query_base
{
    public:

        advget_query() : query_base(DBL_TYPE_ADVGET)
        {

        }

        void Run();
};

class ExportAPI dbsize_query  : public query_base
{
    public:

        dbsize_query() : query_base(DBL_TYPE_DBSIZE)
        {

        }

        void Run();
};

class ExportAPI hmove_query  : public query_base
{
    public:

        hmove_query() : query_base(DBL_TYPE_HMOVE)
        {

        }

        void Run();
};

class ExportAPI touch_query  : public query_base
{
    public:

        touch_query() : query_base(DBL_TYPE_TOUCH)
        {

        }

        void Run();
};


class ExportAPI append_query  : public query_base
{
    public:

        append_query() : query_base(DBL_TYPE_APPEND)
        {

        }

        void Run();
};

class ExportAPI lsearch_query  : public query_base
{
    public:

        lsearch_query() : query_base(DBL_TYPE_LSEARCH)
        {

        }

        void Run();
};


class ExportAPI hquery_query  : public query_base
{
    public:

        hquery_query() : query_base(DBL_TYPE_HQUERY)
        {

        }

        void Run();
};


class ExportAPI sflush_query  : public query_base
{
    public:

        sflush_query() : query_base(DBL_TYPE_SFLUSH)
        {

        }

        void Run();
};

class ExportAPI geoadd_query : public query_base
{
    public:

        geoadd_query() : query_base(DBL_TYPE_GEOADD)
        {

        }

        void Run();
};

class ExportAPI geoget_query : public query_base
{
    public:

        geoget_query() : query_base(DBL_TYPE_GEOGET)
        {

        }

        void Run();
};

class ExportAPI geodel_query : public query_base
{
    public:

        geodel_query() : query_base(DBL_TYPE_GEODEL)
        {

        }

        void Run();
};

class ExportAPI geocalc_query : public query_base
{
    public:

        geocalc_query() : query_base(DBL_TYPE_GEOCALC)
        {

        }

        void Run();
};

class ExportAPI lremove_query : public query_base
{
    public:

        lremove_query() : query_base(DBL_TYPE_LREMOVE)
        {

        }

        void Run();
};

class ExportAPI geoclose_query : public query_base
{
    public:

        geoclose_query() : query_base(DBL_TYPE_GEOCLOSE)
        {

        }

        void Run();
};

class ExportAPI georemove_query : public query_base
{
    public:

        georemove_query() : query_base(DBL_TYPE_GEOREMOVE)
        {

        }

        void Run();
};



*/