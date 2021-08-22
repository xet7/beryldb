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
#include "brldb/database.h"
#include "cstruct.h"

enum STR_FUNCTION
{
       STR_TO_LOW 	         =	1,
       STR_TO_UPPER		 = 	2,
       STR_TO_CAP		 =      3
};

enum QUERY_TYPE
{
       QUERY_TYPE_RENAME         = 	1,
       QUERY_TYPE_COPY 	         = 	2,
       QUERY_TYPE_TYPE           = 	3,
       QUERY_TYPE_DELETE         = 	4,
       QUERY_TYPE_ITER           = 	5,
       QUERY_TYPE_WRITE          = 	6,
       QUERY_TYPE_TEST           = 	7,
       QUERY_TYPE_DBSIZE         = 	8,
       QUERY_TYPE_OP             = 	10,
       QUERY_TYPE_READ           = 	11,
       QUERY_TYPE_EXPIRE         = 	12,
       QUERY_TYPE_EXISTS         = 	13,
       QUERY_TYPE_SKIP 		 = 	14,
       QUERY_TYPE_SETEX 	 = 	15,
       QUERY_TYPE_MOVE 		 = 	16,
       QUERY_TYPE_CLONE          = 	17,
       QUERY_TYPE_FUTURE         = 	18,
       QUERY_TYPE_FUTURE_RUN     = 	19,
       QUERY_TYPE_READ_ALLOW     = 	20,
       QUERY_TYPE_RENAMENX       = 	21,
       QUERY_TYPE_DIFF           = 	22,
       QUERY_TYPE_LAT		 =	23,
       QUERY_TYPE_LONG		 = 	24,
       QUERY_TYPE_TRANSFER	 =      25,
       QUERY_TYPE_SORT		=	26
};

enum QUERY_FLAGS
{
       QUERY_FLAGS_NONE 	 = 	0,
       QUERY_FLAGS_QUIET 	 = 	1,
       QUERY_FLAGS_CORE 	 = 	2,
       QUERY_FLAGS_COUNT 	 = 	9,
       QUERY_FLAGS_GLOBAL        =      10
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
       OP_MULT	=   7,
       OP_SQRT	=   8
};

class ExportAPI QueryBase
{
    private:

        /* True if this query has finished. */

        bool finished;
        
        /* Used to indicate wether a key is required or not. */

        bool key_required;

        RocksData mapped;
        
    public:
    
        void Required(bool flag)
        {
              this->key_required = flag;
        }
        
        STR_FUNCTION function;
        
        QUERY_FLAGS flags;
        
        QUERY_TYPE type;
        
        DBL_CODE access;
        
        std::string identified;
        
        std::string dest;
        
        std::string base_request;
        
        unsigned int subresult;
        
        bool partial;
        
        std::string hesh;
            
        std::string newkey;
        
        std::string key;
        
        std::string value;
       
        std::string length;
        
        signed int offset;
        
        signed int limit;
        
        DualMMap mlist;
        
        unsigned int select_query;

        unsigned int id;

        /* Only first element */
        
        bool onlyfirst;
        
        /* q_ stands for query */
        
        std::string response;
        
        bool all;

        Args VecData;
        
        std::multimap<std::string, std::string>  mmap;
        
        std::map<std::string, unsigned int> nmap;
        
        std::shared_ptr<Database> database;

        std::shared_ptr<Database> transf_db;
        
        std::vector<std::string> list;

        User* user;
        
        OP_TYPE operation;
        
        unsigned int counter;
        
        signed int data;
        
        double size;

        void access_set(DBL_CODE status)
        {
            this->access = status;
        }
        
        /* Query finished OK */
        
        void SetOK()
        {
            this->finished  = true;
        }
        
        /* Returns this->finished */
        
        bool GetStatus()
        {
             return this->finished;
        }
        
        QueryBase() :  finished(false), key_required(false), flags(QUERY_FLAGS_NONE), access(DBL_NONE),  
                        subresult(0), partial(false), offset(0), limit(0), user(NULL), 
                        operation(OP_NONE), counter(0), data(0), size(0.0)
        {
              
        }
        
        bool Check();
        
        bool CheckKey();

        void DelFuture();
        
        void DelExpire();	

        /* 
         * Checks whether a given key is expiring.
         * 
         * @return:
 	 *
         *         · uint: Seconds.
         */    
         
        signed int IsExpiring();

        /* 
         * Writes and adds an expire to the database.
         * 
         * @parameters:
	 *
	 *         · Expire information, including expiring time.
         */    
                 
