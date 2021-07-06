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
#include "exit.h"

static brld::node_list<ddl_base_ref>* dll_reference = NULL;

void ddl_base_ref::reset_all()
{
	if (!dll_reference)
	{
		return;
	}
	
	for (brld::node_list<ddl_base_ref>::iterator i = dll_reference->begin(); i != dll_reference->end(); ++i)
	{
		(*i)->resolve();
	}
}

Version::Version(const std::string &desc, int flags) : description(desc), Flags(flags)
{

}

Version::Version(const std::string &desc, int flags, const std::string& clusterdata) : description(desc), Flags(flags), cluster_data(clusterdata)
{

}

Module::Module() : DLL(NULL), exiting(false)
{

}

DiscardResult Module::discard()
{
	if (this->DLL)
	{
		Kernel->Reducer.Add(this->DLL);
	}
	
	return base_class::discard();
}

Module::~Module()
{

}

void Module::DetachEvent(Application app)
{
	Kernel->Modules->Detach(app, this);
}

void		Module::ConfigReading(config_status& status) { }
void		Module::OnInstanceConnect(LocalUser*) { DetachEvent(I_OnInstanceConnect); }
ModuleResult	Module::OnUserPreExit(LocalUser*, std::string&) { DetachEvent(I_OnUserPreExit); return MOD_RES_SKIP; }
void		Module::OnUserExit(User*, const std::string&) { DetachEvent(I_OnUserExit); }
void		Module::OnInstanceDisconnect(LocalUser*) { DetachEvent(I_OnInstanceDisconnect); }
void		Module::OnUserJoin(Subscription*, bool, bool, DiscardList&) { DetachEvent(I_OnUserJoin); }
void		Module::OnPostJoin(Subscription*) { DetachEvent(I_OnPostJoin); }
void		Module::OnUserPart(Subscription*, DiscardList&) { DetachEvent(I_OnUserPart); }
ModuleResult	Module::OnUserPreJoin(LocalUser*, Channel*, const std::string&) { DetachEvent(I_OnUserPreJoin); return MOD_RES_SKIP; }
void		Module::OnAdmin(User*, const std::string&) { DetachEvent(I_OnAdmin); }
ModuleResult	Module::OnInstancePrePublish(User*, const MessageTarget&, MessageLoad&) { DetachEvent(I_OnInstancePrePublish); return MOD_RES_SKIP; }
ModuleResult	Module::OnUserPreLogin(LocalUser*, const std::string&) { DetachEvent(I_OnUserPreLogin); return MOD_RES_SKIP; }
void		Module::OnUserPostLogin(User*, const std::string&) { DetachEvent(I_OnUserPostLogin); }
void		Module::OnInstaceReady(std::map<std::string, std::string>&) { DetachEvent(I_OnInstaceReady); }
ModuleResult	Module::InstanceLogout(User*, User*, const std::string&) { DetachEvent(I_InstanceLogout); return MOD_RES_SKIP; }
void		Module::OnLoadModule(Module*) { DetachEvent(I_OnLoadModule); }
void		Module::OnUnloadModule(Module*) { DetachEvent(I_OnUnloadModule); }
void		Module::OnEveryTwoSeconds(time_t) { DetachEvent(I_OnEveryTwoSeconds); }
void		Module::OnEveryHalfMinute(time_t) { DetachEvent(I_OnEveryHalfMinute); }
ModuleResult	Module::OnPreCommand(std::string&, CommandModel::Params&, LocalUser*, bool) { DetachEvent(I_OnPreCommand); return MOD_RES_SKIP; }
void		Module::OnPostCommand(Command*, const CommandModel::Params&, LocalUser*, COMMAND_RESULT, bool) { DetachEvent(I_OnPostCommand); }
void		Module::OnCommandBlocked(const std::string&, const CommandModel::Params&, LocalUser*) { DetachEvent(I_OnCommandBlocked); }
void 		Module::OnQueryFailed(DBL_CODE code, LocalUser* luser, std::shared_ptr<QueryBase> bquery) { DetachEvent(I_OnQueryFailed); }
void		Module::OnInstanceInit(LocalUser*) { DetachEvent(I_OnInstanceInit); }
void		Module::OnInstancePostInit(LocalUser*) { DetachEvent(I_OnInstancePostInit); }
ModuleResult	Module::OnUserReady(LocalUser*) { DetachEvent(I_OnUserReady); return MOD_RES_SKIP; }
ModuleResult	Module::OnUserRegister(LocalUser*) { DetachEvent(I_OnUserRegister); return MOD_RES_SKIP; }
ModuleResult	Module::OnPreChangeHost(LocalUser*, const std::string&) { DetachEvent(I_OnPreChangeHost); return MOD_RES_SKIP; }
ModuleResult	Module::OnPassCompare(Expandable* ex, const std::string &password, const std::string &input, const std::string& hashtype) { DetachEvent(I_OnPassCompare); return MOD_RES_SKIP; }
void		Module::OnPostConnect(User*) { DetachEvent(I_OnPostConnect); }
void		Module::OnInstancePostPublish(User*, const MessageTarget&, const MessageLoad&) { DetachEvent(I_OnInstancePostPublish); }
void		Module::OnInstancePublishBlocked(User*, const MessageTarget&, const MessageLoad&) { DetachEvent(I_OnInstancePublishBlocked); }
void		Module::OnDecodeMetaData(Expandable*, const std::string&, const std::string&) { DetachEvent(I_OnDecodeMetaData); }
void		Module::OnHostSet(User*, const std::string&) { DetachEvent(I_OnHostSet); }
void		Module::OnSetHost(User*, const std::string&) { DetachEvent(I_OnSetHost); }
void		Module::OnSetAgent(User*, const std::string&) { DetachEvent(I_OnSetAgent); }
void 		Module::OnCleanup(extension_item::ExpandableType, Expandable*) { }
ModuleResult	Module::OnChannelPreDelete(Channel*) { DetachEvent(I_OnChannelPreDelete); return MOD_RES_SKIP; }
void		Module::OnChannelDelete(Channel*) { DetachEvent(I_OnChannelDelete); }
void		Module::OnNeighborBuild(User*, SubscriptionList&, std::map<User*,bool>&) { DetachEvent(I_OnNeighborBuild); }
void		Module::OnEveryHour(time_t) { DetachEvent(I_OnEveryHour); }
ModuleResult	Module::OnSetConnectConfig(LocalUser* user, connect_config* myclass) { DetachEvent(I_OnSetConnectConfig); return MOD_RES_SKIP; }
void 		Module::OnInstancePublish(User*, const MessageTarget&, const MessageLoad&) { DetachEvent(I_OnInstancePublish); }
ModuleResult    Module::OnAcceptConnection(int, BindingPort*, engine::sockets::sockaddrs*, engine::sockets::sockaddrs*) { DetachEvent(I_OnAcceptConnection); return MOD_RES_SKIP; }
void		Module::OnSetUserIP(LocalUser*) { DetachEvent(I_OnSetUserIP); }
void		Module::OnServiceAdd(ServiceProvider&) { DetachEvent(I_OnServiceAdd); }
void		Module::OnServiceDel(ServiceProvider&) { DetachEvent(I_OnServiceDel); }
ModuleResult	Module::OnConnectionFail(LocalUser*, LiveSocketError) { DetachEvent(I_OnConnectionFail); return MOD_RES_SKIP; }
void		Module::OnHalt(const std::string& reason) { DetachEvent(I_OnHalt); }
void 		Module::OnExpireAdd(User* user, const std::string& dbname, const std::string& key, const std::string& select, unsigned int seconds) { DetachEvent(I_OnExpireAdd); }
void 	        Module::OnExpireDel(User* user, const std::string& dbname, const std::string& key, const std::string& select) { DetachEvent(I_OnExpireDel); } 

