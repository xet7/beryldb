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
    TYPE_SCONCAT = 17,
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

class Externalize query_base
{
    public:
        
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
       
        std::string int_keys;
        
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
        
        DBL_CODE type;
        
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
        
        query_base(DBL_CODE base) : Lock(false), started(0), core(false), 
                                    subresult(0), partial(false), int_keys(INT_KEYS), qtype(TYPE_NONE), 
                                    offset(0), limit(0), quiet(false), finished(false), onlyfirst(false), user(NULL), 
                                    type(base), operation(OP_NONE), counter(0), counter2(0), data(0), size(0.0), exists(false), from(0), to(0)
        {
        
        }
        
        query_base()
        {
        
        }
        
        bool Check();
        
        virtual void Run() = 0;

};

class Externalize lfind_query : public query_base
{
    public:

        lfind_query() : query_base(DBL_TYPE_LFIND)
        {

        }

        void Run();
};


class Externalize search_query : public query_base
{
    public:

        search_query() : query_base(DBL_TYPE_KSEARCH)
        {

        }

        void Run();
};

class Externalize hset_query : public query_base
{
    public:
        
        hset_query() : query_base(DBL_TYPE_HSET)
        {
        
        }
        
        void Run();
};

class Externalize hdel_query : public query_base
{
    public:

        hdel_query() : query_base(DBL_TYPE_HDEL)
        {

        }

        void Run();
};

class Externalize hget_query : public query_base
{
    public:

        hget_query() : query_base(DBL_TYPE_HGET)
        {

        }

        void Run();
};

class Externalize lmove_query : public query_base
{
    public:

        lmove_query() : query_base(DBL_TYPE_LMOVE)
        {

        }

        void Run();
};

class Externalize lpush_query : public query_base
{
    public:	
    
        lpush_query() : query_base(DBL_TYPE_LPUSH)
        {

        }

        void Run();
};

class Externalize lget_query : public query_base
{
    public:

        lget_query() : query_base(DBL_TYPE_LGET)
        {

        }

        void Run();
};

class Externalize lpop_query : public query_base
{
    public:

        lpop_query() : query_base(DBL_TYPE_LPOP)
        {

        }

        void Run();
};

class Externalize  llist_query : public query_base
{
    public:

        DualMap dmap;

        llist_query() : query_base(DBL_TYPE_LLIST)
        {

        }

        void Run();
};


class Externalize get_query  : public query_base
{
    public:

        get_query() : query_base(DBL_TYPE_GET)
        {
        
        }
        
        void Run();
};

class Externalize set_query  : public query_base
{
    public:
        
        std::string response;

        /* Will not return any result */

        bool quiet;

        set_query() : query_base(DBL_TYPE_SET), quiet(false)
        {

        }

        void Run();


};

class Externalize del_query  : public query_base
{
    public:

        del_query() : query_base(DBL_TYPE_DEL)
        {

        }

        void Run();
};

class Externalize op_query  : public query_base
{
    public:

        op_query() : query_base(DBL_TYPE_OP)
        {

        }

        void Run();
};

class Externalize find_query  : public query_base
{
    public:

        find_query() : query_base(DBL_TYPE_FIND)
        {

        }

        void Run();
};

class Externalize hsearch_query  : public query_base
{
    public:

        hsearch_query() : query_base(DBL_TYPE_SEARCH)
        {

        }

        void Run();
};


class Externalize move_query  : public query_base
{
    public:

        move_query() : query_base(DBL_TYPE_MOVE)
        {

        }

        void Run();
};


class Externalize hdel_all_query  : public query_base
{
    public:

        hdel_all_query() : query_base(DBL_TYPE_HDEL_ALL)
        {

        }

        void Run();
};

class Externalize hsearch_hesh_query  : public query_base
{
    public:

        hsearch_hesh_query() : query_base(DBL_TYPE_HSEARCH)
        {

        }

        void Run();
};


class Externalize hkeys_query  : public query_base
{
    public:

        hkeys_query() : query_base(DBL_TYPE_HKEYS)
        {

        }

        void Run();
};

class Externalize advget_query  : public query_base
{
    public:

        advget_query() : query_base(DBL_TYPE_ADVGET)
        {

        }

        void Run();
};

class Externalize dbsize_query  : public query_base
{
    public:

        dbsize_query() : query_base(DBL_TYPE_DBSIZE)
        {

        }

        void Run();
};

class Externalize hmove_query  : public query_base
{
    public:

        hmove_query() : query_base(DBL_TYPE_HMOVE)
        {

        }

        void Run();
};

class Externalize touch_query  : public query_base
{
    public:

        touch_query() : query_base(DBL_TYPE_TOUCH)
        {

        }

        void Run();
};


class Externalize append_query  : public query_base
{
    public:

        append_query() : query_base(DBL_TYPE_APPEND)
        {

        }

        void Run();
};

class Externalize lsearch_query  : public query_base
{
    public:

        lsearch_query() : query_base(DBL_TYPE_LSEARCH)
        {

        }

        void Run();
};


class Externalize hquery_query  : public query_base
{
    public:

        hquery_query() : query_base(DBL_TYPE_HQUERY)
        {

        }

        void Run();
};


class Externalize sflush_query  : public query_base
{
    public:

        sflush_query() : query_base(DBL_TYPE_SFLUSH)
        {

        }

        void Run();
};

class Flusher
{
    public:
        
        static void Set(User* user, std::shared_ptr<query_base> query);
        
        static void Get(User* user, std::shared_ptr<query_base> query);

        static void Del(User* user, std::shared_ptr<query_base> query);


        static void HSet(User* user, std::shared_ptr<query_base> query);

        static void HGet(User* user, std::shared_ptr<query_base> query);

        static void HDel(User* user, std::shared_ptr<query_base> query);
        
        static void HFind(User* user, std::shared_ptr<query_base> query);
        
        static void LPush(User* user, std::shared_ptr<query_base> query);

        static void LGet(User* user, std::shared_ptr<query_base> query);
        
        static void LPop(User* user, std::shared_ptr<query_base> query);
        
        static void Operation(User* user, std::shared_ptr<query_base> query);
        
        static void Find(User* user, std::shared_ptr<query_base> query);

        static void HSearch(User* user, std::shared_ptr<query_base> query);

        static void HList(User* user, std::shared_ptr<query_base> query);

        static void Move(User* user, std::shared_ptr<query_base> query);

        static void HDelAll(User* user, std::shared_ptr<query_base> query);
        
        static void HSearch_Hesh(User* user, std::shared_ptr<query_base> query);
        
        static void HKeys(User* user, std::shared_ptr<query_base> query);

        static void DBSize(User* user, std::shared_ptr<query_base> query);

        static void AdvancedGET(User* user, std::shared_ptr<query_base> query);

        static void HMove(User* user, std::shared_ptr<query_base> query);

        static void Touch(User* user, std::shared_ptr<query_base> query);

        static void Append(User* user, std::shared_ptr<query_base> query);

        static void LMove(User* user, std::shared_ptr<query_base> query);

        static void SFlush(User* user, std::shared_ptr<query_base> query);
        
        static void Search(User* user, std::shared_ptr<query_base> query);
        
        static void LSearch(User* user, std::shared_ptr<query_base> query);

        static void LFind(User* user, std::shared_ptr<query_base> query);
};