        void WriteExpire(const std::string& e_key, unsigned int select, unsigned int ttl, std::shared_ptr<Database> db = NULL);

        bool Swap(const std::string& newdest, const std::string& dest, const std::string& lvalue, std::shared_ptr<Database> db = NULL);

        bool SwapWithExpire(const std::string& newdest, const std::string& ldest, const std::string& lvalue, unsigned int select, const std::string& key, unsigned int ttl, const std::string& oldkey);

        void ExpireBatch(const std::string& wdest, const std::string& value, const std::string& e_key, unsigned int select, unsigned int ttl);
        
        void WriteFuture(const std::string& e_key, unsigned int select, unsigned int ttl, const std::string& value);

        bool Prepare();
        
        bool GetRegistry(unsigned int select, const std::string& regkey, bool do_load = false);
        
        void SetDest(const std::string& regkey, unsigned int regselect, const std::string& regtype);
        
        int CheckDest(unsigned int select, const std::string& regkey,  const std::string& ltype, std::shared_ptr<Database> db = NULL);

        RocksData Get(const std::string& where);
        
        /* 
         * Writes an entry to the database.
         * 
         * @parameters:
	 *
	 *         · dest: identifier.
	 *         · value: Value to store.
         */          
           
        bool Write(const std::string& wdest, const std::string& lvalue);        
        
        void Delete(const std::string& wdest);
        
        virtual void Run() = 0;
        
        virtual void Process() = 0;
        
        virtual ~QueryBase()
        {
        
        }
};


class ExportAPI routed_query : public QueryBase
{
   public:
   
        routed_query(QUERY_TYPE ttype) : QueryBase()
        {
                 this->type = ttype;
                 
        }

        virtual void Keys() = 0;
        
        virtual void Maps() = 0;
        
        virtual void Geos() = 0;
        
        virtual void Multis() = 0;
        
        virtual void Vectors() = 0;

};

class ExportAPI sort_query : public routed_query
{
   public:

        sort_query() : routed_query(QUERY_TYPE_SORT)
        {

        }

        void Keys();

        void Maps();

        void Geos();

        void Multis();

        void Lists();

        void Vectors();

        void Run();

        void Process();
};


class ExportAPI rename_query : public routed_query
{
   public:

        rename_query() : routed_query(QUERY_TYPE_RENAME)
        {

        }

        void Keys();

        void Maps();

        void Geos();
        
        void Multis();
        
        void Lists();
        
        void Vectors();
        
        void Run();
        
        void Process();
};

class ExportAPI renamenx_query : public routed_query
{
   public:

        renamenx_query() : routed_query(QUERY_TYPE_RENAMENX)
        {

        }

        void Keys();

        void Maps();

        void Geos();
        
        void Multis();
        
        void Lists();

        void Vectors();

        void Run();

        void Process();
};

class ExportAPI diff_query : public routed_query
{
   public:

        diff_query() : routed_query(QUERY_TYPE_DIFF)
        {

        }

        void Keys();

        void Maps();

        void Geos();

        void Multis();

        void Lists();

        void Vectors();

        void Run();

        void Process();
};


class ExportAPI del_query : public routed_query
{
    public:

        del_query() : routed_query(QUERY_TYPE_DELETE)
        {
        
        }

        void Keys();

        void Maps();

        void Geos();

        void Multis();
        
        void Lists();

        void Vectors();

        void Run();

        void Process();
};

class ExportAPI transfer_query : public routed_query
{
    public:

        transfer_query() : routed_query(QUERY_TYPE_TRANSFER)
        {

        }

        void Keys();

        void Maps();

        void Geos();

        void Multis();

        void Lists();

        void Vectors();

        void Run();

        void Process();
};

class ExportAPI move_query : public routed_query
{
    public:

        move_query() : routed_query(QUERY_TYPE_MOVE)
        {

        }

        void Keys();

        void Maps();

        void Geos();

        void Multis();

        void Lists();

        void Vectors();

        void Run();

        void Process();
};


class ExportAPI clone_query : public routed_query
{
   public:

        clone_query() : routed_query(QUERY_TYPE_CLONE)
        {

        }

        void Keys();

        void Maps();

        void Geos();

        void Multis();

        void Lists();

        void Vectors();

        void Run();

        void Process();
};


class ExportAPI copy_query : public routed_query
{
   public:

        copy_query() : routed_query(QUERY_TYPE_COPY)
        {

        }

        void Keys();

        void Maps();

        void Geos();

        void Multis();

        void Lists();

        void Vectors();

        void Run();

        void Process();
};

