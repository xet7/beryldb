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

#include "beryl.h"
#include "modules/status.h"

class CommandStatus : public Command
{
 private:

	Events::ModuleEventProvider statusevprov;
	void DispatchData(Status::Context& status);

 public:
	
	CommandStatus(Module* Creator) : Command(Creator, "STATUS", 1, 2), statusevprov(Creator, "event/status")
	{
		last_empty_ok = false;
		syntax = "<symbol> [<servername>]";
		requires = 'e';
	}

	COMMAND_RESULT Handle(User* user, const Params& parameters) ;

	RouteParams GetRouting(User* user, const Params& parameters) 
	{
		if ((parameters.size() > 1) && (parameters[1].find('.') != std::string::npos))
		{
			return ROUTE_UNICAST(parameters[1]);
		}

		return ROUTE_LOCALONLY;
	}
};


void CommandStatus::DispatchData(Status::Context& status)
{
	const char statusid = status.GetSymbol();

	ModuleResult MOD_RESULT;
	UNTIL_RESULT_CUSTOM(statusevprov, Status::EventListener, OnStatus, MOD_RESULT, (status));

	if (MOD_RESULT == MOD_RES_STOP)
	{
		status.AppendLine(BRLD_END_STATUS, statusid, "End of STATUS report");
		return;
	}

	switch (statusid)
	{
		case 'p':
		{
			for (std::vector<BindingPort*>::const_iterator i = Kernel->binds.begin(); i != Kernel->binds.end(); ++i)
			{
				BindingPort* ls = *i;
				const std::string type = ls->listen_tag->as_string("type", "clients", 1);
				const std::string attach = ls->listen_tag->as_string("ssl", "plaintext", 1);

				status.AppendLine(249, ls->bind_sa.str() + " (" + type + ", " + attach + ")");
			}
		}

		break;

		case 'c':
		{
			status.AppendLine(249, "Users: "+convto_string(Kernel->Clients->GetInstances().size()));
			status.AppendLine(249, "Channels: "+convto_string(Kernel->Channels->GetSubs().size()));
			status.AppendLine(249, "Commands: "+convto_string(Kernel->Commander.GetCommands().size()));

			rusage R;

			
			if (!getrusage(RUSAGE_SELF,&R))	
			{
#ifndef __HAIKU__
				status.AppendLine(BRLD_ALLOCATION, "Total allocation: "+convto_string(R.ru_maxrss)+"K");
				status.AppendLine(BRLD_SHARED_MEM, "Shared memory size: "+convto_string(R.ru_idrss));
				status.AppendLine(BRLD_SIGNALS, "Signals:          "+convto_string(R.ru_nsignals));
				status.AppendLine(BRLD_PFAULTS, "Page faults:      "+convto_string(R.ru_majflt));
				status.AppendLine(BRLD_SWAPS, "Swaps:            "+convto_string(R.ru_nswap));
				status.AppendLine(BRLD_CSW, "Context Switches: Voluntary; "+convto_string(R.ru_nvcsw)+" Involuntary; "+convto_string(R.ru_nivcsw));
#endif
				float sample_sincelast = (Kernel->Now() - Kernel->Stats.LastSampled.tv_sec) * 1000000
					+ (Kernel->TimeStamp() - Kernel->Stats.LastSampled.tv_nsec) / 1000;
				float sample_used = ((R.ru_utime.tv_sec - Kernel->Stats.LastCPU.tv_sec) * 1000000 + R.ru_utime.tv_usec - Kernel->Stats.LastCPU.tv_usec);
				float per = (sample_used / sample_sincelast) * 100;

				status.AppendLine(BRLD_CPU_NOW, Daemon::Format("CPU Usage (current):    %03.5f%%", per));

				sample_sincelast = Kernel->Now() - Kernel->GetStartup();
				sample_used = (float)R.ru_utime.tv_sec + R.ru_utime.tv_usec / 100000.0;
				per = (sample_used / sample_sincelast) * 100;

				status.AppendLine(BRLD_CPU_USE, Daemon::Format("CPU Usage (total):  %03.5f%%", per));
			}
		}

		break;


	
		case 'u':
		{
			unsigned int up = static_cast<unsigned int>(Kernel->Now() - Kernel->GetStartup());
			
			status.AppendLine(BRLD_UPTIME, Daemon::Format("Uptime: %u days, %.2u:%.2u:%.2u", up / 86400, (up / 3600) % 24, (up / 60) % 60, up % 60));
		}
		
		break;
		

		default:
		break;
	}

	status.AppendLine(BRLD_END_STATUS, statusid, "End of STATUS report");
	return;
}

COMMAND_RESULT CommandStatus::Handle(User* user, const Params& parameters)
{
	if (parameters.size() > 1 && !engine::equals(parameters[1], Kernel->Config->ServerName))
	{
		return SUCCESS;
	}
	
	Status::Context status(user, parameters[0][0]);
	DispatchData(status);

	const std::vector<Status::Row>& rows = status.GetRows();
	
	for (std::vector<Status::Row>::const_iterator i = rows.begin(); i != rows.end(); ++i)
	{
		const Status::Row& row = *i;
		user->SendRemoteProtocol(row);
	}

	return SUCCESS;
}

class CoreModStatus : public Module
{
 private:

	CommandStatus cmd;

 public:

	CoreModStatus() : cmd(this)
	{

	}

	Version GetDescription() 
	{
		return Version("Provides the STATUS command", VF_CORE | VF_BERYLDB);
	}
};

MODULE_LOAD(CoreModStatus)
