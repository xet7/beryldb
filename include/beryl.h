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

#include <iostream>

#include <csignal>
#include <map>
#include <string>
#include <vector>
#include <memory>

#include "constants.h"
#include "ilist.h"
#include "flat_map.h"
#include "adaptables.h"
#include "aligned_storage.h"
#include "forwards.h"
#include "typedefs.h"
#include "bmaps.h"
#include "converter.h"
#include "stdhelpers.h"

Externalize extern std::unique_ptr<Beryl> Kernel;

#include "config.h"
#include "dynref.h"
#include "colors.h"
#include "reducer.h"
#include "serialize.h"
#include "expandable.h"
#include "filehandler.h"
#include "chandler.h"
#include "protocols.h"
#include "numeric.h"
#include "uid.h"
#include "server.h"
#include "instances.h"
#include "channels.h"
#include "timer.h"
#include "nodes.h"
#include "logs.h"
#include "clientmanager.h"
#include "channelmanager.h"
#include "socket.h"
#include "commandproc.h"
#include "socketstream.h"
#include "message.h"
#include "modules.h"
#include "clientprotocol.h"
#include "configprocess.h"
#include "brldstr.h"
#include "protocol.h"
#include "stats.h"
#include "engine.h"
#include "login.h"

#include "brldb/dbmanager.h"

int main(int argc, char** argv);

/*
 * Class Beryl: This is the main class of the server.
 * This class contains instances to other relevant classes,
 * such as storage, timers, clients and more.
 */
 
class Externalize Beryl
{
  private:
  	
	/* Configuration file. */

	std::string ConfigFile;

	/* Time this BerylDB instance was initialized. */
	
	time_t startup;
	
	/* Keeps track of current time. TIME is updated in the mainloop. */

	struct timespec TIME;
	
	/* A buffer used to read/write pending data. */
	
	char PendingBuffer[BUFFERSIZE];

	/* Major events handler, such as publish, join, etc. */
	
	ProtocolTrigger::BRLDEvents brldevents;

	/* 
	 * Generates a random seed, which is later
	 * used by some random functions. 
	 */
	
	void RandomSeed();
	
	/* Sends BerylDB to the background. */
        
	void Detach();

	/* Starts listening in provided ports. */
	
	void StartListening();
	
	/* Prepares BerylDB for a shut down. */
	        
	void PrepareExit();
	
	/* Handles signals. */
	
	void SignalManager(int signal);
	
	/* Runs every one second. Function meant to dispatch timers. */
	
	void RunTimed(time_t current);
	
	/* This function is called during every cycle. */
	
	void Loop();

	/* Checks if the test office should be run. */

	void CheckOffice();

	/* Tries to listen in X port. */
	
	bool ListenPort(config_rule* tag, const engine::sockets::sockaddrs& sa);

	/* Tries to listen in all ports in config file. */
		
	size_t TryListening(FailedListenings &failed_ports);

        /* CommandLine arguments reader. */

       void CommandLine();
       
  public:
         
        /* Locked interval */
        
        std::atomic<bool> Lock;
        
        /* Starts Beryl instance. */
       
        void Initialize();

  	/* Time it takes for epoll_wait to read timeouts. */
  	
  	unsigned int Interval;
  	
	/* Ports where your BerylDB instance is listening to. */
	
	std::vector<BindingPort*> binds;

	/* Signals that are received to BerylDB. */
	
	static sig_atomic_t s_signal;

	/* Handles BerylDB's configuration files. */
	
	std::unique_ptr<Configuration> Config;
	
	/* Handles unique ids. */
        
	UIDHandler UID;

	/* Handles class objects that are awaiting to be removed. */
	
	ReducerHandler Reducer;

	/* Actions that will take place outside the current loop. */
	
	ActionHandler Atomics;

	ExtensionManager Extensions;
	
	/* Handles Cache and Session information */
	
	LoginCache Logins;
	
	/* Handles core databases. */
	
	CoreManager Core;
	
	/* Manages databases, both user-related and core ones. */
	
	StoreManager	Store;
	
	/* Parses and processes user-provided commands. */
	
	CommandHandler Commander;
	
	/* Handles logs. This class is used actively during BerylDB. */
	
	LogHandler Logs;
	
	/* Manages modules. */
	
	ModuleHandler Modules;

	/* Handles subscriptions channels. */
	
	ChannelManager Channels;
	
	/* Internal statistics. */
	
	Serverstats Stats;
	
	/* Manages tick timers. */
	
	TickManager Tickers;

	/* Handles user's connections. */
	
	ClientManager Clients;

	/* Utils function to the Beryl class. */
	
	Daemon Engine;

	/*
	 * Beryl's main. This function will initialize most of the beryl's core system
	 * and will read the config file.
	 */

	Beryl(int argc, char** argv);

	/*
	 * This function contains Beryl's mainloop. The mainloop runs
	 * continuously, unless a signal is received.
	 */
	 
	void Dispatcher();

	/* Exits Beryl. This function will call PrepareExit() before exiting. */
	
	void Exit(int status, bool nline = false, bool skip = false);

	/* Returns current time. */
	
	inline time_t Now() 
	{ 
		return this->TIME.tv_sec; 
	}
	
	/* Returns current time, as expressed in microseconds. */
	
	inline long TimeStamp() 
	{ 
		return this->TIME.tv_nsec; 
	}
	
	/* Returns startup time. */

	time_t GetStartup()
	{
		return this->startup;
	}
	
	/* Returns full path to configuration file. */
	
	const std::string& GetConfigFile()
	{
		return	this->ConfigFile;
	}

	/* Refreshes TIME, this function is called in every loop. */
	
	void Refresh();

	/* Returns BerylDB's version */
	 
	std::string GetVersion(bool full = false);

	/* Sets a signal, which is later processed with SignalManager() */

	static void AssignSignal(int signal);
	
	/* Returns pending buffer. */
	
	char* GetPendingBuffer()
	{
		return this->PendingBuffer;
	}
	
	/* Returns BRLD events. */

	ProtocolTrigger::BRLDEvents& GetBRLDEvents() 
	{ 
		return brldevents; 
	}
};

#include "inlines.h"
#include "triggers.h"
#include "clientmsg.h"
#include "clientevent.h"