class ExportAPI exists_query : public routed_query
{
   public:

        exists_query() : routed_query(QUERY_TYPE_EXISTS)
        {

        }

        void Keys();

        void Maps();

        void Geos();

        void Multis();
        
        void Lists();

        void Vectors();

        void Run();

        void Process();
};


class ExportAPI hstrlen_query  : public QueryBase
{
    public:

        hstrlen_query() 
        {
                this->type = QUERY_TYPE_READ;
                this->base_request = INT_MAP;
        }

        void Run();

        void Process();
};


class ExportAPI type_query  : public QueryBase
{
    public:

        type_query() 
        {
                this->type = QUERY_TYPE_TYPE;
        }
        
        void Run();
        
        void Process();
};

class ExportAPI total_query  : public QueryBase
{
    public:

        total_query() 
        {
                this->type = QUERY_TYPE_SKIP;
        }

        void Run();

        void Process();
};

class ExportAPI glist_query  : public QueryBase
{
    public:

        glist_query() 
        {
                this->type = QUERY_TYPE_SKIP;
        }

        void Run();

        void Process();
};

class ExportAPI list_query  : public QueryBase
{
    public:

        list_query() 
        {
                this->type = QUERY_TYPE_SKIP;
        }

        void Run();

        void Process();
};

class ExportAPI test_dump_query  : public QueryBase
{
    public:

        test_dump_query() 
        {
                this->type = QUERY_TYPE_SKIP;
        }

        void Run();

        void Process();
};


class ExportAPI getpersist_query  : public QueryBase
{
    public:

        getpersist_query() 
        {
                this->type = QUERY_TYPE_READ;
                this->base_request = INT_KEY;
        }

        void Run();

        void Process();
};


class ExportAPI getdel_query  : public QueryBase
{
    public:

        getdel_query() 
        {
                this->type = QUERY_TYPE_READ;
                this->base_request = INT_KEY;
        }

        void Run();

        void Process();
};

class ExportAPI insert_query  : public QueryBase
{
    public:

        insert_query() 
        {
                this->type = QUERY_TYPE_READ;
                this->base_request = INT_KEY;
        }

        void Run();

        void Process();
};

class ExportAPI char_query  : public QueryBase
{
    public:

        char_query() 
        {
                this->type = QUERY_TYPE_READ;
                this->base_request = INT_KEY;
        }

        void Run();

        void Process();
};

class ExportAPI getset_query  : public QueryBase
{
    public:

        getset_query() 
        {
                this->type = QUERY_TYPE_READ;
                this->base_request = INT_KEY;
        }

        void Run();

        void Process();
};

class ExportAPI touch_query  : public QueryBase
{
    public:

        touch_query() 
        {
                this->type = QUERY_TYPE_SKIP;
        }

        void Run();

        void Process();
};

class ExportAPI ntouch_query  : public QueryBase
{
    public:

        ntouch_query() 
        {
                this->type = QUERY_TYPE_SKIP;
        }

        void Run();

        void Process();
};

class ExportAPI count_query  : public QueryBase
{
    public:

        count_query() 
        {
                this->type = QUERY_TYPE_SKIP;
                this->base_request = INT_KEY;
        }

        void Run();

        void Process();
};

class ExportAPI sflush_query  : public QueryBase
{
    public:

        sflush_query() 
        {
                this->type = QUERY_TYPE_SKIP;
        }

        void Run();

        void Process();
};

class ExportAPI mrepeats_query  : public QueryBase
{
    public:

        mrepeats_query() 
        {
                this->type = QUERY_TYPE_READ;
                this->base_request = INT_MMAP;
        }

        void Run();

        void Process();
};

class ExportAPI msetnx_query  : public QueryBase
{
    public:

        msetnx_query() 
        {
                this->type = QUERY_TYPE_WRITE;
                this->base_request = INT_MMAP;
        }

        void Run();

        void Process();
};

class ExportAPI mset_query  : public QueryBase
{
    public:

        mset_query() 
        {
                this->type = QUERY_TYPE_WRITE;
                this->base_request = INT_MMAP;
        }

        void Run();

        void Process();
};

class ExportAPI hsetnx_query  : public QueryBase
{
    public:

        hsetnx_query() 
        {
                this->type = QUERY_TYPE_READ_ALLOW;
                this->base_request = INT_MAP;
        }

        void Run();

        void Process();
};

class ExportAPI hset_query  : public QueryBase
{
    public:

        hset_query() 
        {
                this->type = QUERY_TYPE_WRITE;
                this->base_request = INT_MAP;
        }

        void Run();

        void Process();
};

