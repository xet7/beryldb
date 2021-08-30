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

#include "beryl.h"
#include "brldb/list_handler.h"
#include "brldb/database.h"
#include "brldb/datathread.h"
#include "brldb/dbnumeric.h"
#include "brldb/dbflush.h"
#include "brldb/query.h"
#include "helpers.h"

void DataFlush::BatchFailed(User* user, std::shared_ptr<QueryBase> signal)
{
      user->SendProtocol(ERR_INPUT, BATCH_FAIL);
}

void DataFlush::InvalidRange(User* user, std::shared_ptr<QueryBase> signal)
{
      user->SendProtocol(ERR_INPUT, INVALID_RANGE);
}

void DataFlush::InvalidFormat(User* user, std::shared_ptr<QueryBase> signal)
{
      user->SendProtocol(ERR_INPUT, INVALID_FORMAT);
}

void DataFlush::EntryExists(User* user, std::shared_ptr<QueryBase> signal)
{
       user->SendProtocol(ERR_INPUT, ENTRY_DEFINED);
}

void DataFlush::EntryNOExpires(User* user, std::shared_ptr<QueryBase> signal)
{
       user->SendProtocol(ERR_INPUT, ENTRY_NOT_EXPIRING);
}

void DataFlush::NotFound(User* user, std::shared_ptr<QueryBase> signal)
{
        user->SendProtocol(ERR_INPUT, PROCESS_NULL);
}

void DataFlush::EntryExpires(User* user, std::shared_ptr<QueryBase> signal)
{
        user->SendProtocol(ERR_INPUT, ENTRY_EXPIRES);
}

void DataFlush::StatusFailed(User* user, std::shared_ptr<QueryBase> signal)
{
        user->SendProtocol(ERR_INPUT, DB_NULL);
}

void DataFlush::MissStringVector(User* user, std::shared_ptr<QueryBase> signal)
{
        user->SendProtocol(ERR_INPUT, MIS_ARGS);
}

void DataFlush::InvalidType(User* user,  std::shared_ptr<QueryBase> signal)
{
       std::string reply = INVALID_FORMAT + ":" + Helpers::TypeString(signal->response);
       user->SendProtocol(ERR_INPUT, reply);
}

void DataFlush::UnableWrite(User* user, std::shared_ptr<QueryBase> signal)
{
     user->SendProtocol(ERR_INPUT, PROCESS_FALSE);
}

void DataFlush::Flush(User* user, std::shared_ptr<QueryBase> signal)
{
        signal->Process();
        signal = NULL;
}

void DataFlush::NotNumeric(User* user, std::shared_ptr<QueryBase> signal)
{
     user->SendProtocol(ERR_INPUT, PROCESS_NOT_NUM);
}