ServiceProvider::ServiceProvider(Module* Creator, const std::string& Name, ServiceID Type) : creator(Creator), name(Name), service(Type)
{
	if ((Kernel) && (Kernel->Modules->new_services))
	{
		Kernel->Modules->new_services->push_back(this);
	}
}

void ServiceProvider::DisableAutoRegister()
{
	if ((Kernel) && (Kernel->Modules->new_services))
	{
		stdhelpers::erase(*Kernel->Modules->new_services, this);
	}
}

const char* ServiceProvider::GetTypeString() const
{
	switch (service)
	{
		case SERVICE_COMMAND:
			return "command";

		case SERVICE_STREAM:
			return "metadata";

		case SERVICE_QUEUE:
			return "ioattach";

		case SERVICE_DATA:
			return "data service";

		case SERVICE_CUSTOM:
			return "module service";
	}

	return "unknown service";
}

ModuleHandler::ModuleHandler()
{

}

ModuleHandler::~ModuleHandler()
{

}

bool ModuleHandler::Attach(Application app, Module* module)
{
	if (stdhelpers::isin(EventHandlers[app], module))
	{
		return false;
	}

	EventHandlers[app].push_back(module);
	return true;
}

bool ModuleHandler::Detach(Application app, Module* mod)
{
	return stdhelpers::erase(EventHandlers[app], mod);
}