class ExportAPI georem_query  : public QueryBase
{
    public:

        georem_query() 
        {
                this->type = QUERY_TYPE_SKIP;
                this->base_request = INT_GEO;
        }

        void Run();

        void Process();
};

class ExportAPI geocalc_query  : public QueryBase
{
    public:

        geocalc_query() 
        {
                this->type = QUERY_TYPE_SKIP;
                this->base_request = INT_GEO;
        }

        void Run();

        void Process();
};

class ExportAPI geoadd_pub_query  : public QueryBase
{
    public:

        geoadd_pub_query() 
        {
                this->type = QUERY_TYPE_WRITE;
                this->base_request = INT_GEO;
        }

        void Run();

        void Process();
};

class ExportAPI geoadd_query  : public QueryBase
{
    public:

        geoadd_query() 
        {
                this->type = QUERY_TYPE_WRITE;
                this->base_request = INT_GEO;
        }

        void Run();

        void Process();
};

class ExportAPI geoaddnx_query  : public QueryBase
{
    public:

        geoaddnx_query() 
        {
                this->type = QUERY_TYPE_WRITE;
                this->base_request = INT_GEO;
        }

        void Run();

        void Process();
};

class ExportAPI alpha_query  : public QueryBase
{
    public:

        alpha_query() 
        {
                this->type = QUERY_TYPE_READ;
                this->base_request = INT_KEY;
        }

        void Run();

        void Process();
};

class ExportAPI geoget_custom_query  : public QueryBase
{
    public:

        geoget_custom_query() 
        {
                this->type = QUERY_TYPE_READ;
                this->base_request = INT_GEO;
        }

        void Run();

        void Process();
};


class ExportAPI geoget_query  : public QueryBase
{
    public:

        geoget_query() 
        {
                this->type = QUERY_TYPE_READ;
                this->base_request = INT_GEO;
        }

        void Run();

        void Process();
};


class ExportAPI lpush_query  : public QueryBase
{
    public:

        lpush_query() 
        {
                this->type = QUERY_TYPE_WRITE;
                this->base_request = INT_LIST;
        }

        void Run();

        void Process();
};

class ExportAPI vpush_query  : public QueryBase
{
    public:

        vpush_query() 
        {
                this->type = QUERY_TYPE_WRITE;
                this->base_request = INT_VECTOR;
        }

        void Run();

        void Process();
};

class ExportAPI vsum_query  : public QueryBase
{
    public:

        vsum_query() 
        {
                this->type = QUERY_TYPE_READ;
                this->base_request = INT_VECTOR;
        }

        void Run();

        void Process();
};

class ExportAPI llow_query  : public QueryBase
{
    public:

        llow_query() 
        {
                this->type = QUERY_TYPE_READ;
                this->base_request = INT_LIST;
        }

        void Run();

        void Process();
};

class ExportAPI lhigh_query  : public QueryBase
{
    public:

        lhigh_query() 
        {
                this->type = QUERY_TYPE_READ;
                this->base_request = INT_LIST;
        }

        void Run();

        void Process();
};

class ExportAPI vlow_query  : public QueryBase
{
    public:

        vlow_query() 
        {
                this->type = QUERY_TYPE_READ;
                this->base_request = INT_VECTOR;
        }

        void Run();

        void Process();
};

class ExportAPI vhigh_query  : public QueryBase
{
    public:

        vhigh_query() 
        {
                this->type = QUERY_TYPE_READ;
                this->base_request = INT_VECTOR;
        }

        void Run();

        void Process();
};

class ExportAPI lpushnx_query  : public QueryBase
{
    public:

        lpushnx_query() 
        {
                this->type = QUERY_TYPE_WRITE;
                this->base_request = INT_LIST;
        }

        void Run();

        void Process();
};

class ExportAPI vpushnx_query  : public QueryBase
{
    public:

        vpushnx_query() 
        {
                this->type = QUERY_TYPE_WRITE;
                this->base_request = INT_VECTOR;
        }

        void Run();

        void Process();
};

class ExportAPI vget_query  : public QueryBase
{
    public:

        vget_query() 
        {
                this->type = QUERY_TYPE_READ;
                this->base_request = INT_VECTOR;
        }

        void Run();

        void Process();
};

class ExportAPI vresize_query  : public QueryBase
{
    public:

        vresize_query() 
        {
                this->type = QUERY_TYPE_READ;
                this->base_request = INT_VECTOR;
        }

        void Run();

        void Process();
};

class ExportAPI lexist_query  : public QueryBase
{
    public:

