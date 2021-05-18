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

#include <signal.h>
#include <iostream>
#include <getopt.h>

#include "beryl.h"
#include "engine.h"
#include "exit.h"

#ifdef ENABLE_TESTOFFICE

	#include "testoffice.h"

	void Beryl::CheckOffice()
	{

			/* Checks if run_tests is enabled. This scenario would prevent entering mainloop. */

			if (this->Config->usercmd.run_tests)
			{
					std::unique_ptr<TestOffice> Suite = std::make_unique<TestOffice>();
					Suite->Run();
					this->Exit(EXIT_CODE_OK);
			}
	}
	
#endif  

void Beryl::CommandLine()
{
	int do_debug = 0, do_nofork = 0, do_nolog = 0, do_tests = 0;
	int do_nopid = 0, do_asroot = 0, do_version = 0, do_flushdb = 0;

	struct option longopts[] =
	{
			{ "config",    required_argument, NULL,          'c' },
                        { "debug",     no_argument,       &do_debug,     1   },
			{ "nofork",    no_argument,       &do_nofork,    1   },
			{ "nolog",     no_argument,       &do_nolog,     1   },
#ifdef ENABLE_TESTOFFICE

                        { "testsuite",  no_argument,      &do_tests,     1   },

#endif
			{ "nopid",     no_argument,       &do_nopid,     1   },
			{ "asroot",    no_argument,       &do_asroot,    1   },
			{ "version",   no_argument,       &do_version,   1   },
			{ "flushdb",   no_argument,       &do_flushdb,   1   },
			{ 0, 0, 0, 0 }
	};

	char** argv = this->Config->usercmd.argv;
	int value;

	while ((value = getopt_long(this->Config->usercmd.argc, argv, ":c:", longopts, NULL)) != -1)
	{
			switch (value)
			{
					case 0:
						break;

					case 'c':

						this->ConfigFile = Kernel->Engine->get_real_path(optarg);
						break;

					default:

						std::cout << engine::color::bold << "Usage: " << engine::color::reset << argv[0] << " [--config <file>] [--debug] [--nofork] [--nolog]" << std::endl
						<< std::string(strlen(argv[0]) + 8, ' ') << "[--nopid] [--asroot] [--version]" << std::endl;
						this->Exit(EXIT_CODE_ARGV);
						break;
			}
	}
	
	if (do_flushdb)
	{
		this->Config->usercmd.flushdb = 1;
		return;
	}
	else
	{
                this->Config->usercmd.flushdb = 0;
	}
	
	/* Show version and then quit. */
	
	if (do_version)
	{
		std::cout << VERSION << std::endl;
		this->Exit(EXIT_CODE_OK, false, false);
	}
	
	this->Config->usercmd.forcedebug = !!do_debug;
	this->Config->usercmd.nofork = !!do_nofork;
	this->Config->usercmd.asroot = !!do_asroot;
	this->Config->usercmd.writelog = !do_nolog;
	this->Config->usercmd.writepid = !do_nopid;
 	this->Config->usercmd.run_tests = !!do_tests;

	if (!this->Config->usercmd.writepid)
	{
	       bprint(INFO, "This instance is not writing a PID file.");
	}
	 	
#ifdef ENABLE_TESTOFFICE
        
        /* Enables testoffice.cpp and runs Run() */
        
        if (do_tests)
        {       
               this->Config->usercmd.run_tests = 1;
               this->Config->usercmd.nofork = this->Config->usercmd.forcedebug = true;
        }
#endif
	
}