void ModuleHandler::Attach(Application* app, Module* module, size_t sz)
{
	for (size_t n = 0; n < sz; ++n)
	{
		Attach(app[n], module);
	}
}

void ModuleHandler::AttachAll(Module* module)
{
	for (size_t i = 0; i != I_END; ++i)
	{
		Attach((Application)i, module);
	}
}

void ModuleHandler::DetachAll(Module* module)
{
	for (size_t n = 0; n != I_END; ++n)
	{
		Detach((Application)n, module);
	}
}

void ModuleHandler::SetPriority(Module* module, Priority s)
{
	for (size_t n = 0; n != I_END; ++n)
	{
		SetPriority(module, (Application)n, s);
	}
}

bool ModuleHandler::SetPriority(Module* module, Application app, Priority s, Module* which)
{
	size_t my_pos = 0;

	for (size_t x = 0; x != EventHandlers[app].size(); ++x)
	{
			if (EventHandlers[app][x] == module)
			{
				my_pos = x;
				goto found_src;
			}
	}

	
	return false;

found_src:
	size_t swap_pos = my_pos;
	switch (s)
	{
		case PRIORITY_LAST:
			if (PriorityState != PRIO_STATE_FIRST)
				return true;
			else
				swap_pos = 0;
			break;
		case PRIORITY_FIRST:
			if (PriorityState != PRIO_STATE_FIRST)
				return true;
			else
				swap_pos = EventHandlers[app].size() - 1;
			break;
		case PRIORITY_BEFORE:
		{
			
			for (size_t x = EventHandlers[app].size() - 1; x > my_pos; --x)
			{
				if (EventHandlers[app][x] == which)
				{
					swap_pos = x;
					goto swap_now;
				}
			}
			return true;
		}

		case PRIORITY_AFTER:
		{
			for (size_t x = 0; x < my_pos; ++x)
			{
				if (EventHandlers[app][x] == which)
				{
					swap_pos = x;
					goto swap_now;
				}
			}

			return true;
		}
	}

swap_now:

	if (swap_pos != my_pos)
	{
		if (PriorityState == PRIO_STATE_LAST)
			PriorityState = PRIO_STATE_AGAIN;
		
		int incrmnt = 1;

		if (my_pos > swap_pos)
		{
			incrmnt = -1;
		}

		for (unsigned int j = my_pos; j != swap_pos; j += incrmnt)
		{
			if ((j + incrmnt > EventHandlers[app].size() - 1) || ((incrmnt == -1) && (j == 0)))
				continue;

			std::swap(EventHandlers[app][j], EventHandlers[app][j+incrmnt]);
		}
	}

	return true;
}

bool ModuleHandler::PrioritizeAttachs()
{
	for (int attempts = 0; attempts < 20; attempts++)
	{
		PriorityState = attempts > 0 ? PRIO_STATE_LAST : PRIO_STATE_FIRST;
		
		for (std::map<std::string, Module*>::iterator n = Modules.begin(); n != Modules.end(); ++n)
		{
			n->second->Prioritize();
		}

		if (PriorityState == PRIO_STATE_LAST)
		{
			break;
		}
		if (attempts == 19)
		{
			return false;
		}
	}
	return true;
}

bool ModuleHandler::CheckUnload(Module* module)
{
	std::map<std::string, Module*>::iterator finding = Modules.find(module->SourceFile);

	if ((finding == Modules.end()) || (finding->second != module) || (module->exiting))
	{
		last_known_error = "Module " + module->SourceFile + " is not loaded.";
		slog("MODULE", LOG_DEFAULT, last_known_error);
		return false;
	}

	module->exiting = true;
	return true;
}

