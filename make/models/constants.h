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

/* Entry does not expires */

const std::string ENTRY_NOT_EXPIRING =  "ENTRY_NOT_EXPIRING";

const std::string NO_CHANS = "NO_CHANS";

/* Not logged. */

const std::string NO_AUTH = "NOT_LOGGED";

/* not found. */

const std::string NOT_FOUND = "NOT_FOUND";

/* No login provided */

const std::string NO_LOG_PROV = "NO_LOG_PROVIDED";

/* Wrong login */

const std::string ERRON_LOGIN = "ERROR_LOGIN";

/* User is already logged. */

const std::string ALREADY_LOGGED = "ALREADY_LOGGED";

/* Entry expires */

const std::string ENTRY_EXPIRES = "ENTRY_EXPIRES";

/* Invalid range. */

const std::string INVALID_RANGE = "INVALID_RANGE";

/* Already using */

const std::string PROCESS_ALREADY = "ALREADY_USING";

/* Database is busy. */

const std::string DATABASE_BUSY = "DATABASE_BUSY";

/* Entry is defined. */

const std::string ENTRY_DEFINED = "ENTRY_DEFINED";

/* Invalid format. */

const std::string INVALID_FORMAT = "INVALID_TYPE";

/* Database is null */

const std::string DB_NULL = "DATABASE_NULL";

/* Missing arguments */

const std::string MIS_ARGS = "MISSING_ARGUMENTS";

/* Invalid type. */

const std::string INVALID_TYPE = "INVALID";

/* Invalid coordinate */

const std::string INVALID_COORD = "Invalid coordinate.";

/* An error has occured. */

const std::string PROCESS_ERROR = "ERROR";

/* Entry exists. */

const std::string PROCESS_FALSE = "FALSE";

/* Invalid monitor level */

const std::string INVALID_MLEVEL = "Invalid monitor level.";

/* Invalid notification level */

const std::string INVALID_NLEVEL = "Invalid notification level.";

/* Not numeric. */

const std::string PROCESS_NOT_NUM = "NOT_NUMERIC";

/* Item not found. */

const std::string PROCESS_NULL = "NULL";

/* Everything OK */

const std::string PROCESS_OK = "OK";

/* Unable to set key. */

const std::string UNABLE_SET_KEY = "Unable to set key.";

/* > 0 */

const std::string MUST_BE_GREAT_ZERO = "Number must be greater than zero.";

/* Command not found. */

const std::string CMD_NOT_FOUND = "Command not found.";

/* Unknown issue happened. */

const std::string UNKNOWN_ISSUE	= "A problem has occured while performing this operation.";

/* Incorrect agent format. */

const std::string WRONG_AGENT = "Your agent is not valid.";

/* Valid agent. */

const std::string VALID_AGENT = "INVALID_AGENT";

/* Default ping interval. */

const int PING_INTVL = 10;

/* Default publish command. */

#define PUBL_DEF "PUBLISH"

/* Key not found. */

const std::string NOT_FOUND_KEY = "Key not found.";

/* Unable to remove key. */

const std::string UNABLE_KEY = "Unable to remove key";

/* Used when a password is changed. */

const std::string PASS_CHANGED = "Password changed.";

/* Used when an user is changing a pass to one with less than 3 chars */

const std::string PASS_AT_LEAST = "Password must be of at least 3 chars.";

/* Invalid user name. */

const std::string INVALID_UNAME = "Invalid login.";

/* Password indication. */

const std::string PASS_MIMMAX_LENGTH = "Min password length is 3, max is 100.";

/* Used when indicating that an user length must be inside this range. */

const std::string USER_MIMMAX_LENGTH = "Min user length is 3, max is 15.";

/* Setting variable provided has not been found. */

const std::string SETTING_NOT_FOUND = "Setting variable not found.";

/* Invalid parameters have been provided. */

const std::string INVALID_PARAM = "Invalid parameters.";

/* 
 * Access Denied. Used when an user tries to
 * run a command when he/she has no required flags.
 */
 
const std::string ACCESS_DENIED = "Access Denied.";

/* 
 * Unable to find a map. Usually called when trying to
 * delete items in a non-existant map.
 */
 
const std::string UNABLE_MAP = "Unable to find map.";

/* No items were found. */

const std::string UNABLE_ITEMS = "Unable to find items.";

/* real msg. */

const std::string MUST_REAL_MSG = "Must be a real number.";

/* These definitions must go in core_expire. */

const std::string MUST_BE_NUMERIC = "MUST_BE_NUMERIC";

/* The user most likely provided a negative number. */

const std::string MUST_BE_POSIT = "Value provided must be a positive number.";

/* Unable to define an expiring key. */

const std::string UNABLE_EXPIRE = "Unable to set expire.";

/* Internal registry. */

const std::string INT_REG = "9";


/* Keys definition */

const std::string INT_KEY = "1";

/* Map definition. */

const std::string INT_MAP = "2";

/* Lists. */

const std::string INT_LIST = "3";

/* GEO functions */ 

const std::string INT_GEO = "4";

/* Map definition. */

const std::string INT_MMAP = "5";

/* Vector definition. */

const std::string INT_VECTOR = "6";

/* Expires definition. */

const std::string INT_EXPIRE = "8";

/* Futures */

const std::string INT_FUTURE = "9";

/* Table expires. */

const std::string TABLE_EXPIRES = "expires";

/* Auto join table. */
 
const std::string TABLE_AUTOJOIN = "autojoin";

/* Default exiting msg */

const std::string SERVER_EXITING = "Server Exiting.";

/* settings links */

const std::string SETTINGS_LINKS = "links";

/* Settings table. */

const std::string TABLE_SETTINGS = "settings";

/* Table for databases. */

const std::string TABLE_DBS = "database";

/* Table for admin entries */

const std::string TABLE_ADMIN = "admins";

/* Table for users entries. */

const std::string TABLE_USERS = "users";

/* Key removed." */

const std::string KEY_REMOVED = "Key removed.";

/* A problem occured while obtaining key. */

const std::string UNABLE_GET_KEY = "Unable to retrieve key.";

/* Registry types. */

const std::vector<std::string> TypeRegs = 
{ 
    INT_LIST, 
    INT_KEY, 
    INT_GEO, 
    INT_MAP, 
    INT_MMAP, 
    INT_VECTOR, 
    INT_EXPIRE, 
    INT_FUTURE 
};

%target include/constants.h