        lexist_query() 
        {
                this->type = QUERY_TYPE_READ;
                this->base_request = INT_LIST;
        }

        void Run();

        void Process();
};

class ExportAPI vexist_query  : public QueryBase
{
    public:

        vexist_query() 
        {
                this->type = QUERY_TYPE_READ;
                this->base_request = INT_VECTOR;
        }

        void Run();

        void Process();
};

class ExportAPI vrepeats_query  : public QueryBase
{
    public:

        vrepeats_query() 
        {
                this->type = QUERY_TYPE_READ;
                this->base_request = INT_VECTOR;
        }

        void Run();

        void Process();
};


class ExportAPI lrepeats_query  : public QueryBase
{
    public:

        lrepeats_query() 
        {
                this->type = QUERY_TYPE_READ;
                this->base_request = INT_LIST;
        }

        void Run();

        void Process();
};


class ExportAPI lresize_query  : public QueryBase
{
    public:

        lresize_query() 
        {
                this->type = QUERY_TYPE_READ;
                this->base_request = INT_LIST;
        }

        void Run();

        void Process();
};

class ExportAPI vcount_query  : public QueryBase
{
    public:

        vcount_query() 
        {
                this->type = QUERY_TYPE_READ;
                this->base_request = INT_VECTOR;
        }

        void Run();

        void Process();
};

class ExportAPI lcount_query  : public QueryBase
{
    public:

        lcount_query() 
        {
                this->type = QUERY_TYPE_READ;
                this->base_request = INT_LIST;
        }

        void Run();

        void Process();
};

class ExportAPI lrop_query  : public QueryBase
{
    public:

        lrop_query() 
        {
                this->type = QUERY_TYPE_READ;
                this->base_request = INT_LIST;
        }

        void Run();

        void Process();
};

class ExportAPI lrfront_query  : public QueryBase
{
    public:

        lrfront_query() 
        {
                this->type = QUERY_TYPE_READ;
                this->base_request = INT_LIST;
        }

        void Run();

        void Process();
};

class ExportAPI lfront_query  : public QueryBase
{
    public:

        lfront_query() 
        {
                this->type = QUERY_TYPE_READ;
                this->base_request = INT_LIST;
        }

        void Run();

        void Process();
};


class ExportAPI lback_query  : public QueryBase
{
    public:

        lback_query() 
        {
                this->type = QUERY_TYPE_READ;
                this->base_request = INT_LIST;
        }

        void Run();

        void Process();
};

class ExportAPI lget_query  : public QueryBase
{
    public:

        lget_query() 
        {
                this->type = QUERY_TYPE_READ;
                this->base_request = INT_LIST;
        }

        void Run();

        void Process();
};

class ExportAPI vfront_query  : public QueryBase
{
    public:

        vfront_query() 
        {
                this->type = QUERY_TYPE_READ;
                this->base_request = INT_VECTOR;
        }

        void Run();

        void Process();
};

class ExportAPI vback_query  : public QueryBase
{
    public:

        vback_query() 
        {
                this->type = QUERY_TYPE_READ;
                this->base_request = INT_VECTOR;
        }

        void Run();

        void Process();
};

class ExportAPI setnx_query  : public QueryBase
{
    public:

        setnx_query() 
        {
                this->type = QUERY_TYPE_WRITE;
                this->base_request = INT_KEY;
        }

        void Run();

        void Process();
};

class ExportAPI settx_query  : public QueryBase
{
    public:

        settx_query() 
        {
                this->type = QUERY_TYPE_WRITE;
                this->base_request = INT_KEY;
        }

        void Run();

        void Process();
};


class ExportAPI set_query  : public QueryBase
{
    public:

        set_query() 
        {
                this->type = QUERY_TYPE_WRITE;
                this->base_request = INT_KEY;
        }

        void Run();

        void Process();
};

class ExportAPI lpop_front_query  : public QueryBase
{
    public:

        lpop_front_query() 
        {
                this->type = QUERY_TYPE_READ;
                this->base_request = INT_LIST;
        }

        void Run();

        void Process();
};

class ExportAPI lpop_back_query  : public QueryBase
{
    public:

        lpop_back_query() 
        {
                this->type = QUERY_TYPE_READ;
                this->base_request = INT_LIST;
        }

        void Run();

        void Process();
};

class ExportAPI vpop_front_query  : public QueryBase
{
    public:

        vpop_front_query() 
        {
                this->type = QUERY_TYPE_READ;
                this->base_request = INT_VECTOR;
        }

        void Run();

        void Process();
};

class ExportAPI vpop_back_query  : public QueryBase
{
    public:

