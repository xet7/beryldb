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

/* 
 * GetFlags returns flags for a given user.
 * 
 * @parameters:
 *
 *         · string   :  user to find flags for.
 * 
 * @protocol:
 *
 *         · enum     : NULL, or ERROR.
 *         · string   : flags.
 */

class CommandGetFlags : public Command
{
 public:

        CommandGetFlags(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Adds a new user to the user manager.
 * 
 * @parameters:
 *
 *         · string   :  Login to use, must have at least 3 characters.
 *         · string   :  Password to use, valid strings must have between 3 and
 *                       30 chars.
 * 
 * @protocol:
 *
 *         · protocol : NULL, ERROR or OK.
 */

class CommandAddUser : public Command
{
 public:

        CommandAddUser(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Removes an user from the server.
 * 
 * @parameters:
 *
 *         · string   :  Login to remove.
 * 
 * @protocol:
 *
 *         · protocol : NULL, ERROR or OK.
 */

class CommandDelUser : public Command
{
 public:

        CommandDelUser(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Adds a flag for a given user.
 *
 * @parameters:
 *
 *         ·  string	:   Login to add a flag to.
 *         ·  char      :   New flag, either 'm', 'e' or 'r'. 
 *
 * @protocol:
 *
 *         · enum       :   NULL, INVALID or OK.
 */

class CommandAddFlag : public Command
{
 public:

        CommandAddFlag(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Deletes a flag for a given user.
 *
 * @parameters:
 *
 *         ·  string    :   Login to remove a flag from.
 *         ·  char      :   New flag, either 'm', 'e' or 'r'. 
 *
 * @protocol:
 *
 *         · enum      :   NULL, INVALID or OK.
 */
 
class CommandDelFlags : public Command
{
 public:

        CommandDelFlags(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Lists all users in the server.
 * 
 * @requires 'r'.
 *
 * @protocol:
 *
 *         · vector    : user list.
 */

class CommandListUsers : public Command
{
 public:

        CommandListUsers(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * List flags for a given user.
 * 
 * @parameters:
 *
 *         · string    : Destination user.
 * 
 * @protocol:
 *
 *         · vector    : List of flags.
 */ 

class CommandListFlags : public Command
{
 public:

        CommandListFlags(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Changes password for current user. If 3 arguments are provided, this command
 * will change the password of another user.
 * 
 * @parameters:
 *
*	   · string   : user.
 *         · string   : password.
 * 
 * @protocol:
 *
 *         · enum: OK, ERROR OR NOT_FOUND.
 */ 

class CommandPasswd : public Command
{
 public:

        CommandPasswd(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Lists all admins in the server.
 * 
 * @requires 'r'.
 *
 * @protocol:
 *
 *         · vector : admin list.
 */

class CommandListAdmins : public Command
{
 public:

        CommandListAdmins(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Checks whether a given user is in cache.
 *
 * @parameters:
 *
 *         · string   :   Login to verify if is present in cache.
 *
 * @protocol:
 *
 *         · enum     :   1 or 0.
 */
 
class CommandInCache : public Command
{
 public:

        CommandInCache(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Resets all login in the local cache.
 *
 * @protocol:
 *
 *         · enum      :   OK.
 */

class CommandResetCache : public Command
{
 public:

        CommandResetCache(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

