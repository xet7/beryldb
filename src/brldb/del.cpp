/*
 * BerylDB - A modular database.
 * http://www.beryldb.com
 *
 * Copyright (C) 2015-2021 Carlos F. Ferry <cferry@beryldb.com>
 * 
 * This file is part of BerylDB. BerylDB is free software: you can
 * redistribute it and/or modify it under the terms of the BSD License
 * version 3.
 *
 * More information about our licensing can be found at https://docs.beryl.dev
 */

#include "beryl.h"
#include "brldb/database.h"
#include "brldb/query.h"
#include "brldb/dbnumeric.h"

void del_query::Run()
{
    if (!this->Check())
    {
        this->access_set(DBL_STATUS_BROKEN);
        return;
    }

    if (q_args.size() < 1)
    {
            this->access_set(DBL_MISS_ARGS);
            return;
    }

    const std::string& where = q_args[0];

    std::string value;
    rocksdb::Status fstatus2 = this->database->db->Get(rocksdb::ReadOptions(), where, &value);

    if (!fstatus2.ok())
    {
            this->access_set(DBL_NOT_FOUND);
            return;
    }
        
    rocksdb::Status auxstatus = this->database->db->Delete(rocksdb::WriteOptions(), where);
    this->access_set(DBL_STATUS_OK);
    
    this->SetOK();
}

void Flusher::Del(User* user, std::shared_ptr<query_base> query)
{
        /* 
         * Quiet means no result should be shown. The 'quiet' var is
         * mostly used by the expire class, but any developer may use this.
         */
           
        if (query && query->quiet)
        {
                return;
        }
        
        if (query->finished || query->access != 0)
        {
            if (query->access == DBL_NOT_FOUND)
            {
                user->WriteProtocol(ERR_DBQUERY_FAILED, DBL_TYPE_DEL, "Key not found.");
            }
            else
            {
                user->WriteProtocol(BRLD_FLUSH, DBL_TYPE_DEL, "Removed.");
            }
        }
        else
        {
            user->WriteProtocol(ERR_FLUSH, DBL_TYPE_DEL, "Unable to remove key.");
        }
}