void ModuleHandler::SmartUnload(Module* module)
{
	NOTIFY_MODS(OnUnloadModule, (module));

	std::map<std::string, Module*>::iterator finding = Modules.find(module->SourceFile);

	std::vector<reference<extension_item> > items;
	Kernel->Extensions.BeginUnregister(finding->second, items);
	const ChanMap& chans = Kernel->Channels->GetSubs();
	
	for (ChanMap::const_iterator c = chans.begin(); c != chans.end(); )
	{
		Channel* chan = c->second;
		++c;
		module->OnCleanup(extension_item::EXT_CHANNEL, chan);
		chan->UnlinkExtensions(items);
		const Channel::SubscriptionMap& users = chan->GetInstances();

		for (Channel::SubscriptionMap::const_iterator mi = users.begin(); mi != users.end(); ++mi)
		{
			module->OnCleanup(extension_item::EXT_SUBSCRIPTION, mi->second);
			mi->second->UnlinkExtensions(items);
		}
	}

	const UserMap& users = Kernel->Clients->GetInstances();

	for (UserMap::const_iterator u = users.begin(); u != users.end(); )
	{
		User* user = u->second;
		++u;
		module->OnCleanup(extension_item::EXT_USER, user);
		user->UnlinkExtensions(items);
	}

	for (DataProviderMap::iterator i = DataProviders.begin(); i != DataProviders.end(); )
	{
		DataProviderMap::iterator curr = i++;

		if (curr->second->creator == module)
		{
			DataProviders.erase(curr);
			NOTIFY_MODS(OnServiceDel, (*curr->second));
		}
	}

	ddl_base_ref::reset_all();

	DetachAll(module);

	Modules.erase(finding);
	Kernel->Reducer.Add(module);

	slog("MODULE", LOG_DEFAULT, "Module %s unloaded", module->SourceFile.c_str());
}

void ModuleHandler::UnloadAll()
{
	/*
	 * Every module is attempted to unllowed multiple times. This is
	 * achieved in order to unattach modules using service providers, and so
	 * these can later remove themselves.
	 */

	for (int attempts = 0; attempts < 5; attempts++)
	{
		std::map<std::string, Module*>::iterator i = Modules.begin();
	
		while (i != Modules.end())
		{
			std::map<std::string, Module*>::iterator me = i++;
			
			if (this->CheckUnload(me->second))
			{
				this->SmartUnload(me->second);
			}
		}
	}
}

namespace
{
	struct PromiseUnload : public PromiseAction
	{
		Module* const mod;
		PromiseUnload(Module* m) : mod(m) 
		{

		}

		void Call() 
		{
			Kernel->Modules->SmartUnload(mod);
			Kernel->Reducer.Apply();
			Kernel->Reducer.Add(this);
		}
	};
}

bool ModuleHandler::Unload(Module* mod)
{
	if (!CheckUnload(mod))
	{
		return false;
	}

	Kernel->Atomics.AddAction(new PromiseUnload(mod));
	return true;
}

void ModuleHandler::LoadAll()
{
	std::map<std::string, ServiceList> ServiceMap;
	LoadCore(ServiceMap);

	unsigned int counter = 0;
	
	MultiTag tags = Kernel->Config->GetTags("module");
	
	for (config_iterator i = tags.first; i != tags.second; ++i)
	{
		config_rule* tag = i->second;

		const std::string shortname = tag->as_string("name");
	
		if (shortname.empty())
		{
			continue; 
		}
		
		/* These module names are not allowed. */

		if (shortname == "root" || shortname == "coredb" || shortname == "beryldb")
		{
			bprint(ERROR, "Invalid module name: %s", shortname.c_str());
			Kernel->Exit(EXIT_CODE_CONFIG);
		}

		const std::string name = FullModName(shortname);
		
		if (Modules.find(name) != Modules.end())
		{
			continue;
		}

		this->new_services = &ServiceMap[name];
	 	bprint(DONE, "Loading module: %s", Daemon::Welcome(shortname).c_str()); 
	 	
	 	counter++;
	 	
		if (!this->Load(name, true))
		{
			slog("MODULE", LOG_DEFAULT, this->LastError());
			bprint(ERROR, "Unable to load module: %s", this->LastError().c_str());
			Kernel->Exit(EXIT_CODE_MODULE);
		}
	}

        if (counter > 0)
        {
		iprint(counter, "Modules loaded."); 
	}

	for (ModuleMap::const_iterator i = Modules.begin(); i != Modules.end(); ++i)
	{
		Module* module = i->second;
		
		try
		{
			AttachAll(module);
			AttachServices(ServiceMap[i->first]);
			module->Initialize();
		}
		catch (KernelException& modexcept)
		{
			last_known_error = "Unable to initialize " + module->SourceFile + ": " + modexcept.get_reason();
			bprint(ERROR, "%s", last_known_error.c_str());
			Kernel->Exit(EXIT_CODE_MODULE);
		}
	}

	this->new_services = NULL;
	config_status confstatus(NULL, true);

	for (ModuleMap::const_iterator i = Modules.begin(); i != Modules.end(); ++i)
	{
		Module* module = i->second;

		try
		{
			slog("MODULE", LOG_DEBUG, "Reading configuration for %s", i->first.c_str());
			module->ConfigReading(confstatus);
		}
		catch (KernelException& modexcept)
		{
			last_known_error = "Unable to read the configuration for " + module->SourceFile + ": " + modexcept.get_reason();
			slog("MODULE", LOG_DEFAULT, last_known_error);
			bprint(ERROR, "%s", last_known_error.c_str());
			Kernel->Exit(EXIT_CODE_CONFIG);
		}
	}

	if (!PrioritizeAttachs())
	{
		Kernel->Exit(EXIT_CODE_MODULE);
	}
}


