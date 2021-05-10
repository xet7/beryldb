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

#pragma once

/* Incorrect agent format. */

#define WRONG_AGENT "Your agent is not valid."

/* Valid agent. */

#define VALID_AGENT "Agent must be in the 3 to 15 length range."

/* Undefined key. */

#define UNDEF_KEY "Undefined key."

/* Default ping interval. */

#define PING_INTVL 20

/* Default publish command. */

#define PUBL_DEF "PUBLISH"

/* Key not found. */

#define NOT_FOUND_KEY "Key not found."

/* Unable to remove key. */

#define UNABLE_KEY "Unable to remove key"

/* Used when a password is changed. */

#define PASS_CHANGED "Password changed."

/* Used when an user is changing a pass to one with less than 3 chars */

#define PASS_AT_LEAST "Password must be of at least 3 chars."

/* Invalid user name. */

#define INVALID_UNAME "Invalid login."

/* Password indication. */

#define PASS_MIMMAX_LENGTH "Min password length is 3, max is 100."

/* Used when indicating that an user length must be inside this range. */

#define USER_MIMMAX_LENGTH "Min user length is 3, max is 15."

/* Setting variable provided has not been found. */

#define SETTING_NOT_FOUND "Setting variable not found."

/* Invalid parameters have been provided. */

#define INVALID_PARAM "Invalid parameters."

/* 
 * Access Denied. Used when an user tries to
 * run a command when he/she has no required flags.
 */
 
#define ACCESS_DENIED "Access Denied."

/* 
 * Unable to find a map. Usually called when trying to
 * delete items in a non-existant map.
 */
 
#define UNABLE_MAP "Unable to find map."

/* No items were found. */

#define UNABLE_ITEMS "Unable to find items."

/* real msg. */

#define MUST_REAL_MSG "Must be a real number."

/* These definitions must go in core_expire. */

#define MUST_BE_NUMERIC "Value provided must be numeric."

/* The user most likely provided a negative number. */

#define MUST_BE_POSIT "Value provided must be a positive number."

/* Unable to define an expiring key. */

#define UNABLE_EXPIRE "Unable to set expire."

/* Keys definition */

#define INT_KEYS "1"

/* Map definition. */

#define INT_MAP "2"

/* Lists. */

#define INT_LIST "3"

/* Table expires. */

#define TABLE_EXPIRES "expires"

/* Auto join table. */
 
#define TABLE_AUTOJOIN "autojoin"

/* Default exiting msg */

#define SERVER_EXITING "Server Exiting."

/* settings links */

#define SETTINGS_LINKS "links"

/* Settings table. */

#define TABLE_SETTINGS "settings"

/* Table for databases. */

#define TABLE_DBS "database"

/* Table for admin entries */

#define TABLE_ADMIN "admins"

/* Table for users entries. */

#define TABLE_USERS "users"

/* Key removed." */

#define KEY_REMOVED "Key removed."

/* A problem occured while obtaining key. */

#define UNABLE_GET_KEY "Unable to retrieve key."

%target include/constants.h
