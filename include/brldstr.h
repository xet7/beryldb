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

#include <cstdarg>
#include <cstring>

#include "config.h"

#define SCHEME(scheme, last, format) \
	do { \
	va_list vlist; \
	va_start(vlist, last); \
	scheme.assign(Daemon::Format(vlist, format)); \
	va_end(vlist); \
	} while (false);


ExportAPI std::string bin_to_hex(const void* raw, size_t rawsize);

ExportAPI std::string bin_to_base64(const std::string& data, const char* table = NULL, char pad = 0);

ExportAPI std::string base_64_to_bin(const std::string& data, const char* table = NULL);

inline std::string bin_to_hex(const std::string& data)
{
	return bin_to_hex(data.data(), data.size());
}