        vpop_back_query() 
        {
                this->type = QUERY_TYPE_READ;
                this->base_request = INT_VECTOR;
        }

        void Run();

        void Process();
};

class ExportAPI vsort_query  : public QueryBase
{
    public:

        vsort_query() 
        {
                this->type = QUERY_TYPE_READ;
                this->base_request = INT_VECTOR;
        }

        void Run();

        void Process();
};

class ExportAPI lsort_query  : public QueryBase
{
    public:

        lsort_query() 
        {
                this->type = QUERY_TYPE_READ;
                this->base_request = INT_LIST;
        }

        void Run();

        void Process();
};

class ExportAPI random_query  : public QueryBase
{
    public:

        random_query() 
        {
                this->type = QUERY_TYPE_SKIP;
                this->base_request = INT_KEY;
        }

        void Run();

        void Process();
};

class ExportAPI vpos_query  : public QueryBase
{
    public:

        vpos_query() 
        {
                this->type = QUERY_TYPE_READ;
                this->base_request = INT_VECTOR;
        }

        void Run();

        void Process();
};


class ExportAPI lpos_query  : public QueryBase
{
    public:

        lpos_query() 
        {
                this->type = QUERY_TYPE_READ;
                this->base_request = INT_LIST;
        }

        void Run();

        void Process();
};

class ExportAPI lavg_query  : public QueryBase
{
    public:

        lavg_query() 
        {
                this->type = QUERY_TYPE_READ;
                this->base_request = INT_LIST;
        }

        void Run();

        void Process();
};

class ExportAPI vavg_query  : public QueryBase
{
    public:

        vavg_query() 
        {
                this->type = QUERY_TYPE_READ;
                this->base_request = INT_VECTOR;
        }

        void Run();

        void Process();
};

class ExportAPI vreverse_query  : public QueryBase
{
    public:

        vreverse_query() 
        {
                this->type = QUERY_TYPE_READ;
                this->base_request = INT_VECTOR;
        }

        void Run();

        void Process();
};

class ExportAPI lreverse_query  : public QueryBase
{
    public:

        lreverse_query() 
        {
                this->type = QUERY_TYPE_READ;
                this->base_request = INT_LIST;
        }

        void Run();

        void Process();
};

class ExportAPI vkeys_query  : public QueryBase
{
    public:

        vkeys_query() 
        {
                this->type = QUERY_TYPE_SKIP;
                this->base_request = INT_LIST;
        }

        void Run();

        void Process();
};

class ExportAPI lkeys_query  : public QueryBase
{
    public:

        lkeys_query() 
        {
                this->type = QUERY_TYPE_SKIP;
                this->base_request = INT_LIST;
        }

        void Run();

        void Process();
};

class ExportAPI vfind_query  : public QueryBase
{
    public:

        vfind_query() 
        {
                this->type = QUERY_TYPE_ITER;
                this->base_request = INT_VECTOR;
        }

        void Run();

        void Process();
};


class ExportAPI lfind_query  : public QueryBase
{
    public:

        lfind_query() 
        {
                this->type = QUERY_TYPE_ITER;
                this->base_request = INT_LIST;
        }

        void Run();

        void Process();
};

class ExportAPI lpopall_query  : public QueryBase
{
    public:

        lpopall_query() 
        {
                this->type = QUERY_TYPE_ITER;
                this->base_request = INT_LIST;
        }

        void Run();

        void Process();
};

class ExportAPI mdel_query  : public QueryBase
{
    public:

        mdel_query() 
        {
                this->type = QUERY_TYPE_DELETE;
                this->base_request = INT_MMAP;
        }

        void Run();

        void Process();
};

class ExportAPI ldel_query  : public QueryBase
{
    public:

        ldel_query() 
        {
                this->type = QUERY_TYPE_DELETE;
                this->base_request = INT_LIST;
        }

        void Run();

        void Process();
};

class ExportAPI vdel_query  : public QueryBase
{
    public:

        vdel_query() 
        {
                this->type = QUERY_TYPE_DELETE;
                this->base_request = INT_VECTOR;
        }

        void Run();

        void Process();
};

class ExportAPI modify_query  : public QueryBase
{
    public:

        modify_query() 
        {
                this->type = QUERY_TYPE_READ;
                this->base_request = INT_KEY;
        }

        void Run();

        void Process();
};


class ExportAPI hdel_query  : public QueryBase
{
    public:

        hdel_query() 
        {
                this->type = QUERY_TYPE_DELETE;
                this->base_request = INT_MAP;
        }

