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

#define DONE 1
#define INFO 2
#define ERROR 3
#define WARNING 4
#define IDONE 5
#define IINFO 6
#define IERROR 7
#define IWARNING 8
#define FDONE 9
#define FINFO 10
#define FERROR 11
#define FWARNING 12

#define STR1 std::tr1
#include <tr1/array>
#include <tr1/functional>
#include <tr1/unordered_map>
#include <tr1/type_traits>

#if defined __clang__ || defined __GNUC__
# define BERYL_PRINTF(stringpos, firstpos) __attribute__((format(printf, stringpos, firstpos)))
#else
# define BERYL_PRINTF(stringpos, firstpos)
#endif

#if defined __clang__ || defined __GNUC__
# define DEPRECATED_METHOD(function) function __attribute__((deprecated))
#elif defined _MSC_VER
# define DEPRECATED_METHOD(function) __declspec(deprecated) function
#else
# define DEPRECATED_METHOD(function) function
#endif

#include <unistd.h>

/* Exports core to modules. */

#define Externalize __attribute__ ((visibility ("default")))

/* bprint is short for "beryl print", which prints out to terminal. */

#define bprint Kernel->Engine->printb

/* slog is short for "server log", or simple log. */

#define slog Kernel->Logs->Log

/* Appends a new event to notification list */

#define falert Kernel->Notify->Push

/* Skip alerts */

#define sfalert Kernel->Notify->SPush

/* Allow modularization */

#define ExternalModule __attribute__ ((visibility ("default")))

/* Calls Daemon::print_newline and prints a newline. */

#define do_newline Daemon::print_newline(1)

/* safecast allow classes to be accessed using a -> */

template <typename T> struct safecast
{
        T* operator->()
        {
                return static_cast<T*>(this);
        }
};
