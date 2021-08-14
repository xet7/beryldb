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

static std::string chansync = "chansync";

/* Default publish command. */

#define PUBL_DEF "PUBLISH"

/* No diff. */

const std::string NO_DIFF		=	"NO_DIFF";

/* WriteBatch failed. */

const std::string BATCH_FAIL 		= 	"SYNC_FAILED";

/* Invalid channel */

const std::string INVALID_CHAN 		= 	"INVALID_CHAN";

/* User is not subscribed */

const std::string NOT_SUBSCRIBED 	= 	"NOT_SUBSCRIBED";

/* No syntax defined. */

const std::string NO_SYNTAX 		= 	"NO_SYNTAX";

/* Target not defined */

const std::string NOT_DEFINED 		= 	"NOT_DEFINED";

/* External messages not allowed. */

const std::string NO_EXTERNAL 		= 	"NO_EXTERNAL";

/* Empty result */

const std::string PROCESS_EMPTY 	= 	"EMPTY";

/* Default group flags */

const std::string DEFAULT_GFLAGS 	= 	"klgvefmxphw";

/* Entry is protected */

const std::string ENTRY_PROTECTED 	= 	"ENTRY_PROTECTED";

/* Already paused */

const std::string ALREADY_PAUSED 	= 	"ENTRY_PAUSED";

/* Entry does not expires */

const std::string ENTRY_NOT_EXPIRING   =  	"ENTRY_NOT_EXPIRING";

/* No chans */

const std::string NO_CHANS 		= 	"NO_CHANS";

/* Not logged. */

const std::string NO_AUTH 		= 	"NOT_LOGGED";

/* not found. */

const std::string NOT_FOUND 		= 	"NOT_FOUND";

/* No login provided */

const std::string NO_LOG_PROV 		= 	"NO_LOG_PROVIDED";

/* Wrong login */

const std::string ERRON_LOGIN 		= 	"ERROR_LOGIN";

/* User is already logged. */

const std::string ALREADY_LOGGED 	= 	"ALREADY_LOGGED";

/* Entry expires */

const std::string ENTRY_EXPIRES 	= 	"ENTRY_EXPIRES";

/* Invalid range. */

const std::string INVALID_RANGE 	= 	"INVALID_RANGE";

/* Already using */

const std::string PROCESS_ALREADY 	= 	"ALREADY_USING";

/* Database is busy. */

const std::string DATABASE_BUSY 	= 	"DATABASE_BUSY";

/* Entry is defined. */

const std::string ENTRY_DEFINED 	= 	"ENTRY_DEFINED";

/* Invalid format. */

const std::string INVALID_FORMAT 	= 	"INVALID_TYPE";

/* Invalid value */

const std::string INVALID_VALUE 	= 	"INVALID_VALUE";

/* Invalid key */

const std::string INVALID_KEY  		= 	"INVALID_KEY";

/* Database is null */

const std::string DB_NULL 		= 	"DATABASE_NULL";

/* Missing arguments */

const std::string MIS_ARGS 		= 	"MISSING_ARGUMENTS";

/* Invalid type. */

const std::string INVALID_TYPE 		= 	"INVALID";

/* Invalid coordinate */

const std::string INVALID_COORD 	= 	"INVALID_COORDINATE";

/* An error has occured. */

const std::string PROCESS_ERROR 	= 	"ERROR";

/* Entry exists. */

const std::string PROCESS_FALSE 	= 	"FALSE";

/* Invalid monitor level */

const std::string INVALID_MLEVEL 	= 	"INVALID_MONITOR";

/* Invalid notification level */

const std::string INVALID_NLEVEL 	= 	"INVALID_LEVEL";

/* Not numeric. */

const std::string PROCESS_NOT_NUM 	= 	"NOT_NUMERIC";

/* Item not found. */

const std::string PROCESS_NULL 		= 	"NULL";

/* Everything OK */

const std::string PROCESS_OK 		= 	"OK";

/* > 0 */

const std::string MUST_BE_GREAT_ZERO   = 	"GREATER_THAN_ZERO";

/* Incorrect agent format. */

const std::string WRONG_AGENT 		= 	"INVALID_INPUT";

/* Valid agent. */

const std::string VALID_AGENT 		= 	"INVALID_AGENT";

/* Default ping interval. */

const int PING_INTVL 			= 	10;

/* Used when an user is changing a pass to one with less than 3 chars */

const std::string PASS_AT_LEAST 	= 	"MUST_BE_AT_LEAST_3_LENGTH";

/* Invalid user name. */

const std::string INVALID_UNAME 	= 	"INVALID_LOGIN";

/* Password indication. */

const std::string PASS_MIMMAX_LENGTH 	= 	"EXCEED_RANGE";

/* Used when indicating that an user length must be inside this range. */

const std::string USER_MIMMAX_LENGTH 	= 	"EXCEED_RANGE";

/* Invalid parameters have been provided. */

const std::string INVALID_PARAM 	= 	"INVALID_PARAMS";

/* 
 * Access Denied. Used when an user tries to
 * run a command when he/she has no required flags.
 */
 
const std::string ACCESS_DENIED 	= 	"ACCESS_DENIED";

/* Invoked when a given number is not numeric. */

const std::string MUST_BE_NUMERIC 	= 	"MUST_BE_NUMERIC";

/* Internal registry. */

const std::string INT_REG 		= 	"9";

/* Keys definition */

const std::string INT_KEY 		= 	"1";

/* Map definition. */

const std::string INT_MAP 		= 	"2";

/* Lists. */

const std::string INT_LIST 		= 	"3";

/* GEO functions */ 

const std::string INT_GEO 		= 	"4";

/* Map definition. */

const std::string INT_MMAP 		= 	"5";

/* Vector definition. */

const std::string INT_VECTOR 		= 	"6";

/* Expires definition. */

const std::string INT_EXPIRE 		= 	"8";

/* Futures */

const std::string INT_FUTURE 		=	 "9";

/* Default exiting msg */

const std::string SERVER_EXITING 	= 	"SERVER_EXITING";

/* Must be positive int. */

const std::string MUST_BE_POSIT		=	"MUST_BE_POSIT";

%target include/constants.h