        void Run();

        void Process();
};

class ExportAPI mget_query  : public QueryBase
{
    public:

        mget_query() 
        {
                this->type = QUERY_TYPE_READ;
                this->base_request = INT_MMAP;
        }

        void Run();

        void Process();
};

class ExportAPI op_query  : public QueryBase
{
    public:

        op_query() 
        {
                this->type = QUERY_TYPE_WRITE;
                this->base_request = INT_KEY;
        }

        void Run();

        void Process();
};

class ExportAPI hget_query  : public QueryBase
{
    public:

        hget_query() 
        {
                this->type = QUERY_TYPE_READ;
                this->base_request = INT_MAP;
        }

        void Run();

        void Process();
};

class ExportAPI get_query  : public QueryBase
{
    public:

        get_query() 
        {
                this->type = QUERY_TYPE_READ;
                this->base_request = INT_KEY;
        }

        void Run();

        void Process();
};

class ExportAPI getexp_query  : public QueryBase
{
    public:

        getexp_query() 
        {
                this->type = QUERY_TYPE_READ;
                this->base_request = INT_KEY;
        }

        void Run();

        void Process();
};
class ExportAPI get_occurs_query  : public QueryBase
{
    public:

        get_occurs_query() 
        {
                this->type = QUERY_TYPE_READ;
                this->base_request = INT_KEY;
        }

        void Run();

        void Process();
};


class ExportAPI get_substr_query  : public QueryBase 
{
    public:

        get_substr_query() 
        {
                this->type = QUERY_TYPE_WRITE;
                this->base_request = INT_KEY;
        }

        void Run();

        void Process();
};

class ExportAPI strlen_query  : public QueryBase
{
    public:

        strlen_query() 
        {
                this->type = QUERY_TYPE_READ;
                this->base_request = INT_KEY;
        }

        void Run();

        void Process();
};

class ExportAPI append_query  : public QueryBase
{
    public:

        append_query() 
        {
                this->type = QUERY_TYPE_WRITE;
                this->base_request = INT_KEY;
        }

        void Run();

        void Process();
};


class ExportAPI hexists_query  : public QueryBase
{
    public:

        hexists_query() 
        {
                this->type = QUERY_TYPE_EXISTS;
                this->base_request = INT_MAP;
        }

        void Run();

        void Process();
};

class ExportAPI geodistance_query : public QueryBase
{
    public:

        geodistance_query() 
        {
                this->type = QUERY_TYPE_SKIP;
                this->base_request = INT_GEO;
        }

        void Run();

        void Process();
};

class ExportAPI future_list_query  : public QueryBase
{
    public:

        future_list_query() 
        {
                this->type = QUERY_TYPE_SKIP;
        }

        void Run();

        void Process();
};


class ExportAPI expire_list_query  : public QueryBase
{
    public:

        expire_list_query() 
        {
                this->type = QUERY_TYPE_SKIP;
        }

        void Run();

        void Process();
};

class ExportAPI setex_query  : public QueryBase
{
    public:

        setex_query() 
        {
                this->type = QUERY_TYPE_WRITE;
                this->base_request = INT_KEY;
        }

        void Run();

        void Process();
};

class ExportAPI future_del_query  : public QueryBase
{
    public:

        future_del_query() 
        {
                this->type = QUERY_TYPE_FUTURE;
                this->base_request = INT_KEY;
        }

        void Run();

        void Process();
};

class ExportAPI future_query  : public QueryBase
{
    public:

        future_query() 
        {
                this->type = QUERY_TYPE_FUTURE;
                this->base_request = INT_KEY;
        }

        void Run();

        void Process();
};

class ExportAPI dbreset_query  : public QueryBase
{
    public:

        dbreset_query() 
        {
                this->type = QUERY_TYPE_SKIP;
        }

        void Run();

        void Process();
};

class ExportAPI expire_query  : public QueryBase
{
    public:

        expire_query() 
        {
                this->type = QUERY_TYPE_EXPIRE;
                this->base_request = INT_KEY;
        }

        void Run();

        void Process();
};

class ExportAPI expireat_query  : public QueryBase
{
    public:

        expireat_query() 
        {
                this->type = QUERY_TYPE_READ;
                this->base_request = INT_KEY;
        }

        void Run();

        void Process();
};

class ExportAPI future_exec_query  : public QueryBase
{
    public:

        future_exec_query() 
        {
                this->type = QUERY_TYPE_FUTURE_RUN;
                this->base_request = INT_KEY;
        }

        void Run();

        void Process();
};

class ExportAPI expire_del_query  : public QueryBase
{
    public:

