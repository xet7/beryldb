%target Makefile
#
# BerylDB - A modular database.
# http://www.beryldb.com
#
# Copyright (C) 2015-2021 Carlos F. Ferry <cferry@beryldb.com>
# 
# This file is part of BerylDB. BerylDB is free software: you can
# redistribute it and/or modify it under the terms of the BSD License
# version 3.
#
# More information about our licensing can be found at https://docs.beryl.dev

BERYLDB_VERBOSE=1
CXX = @CXX@
COMPILER = @COMPILER_NAME@
SYSTEM = @SYSTEM_NAME@
BUILDPATH ?= $(dir $(realpath $(firstword $(MAKEFILE_LIST))))/build/@COMPILER_NAME@-@COMPILER_VERSION@
CORECXXFLAGS = -fPIC -fvisibility=hidden -fvisibility-inlines-hidden -pipe -Iinclude -Wall -Wextra -Wfatal-errors -Wno-unused-parameter -Wshadow -Wno-switch -std=c++14
LDLIBS = -lstdc++ -lrocksdb -lpthread
CORELDFLAGS = -rdynamic -L.
PICLDFLAGS = -fPIC -shared -rdynamic

DESTDIR := $(if $(DESTDIR),$(DESTDIR),"@DESTDIR|@")
BINPATH = "$(DESTDIR)@BINARY_DIR@"
CONPATH = "$(DESTDIR)@CONFIG_DIR@"
DATPATH = "$(DESTDIR)@DATA_DIR@"
DBPATH =  "$(DESTDIR)@DB_DIR@"
EXAPATH = "$(DESTDIR)@EXAMPLE_DIR@"
LOGPATH = "$(DESTDIR)@LOG_DIR@"
MANPATH = "$(DESTDIR)@MANUAL_DIR@"
MODPATH = "$(DESTDIR)@MODULE_DIR@"
SCRPATH = "$(DESTDIR)@SCRIPT_DIR@"

INSTALL ?= install
INSTMODE_DIR ?= 0755
INSTMODE_BIN ?= 0755
INSTMODE_TXT ?= 0644
INSTMODE_PRV ?= 0640
LDLIBS += -ldl -lrt

ifndef BERYLDB_DEBUG
  BERYLDB_DEBUG=0
endif

DBGOK=0
ifeq ($(BERYLDB_DEBUG), 0)
  CORECXXFLAGS +=  -O3
ifeq ($(COMPILER), GCC)
    CORECXXFLAGS += -g1
endif
  HEADER = std-header
  DBGOK=1
endif
ifeq ($(BERYLDB_DEBUG), 1)
  CORECXXFLAGS += -O0 -g3 -Werror
  HEADER = debug-header
  DBGOK=1
endif
ifeq ($(BERYLDB_DEBUG), 2)
  CORECXXFLAGS += -O2 -g3
  HEADER = debug-header
  DBGOK=1
endif
ifeq ($(BERYLDB_DEBUG), 3)
  CORECXXFLAGS += -O0 -g0 -Werror
  HEADER = std-header
  DBGOK=1
endif

MAKEFLAGS += --no-print-directory

SOURCEPATH = $(shell pwd)

ifndef BERYLDB_VERBOSE
  MAKEFLAGS += --silent
endif


CORECXXFLAGS += $(CPPFLAGS) $(CXXFLAGS)
CORELDFLAGS += $(LDFLAGS)
PICLDFLAGS += $(LDFLAGS)

export BUILDPATH
export CORECXXFLAGS
export CORELDFLAGS
export CXX
export BERYLDB_VERBOSE
export LDLIBS
export PICLDFLAGS
export SOURCEPATH

TARGET = all

ifdef BERYLDB_TARGET
    HEADER = mod-header
    TARGET = $(BERYLDB_TARGET)
endif

ifeq ($(DBGOK), 0)
  HEADER = unknown-debug-level
endif

all: finishmessage

target: $(HEADER)
	$(MAKEENV) perl make/checkenv.pl
	cd "$(BUILDPATH)"; $(MAKEENV) $(MAKE) -f real.mk $(TARGET)

debug:
	@${MAKE} BERYLDB_DEBUG=1 all

debug-header:
	@echo " "
	@echo "· Building with debug symbols   "
	@echo "				       "

mod-header:
	@echo 'Building specific targets:'

std-header:
	@echo " "
	@echo "· Building BerylDB.          "
	@echo " "

finishmessage: target
	@echo " "
	@echo "* BerylDB compiled."
	@echo "* To install BerylDB, type: 'make install'"

