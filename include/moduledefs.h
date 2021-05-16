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

#pragma once

class Module;

#define MODULE_ABI 1UL

#define MODULE_STRINGIFY_SYM1(DEF) MODULE_STRINGIFY_SYM2(DEF)
#define MODULE_STRINGIFY_SYM2(DEF) #DEF

#define MODULE_SYM_ABI BERYL_MODULE_ABI
#define MODULE_STR_ABI MODULE_STRINGIFY_SYM1(MODULE_SYM_ABI)

#define MODULE_SYM_INIT BERYL_MODULE_LOAD
#define MODULE_STR_INIT MODULE_STRINGIFY_SYM1(MODULE_SYM_INIT)

#define MODULE_SYM_VERSION beryl_module_version
#define MODULE_STR_VERSION MODULE_STRINGIFY_SYM1(MODULE_SYM_VERSION)

#define MODULE_LOAD(klass) \
	extern "C" ExternalModule const unsigned long MODULE_SYM_ABI = MODULE_ABI; \
	extern "C" ExternalModule const char MODULE_SYM_VERSION[] = VERSION; \
	extern "C" ExternalModule Module* MODULE_SYM_INIT() { return new klass; }
