/*
 * BerylDB - A modular database.
 * http://www.beryldb.com
 *
 * Copyright (C) 2021 Carlos F. Ferry <cferry@beryldb.com>
 * 
 * This file is part of BerylDB. BerylDB is free software: you can
 * redistribute it and/or modify it under the terms of the BSD License
 * version 3.
 *
 * More information about our licensing can be found at https://docs.beryl.dev
 */

#include "beryl.h"

static const char hex_table[] = "0123456789abcdef";

std::string bin_to_hex(const void* raw, size_t l)
{
	const char* data = static_cast<const char*>(raw);
	std::string rv;
	rv.reserve(l * 2);

	for (size_t i = 0; i < l; i++)
	{
		unsigned char c = data[i];
		rv.push_back(hex_table[c >> 4]);
		rv.push_back(hex_table[c & 0xF]);
	}
	
	return rv;
}

static const char base_64_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

std::string bin_to_base64(const std::string& data_str, const char* table, char pad)
{
	if (!table)
	{
		table = base_64_table;
	}

	uint32_t buffer;
	uint8_t* data = (uint8_t*)data_str.data();
	std::string rv;
	size_t i = 0;
	
	while (i + 2 < data_str.length())
	{
		buffer = (data[i] << 16 | data[i+1] << 8 | data[i+2]);
		rv.push_back(table[0x3F & (buffer >> 18)]);
		rv.push_back(table[0x3F & (buffer >> 12)]);
		rv.push_back(table[0x3F & (buffer >>  6)]);
		rv.push_back(table[0x3F & (buffer >>  0)]);
		i += 3;
	}
	
	if (data_str.length() == i)
	{
	
	}
	else if (data_str.length() == i + 1)
	{
		buffer = data[i] << 16;
		rv.push_back(table[0x3F & (buffer >> 18)]);
		rv.push_back(table[0x3F & (buffer >> 12)]);
	
		if (pad)
		{
			rv.push_back(pad);
			rv.push_back(pad);
		}
	}
	else if (data_str.length() == i + 2)
	{
		buffer = (data[i] << 16 | data[i+1] << 8);
		rv.push_back(table[0x3F & (buffer >> 18)]);
		rv.push_back(table[0x3F & (buffer >> 12)]);
		rv.push_back(table[0x3F & (buffer >>  6)]);
	
		if (pad)
		{
			rv.push_back(pad);
		}
	}
	
	return rv;
}

std::string base_64_to_bin(const std::string& data_str, const char* table)
{
	if (!table)
	{
		table = base_64_table;
	}

	int bitcount = 0;
	uint32_t buffer = 0;
	const char* data = data_str.c_str();
	std::string rv;
	
	while (true)
	{
		const char* find = strchr(table, *data++);
		
		if (!find || find >= table + 64)
		{
			break;
		}
		
		buffer = (buffer << 6) | (find - table);
		bitcount += 6;
		
		if (bitcount >= 8)
		{
			bitcount -= 8;
			rv.push_back((buffer >> bitcount) & 0xFF);
		}
	}
	
	return rv;
}