install: target
	@-$(INSTALL) -d -g @GID@ -o @UID@ -m $(INSTMODE_DIR) $(BINPATH)
	@-$(INSTALL) -d -g @GID@ -o @UID@ -m $(INSTMODE_DIR) $(CONPATH)
	@-$(INSTALL) -d -g @GID@ -o @UID@ -m $(INSTMODE_DIR) $(DATPATH)
	@-$(INSTALL) -d -g @GID@ -o @UID@ -m $(INSTMODE_DIR) $(DBPATH)
	@-$(INSTALL) -d -g @GID@ -o @UID@ -m $(INSTMODE_DIR) $(LOGPATH)
	@-$(INSTALL) -d -g @GID@ -o @UID@ -m $(INSTMODE_DIR) $(MANPATH)
	@-$(INSTALL) -d -g @GID@ -o @UID@ -m $(INSTMODE_DIR) $(MODPATH)
	@-$(INSTALL) -d -g @GID@ -o @UID@ -m $(INSTMODE_DIR) $(SCRPATH)
	-$(INSTALL) -g @GID@ -o @UID@ -m $(INSTMODE_BIN) "$(BUILDPATH)/bin/beryldb" $(BINPATH)
	-$(INSTALL) -g @GID@ -o @UID@ -m $(INSTMODE_BIN) "$(BUILDPATH)/modules/"*.so $(MODPATH)
	-$(INSTALL) -g @GID@ -o @UID@ -m $(INSTMODE_BIN) @CONFIGURE_DIRECTORY@/beryldb $(SCRPATH) 2>/dev/null
	-$(INSTALL) -g @GID@ -o @UID@ -m $(INSTMODE_TXT) @CONFIGURE_DIRECTORY@/beryldb.1 $(MANPATH) 2>/dev/null
	-$(INSTALL) -g @GID@ -o @UID@ -m $(INSTMODE_TXT) docs/conf/*.example $(EXAPATH)
	@echo ""
	@echo "· BerylDB is now installed."
	@echo ""
	@echo '· Paths:'
	@echo ' '
	@echo '    Configuration:' $(CONPATH)
	@echo '    Binaries:' $(BINPATH)
	@echo '    Modules:' $(MODPATH)
	@echo '    DB:' $(DBPATH)
	@echo '    Data:' $(DATPATH)
	@echo ""
	@echo 'You need to create a configuration file:' $(CONPATH)/beryldb.conf
	@echo 'Feel free to check our config examples:' $(EXAPATH)
	@echo "" 	
	@echo '· To start the server, run:'  $(SCRPATH)/beryldb start
	@echo ""

Makefile: make/template/main.mk src/version.sh configure @CONF_CACHEFILE@
	./configure --update

clean:
	@echo "Cleaning           "
	-rm -f "$(BUILDPATH)/bin/beryldb" "$(BUILDPATH)/include" "$(BUILDPATH)/real.mk"
	-rm -rf "$(BUILDPATH)/obj" "$(BUILDPATH)/modules"
	-rm -f $(BINPATH)/beryldb
	-rm -rf "$(BUILDPATH)" 2>/dev/null
	-rm -rf "$(BUILDPATH)" ".configure"
	-rm -f $(MODPATH)/m_*.so
	-rm -f $(MODPATH)/core_*.so
	-rm -rf build
	-rm -f Makefile
	-rm -f include/config.h
	-rm -f include/constants.h
	-rm -f core
	rm -rf @CONFIGURE_DIRECTORY@
	@echo Completed.

deinstall:
	-rm -f $(BINPATH)/beryldb
	-rm -rf $(EXAPATH)
	-rm -f $(MANPATH)/beryldb.1
	-rm -f $(MODPATH)/m_*.so
	-rm -f $(MODPATH)/core_*.so

configureclean:
	-rm -f Makefile
	rm -f include/config.h
	rm -f include/constants.h
	rm -rf @CONFIGURE_DIRECTORY@

distclean: clean configureclean
	-rm -rf "$(SOURCEPATH)/run"
	find "$(SOURCEPATH)/src/modules" -type l | xargs rm -f

help:
	@echo 'BERYLDB Makefile'
	@echo ''
	@echo 'Use: ${MAKE} [flags] [targets]'
	@echo ''
	@echo 'Settings:'
	@echo ' '
	@echo ' BERYLDB_VERBOSE=1  Displays full command compiling.'
	@echo ' BERYLDB_DEBUG=1    Enable debugging, for module development.'
	@echo ' BERYLDB_DEBUG=2    Build with optimizations, for detailed traces.'
	@echo ' BERYLDB_DEBUG=3    Fast build without improvements. '
	@echo ' DESTDIR=           Specifies destination root.'
	@echo ' -j <N>             Parallel building.'
	@echo ''
	@echo 'Targets:'
	@echo ' all       Complete build of BerylDB.'
	@echo ' install   Build and install BerylDB.'
	@echo ' debug     Compile a debug build. '
	@echo ''
	@echo ' BERYLDB_TARGET=target  Builds a user-specified target, such as	"beryldb" or "core_destroy"'
	@echo '                         Multiple targets may be separated by a space'
	@echo ''
	@echo ' clean     Cleans object files.'
	@echo ' distclean Cleans all generated files.'
	@echo ' deinstall Removes the files generated by "make install"'
	@echo

.NOTPARALLEL:

.PHONY: all target debug debug-header mod-header mod-footer std-header finishmessage install clean deinstall configureclean help