        expire_del_query() 
        {
                this->type = QUERY_TYPE_READ;
                this->base_request = INT_KEY;
        }

        void Run();

        void Process();
};

class ExportAPI mvals_query  : public QueryBase
{
    public:

        mvals_query() 
        {
                this->type = QUERY_TYPE_READ;
                this->base_request = INT_MMAP;
        }

        void Run();

        void Process();
};


class ExportAPI hvals_query  : public QueryBase
{
    public:

        hvals_query() 
        {
                this->type = QUERY_TYPE_READ;
                this->base_request = INT_MAP;
        }

        void Run();

        void Process();
};

class ExportAPI mgetall_query  : public QueryBase
{
    public:

        mgetall_query() 
        {
                this->type = QUERY_TYPE_READ;
                this->base_request = INT_MMAP;
        }

        void Run();

        void Process();
};

class ExportAPI miter_query  : public QueryBase
{
    public:

        miter_query() 
        {
                this->type = QUERY_TYPE_READ;
                this->base_request = INT_MMAP;
        }

        void Run();

        void Process();
};

class ExportAPI hgetall_query  : public QueryBase
{
    public:

        hgetall_query() 
        {
                this->type = QUERY_TYPE_READ;
                this->base_request = INT_MAP;
        }

        void Run();

        void Process();
};

class ExportAPI hlist_query  : public QueryBase
{
    public:

        hlist_query() 
        {
                this->type = QUERY_TYPE_ITER;
                this->base_request = INT_MAP;
        }

        void Run();

        void Process();
};

class ExportAPI mexists_query  : public QueryBase
{
    public:

        mexists_query() 
        {
                this->type = QUERY_TYPE_ITER;
                this->base_request = INT_MMAP;
        }

        void Run();

        void Process();
};

class ExportAPI mkeys_query  : public QueryBase
{
    public:

        mkeys_query() 
        {
                this->type = QUERY_TYPE_SKIP;
                this->base_request = INT_MMAP;
        }

        void Run();

        void Process();
};

class ExportAPI hwdel_query  : public QueryBase
{
    public:

        hwdel_query() 
        {
                this->type = QUERY_TYPE_ITER;
                this->base_request = INT_MAP;
        }

        void Run();

        void Process();
};

class ExportAPI wdel_query  : public QueryBase
{
    public:

        wdel_query() 
        {
                this->type = QUERY_TYPE_SKIP;
                this->base_request = INT_KEY;
        }

        void Run();

        void Process();
};

class ExportAPI search_query  : public QueryBase
{
    public:

        search_query() 
        {
                this->type = QUERY_TYPE_SKIP;
                this->base_request = INT_KEY;
                
        }

        void Run();

        void Process();
};

class ExportAPI gkeys_query  : public QueryBase
{
    public:

        gkeys_query() 
        {
                this->type = QUERY_TYPE_SKIP;
                this->base_request = INT_GEO;
        }

        void Run();

        void Process();
};

class ExportAPI hfind_query  : public QueryBase
{
    public:

        hfind_query() 
        {
                this->type = QUERY_TYPE_SKIP;
                this->base_request = INT_MAP;
        }

        void Run();

        void Process();
};

class ExportAPI keys_query  : public QueryBase
{
    public:

        keys_query() 
        {
                this->type = QUERY_TYPE_SKIP;
                this->base_request = INT_KEY;
        }

        void Run();

        void Process();
};

class ExportAPI dbsize_query  : public QueryBase
{
    public:

        dbsize_query() 
        {
                this->type = QUERY_TYPE_SKIP;
                this->Required(false);
        }

        void Run();

        void Process();
};

class ExportAPI isnum_query  : public QueryBase
{
    public:

        isnum_query() 
        {
                this->type = QUERY_TYPE_READ;
                this->base_request = INT_KEY;
        }

        void Run();

        void Process();
};

class ExportAPI isbool_query  : public QueryBase
{
    public:

        isbool_query() 
        {
                this->type = QUERY_TYPE_READ;
                this->base_request = INT_KEY;
        }

        void Run();

        void Process();
};

class ExportAPI asbool_query  : public QueryBase
{
    public:

        asbool_query() 
        {
                this->type = QUERY_TYPE_READ;
                this->base_request = INT_KEY;
        }

        void Run();

        void Process();
};

class ExportAPI ismatch_query  : public QueryBase
{
    public:

        ismatch_query() 
        {
                this->type = QUERY_TYPE_READ;
                this->base_request = INT_KEY;
        }

        void Run();

        void Process();
};
