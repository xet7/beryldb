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

#include "beryl.h"
#include "engine.h"

#include "brldb/database.h"

#include "managers/user.h"
#include "managers/maps.h"
#include "managers/settings.h"

class CommandGetFlags : public Command
{
 public:

        CommandGetFlags(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandAddUser : public Command
{
 public:

        CommandAddUser(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandDelUser : public Command
{
 public:

        CommandDelUser(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandAddFlag : public Command
{
 public:

        CommandAddFlag(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* Delete flags in the system. */

class CommandDelFlags : public Command
{
 public:

        CommandDelFlags(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* List all users in the system. */

class CommandListUsers : public Command
{
 public:

        CommandListUsers(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandPasswd : public Command
{
 public:

        CommandPasswd(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandListAdmins : public Command
{
 public:

        CommandListAdmins(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* Checks whether a login is in cache. */

class CommandInCache : public Command
{
 public:

        CommandInCache(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* Resets cache login */

class CommandResetCache : public Command
{
 public:

        CommandResetCache(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandListFlags : public Command
{
 public:

        CommandListFlags(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};
