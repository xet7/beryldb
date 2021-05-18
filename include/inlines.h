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

inline void stdhelpers::discardrun::operator()(base_class* item)
{
        if (item)
        {
                Kernel->Reducer.Add(item);
        }
}

inline void Channel::Write(ProtocolTrigger::EventProvider& protoevprov, ProtocolTrigger::Message& msg, char status, const DiscardList& except_list, User* joining)
{
        ProtocolTrigger::Event event(protoevprov, msg);
        Write(event, status, except_list, joining);
}

inline void LocalUser::Send(ProtocolTrigger::EventProvider& protoevprov, ProtocolTrigger::Message& msg)
{
        ProtocolTrigger::Event event(protoevprov, msg);
        Send(event);
}