void Beryl::Detach()
{
	if (!this->Config->usercmd.nofork)
	{
		if (kill(getppid(), SIGTERM) == -1)
		{
			bprint(ERROR, "Error killing parent process: %s", strerror(errno));
			slog("STARTUP", LOG_DEFAULT, "Error killing parent process: %s", strerror(errno));
		}
	}

	if ((!Config->usercmd.nofork) && (!Config->usercmd.forcedebug))
	{
	        bprint(DONE, "Detaching.");
		
		int fd = open("/dev/null", O_RDWR);

		fclose(stdin);
		fclose(stderr);
		fclose(stdout);

		if (dup2(fd, STDIN_FILENO) < 0)
		{
			slog("STARTUP", LOG_DEFAULT, "Failed to dup /dev/null to stdin.");
		}
		
		if (dup2(fd, STDOUT_FILENO) < 0)
		{
			slog("STARTUP", LOG_DEFAULT, "Failed to dup /dev/null to stdout.");
		}
		
		if (dup2(fd, STDERR_FILENO) < 0)
		{
			slog("STARTUP", LOG_DEFAULT, "Failed to dup /dev/null to stderr.");
		}
		
		close(fd);
	}
	else
	{
		slog("STARTUP", LOG_DEFAULT, "This instance will NOT detach.");
	}

}

void Beryl::SignalManager(int signal)
{
	if (signal == SIGTERM)
	{
		Kernel->Exit(EXIT_CODE_SIGTERM);
	}
	else if (signal == SIGINT)
	{
		Kernel->Exit(EXIT_CODE_SIGINT, true, false, "Exiting.");
	}
}

std::string Beryl::GetVersion(bool full)
{
	std::string custom;

	if (this->Config->ModifiedVersion != "")
	{
               custom = this->Config->ModifiedVersion;
        }

        /* 
         * Full versions are typically reserved for 
         * those that have 'e' privileges or superior.
         */
         
	if (full)
	{
		return Daemon::Format("%s %s %s", BRANCH, VERSION, custom.c_str());
	}
	
	return Daemon::Format("%s %s", BRANCH, custom.c_str());
}

std::string UIDHandler::Create()
{
	const std::string& servername = Kernel->Config->ServerName;
	
	unsigned int sid = 0;

	for (std::string::const_iterator i = servername.begin(); i != servername.end(); ++i)
	{
		sid = 9 * sid + *i;
	}
	
	for (unsigned int i = 0; i < Kernel->Engine->generate_random_int(100); i++)
	{
		sid = 5 * sid + i;
	}

	std::string sidstr = convto_string(sid % 1000);
	sidstr.insert(0, 3 - sidstr.length(), '0');
	return sidstr;
}

void UIDHandler::FindNext(unsigned int pos)
{

	if (LastUID[pos] == 'Z')
	{
		LastUID[pos] = '0';
	}
	else if (LastUID[pos] == '9')
	{
		LastUID[pos] = 'A';

		if (pos == 4)
		{
			return;
		}

		this->FindNext(pos - 1);
	}
	else
	{
		LastUID[pos]++;
	}
}

void UIDHandler::Initialize()
{	
	const std::string sid = Kernel->Config->sid;
	
	LastUID.resize(UUID_LIMIT, '9');
	LastUID[0] = sid[0];
	LastUID[1] = sid[1];
	LastUID[2] = sid[2];
}

std::string UIDHandler::GetUID()
{
	while (1)
	{
		this->FindNext(UUID_LIMIT - 1);

		if (!Kernel->Clients->FindUUID(LastUID))
		{
			break;
		}
	}

	return LastUID;
}

void Beryl::StartListening()
{
	FailedListenings flist;
	Kernel->TryListening(flist);

	if (!flist.empty())
	{
		bprint(ERROR, "Unable to listen in all ports. A total of %ld ports have failed. " , flist.size());

		for (FailedListenings::const_iterator iter = flist.begin(); iter != flist.end(); ++iter)
		{
			const FailedPort& fiter = *iter;
			bprint(ERROR, "%s %s configured in <listen>  at %s", strerror(fiter.error), fiter.sa.str().c_str(),  fiter.tag->get_tag_location().c_str());
		}

		bprint(ERROR, "Keep in mind that it is always recommended  to use a public IP address in <listen:address> instead of a * wildcardcard.");
	}
}