std::string& ModuleHandler::LastError()
{
	return last_known_error;
}

void ModuleHandler::AttachServices(const ServiceList& list)
{
	for (ServiceList::const_iterator i = list.begin(); i != list.end(); ++i)
	{
		ServiceProvider& s = **i;
		AttachService(s);
	}
}

void ModuleHandler::AttachService(ServiceProvider& item)
{
	switch (item.service)
	{
		case SERVICE_DATA:
		case SERVICE_QUEUE:
		{
			DataProviders.insert(std::make_pair(item.name, &item));
			std::string::size_type slash = item.name.find('/');
			
			if (slash != std::string::npos)
			{
				DataProviders.insert(std::make_pair(item.name.substr(0, slash), &item));
				DataProviders.insert(std::make_pair(item.name.substr(slash + 1), &item));
			}
			
			ddl_base_ref::reset_all();
			break;
		}
		
		default:
			item.RegisterService();
	}

	NOTIFY_MODS(OnServiceAdd, (item));
}

void ModuleHandler::UnAttachService(ServiceProvider& item)
{
	switch (item.service)
	{

		case SERVICE_DATA:
		case SERVICE_QUEUE:
		{
			DelReference(&item);
			break;
		}

		default:
			throw ModuleException("Unable to delete an undefined service type");
	}

	NOTIFY_MODS(OnServiceDel, (item));
}

ServiceProvider* ModuleHandler::FindService(ServiceID type, const std::string& name)
{
	switch (type)
	{
		case SERVICE_DATA:
		case SERVICE_QUEUE:
		{
			DataProviderMap::iterator i = DataProviders.find(name);
			
			if (i != DataProviders.end() && i->second->service == type)
			{
				return i->second;
			}
			
			return NULL;
		}

		default:
			throw ModuleException("Cannot find unknown service type");
	}
}

std::string ModuleHandler::FullModName(const std::string& name)
{
	std::string FullModule;
	
	if (name.compare(0, 5, "core_") != 0 && name.compare(0, 2, "m_") != 0)
	{
		FullModule.append("m_");
	}
	
	FullModule.append(name);
	
	/* Appends ".so" to module name. */
	
	if (name.length() < 3 || name.compare(name.size() - 3, 3, ".so") != 0)
	{
		FullModule.append(".so");
	}
	
	return FullModule;
}

ddl_base_ref::ddl_base_ref(Module* Creator, const std::string& Name)
	: name(Name), attach(NULL), value(NULL), creator(Creator)
{
	if (!dll_reference)
	{
		dll_reference = new brld::node_list<ddl_base_ref>;
	}
	
	dll_reference->push_front(this);

	if (Kernel)
	{
		resolve();
	}
}

ddl_base_ref::~ddl_base_ref()
{
	dll_reference->erase(this);

	if (dll_reference->empty())
	{
		delete dll_reference;
		dll_reference = NULL;
	}
}

void ddl_base_ref::SetProvider(const std::string& newname)
{
	name = newname;
	resolve();
}

void ddl_base_ref::resolve()
{
	ModuleHandler::DataProviderMap::iterator i = Kernel->Modules.DataProviders.lower_bound(name);
	
	if ((i != Kernel->Modules.DataProviders.end()) && (i->first == this->name))
	{
		ServiceProvider* newvalue = i->second;

		if (value != newvalue)
		{
			value = newvalue;

			if (attach)
			{
				attach->OnCapture();
			}
		}
	}
	else
	{
		value = NULL;
	}
}

Module* ModuleHandler::Find(const std::string &name)
{
	std::map<std::string, Module*>::const_iterator finding = Modules.find(this->FullModName(name));

	if (finding == Modules.end())
	{
		return NULL;
	}
	else
	{
		return finding->second;
	}
}

void ModuleHandler::CreateReference(const std::string& name, ServiceProvider* service)
{
	DataProviders.insert(std::make_pair(name, service));
	ddl_base_ref::reset_all();
}

void ModuleHandler::DelReference(ServiceProvider* service)
{
	for (DataProviderMap::iterator i = DataProviders.begin(); i != DataProviders.end(); )
	{
		ServiceProvider* curr = i->second;

		if (curr == service)
		{
			DataProviders.erase(i++);
		}
		else
		{
			++i;
		}
	}

	ddl_base_ref::reset_all();
}
