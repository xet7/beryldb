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

namespace Events
{
	class ModuleEventListener;
	class ModuleEventProvider;
}

class Events::ModuleEventProvider : public ServiceProvider, private ddl_base_ref::aptureAttach
{
 public:
	struct Comp
	{
		bool operator()(ModuleEventListener* lhs, ModuleEventListener* rhs) const;
	};

	struct ElementComp
	{
		bool operator()(ModuleEventListener* lhs, ModuleEventListener* rhs) const;
	};

	typedef brld::flat_multiset<ModuleEventListener*, Comp, ElementComp> SubscriberList;

	
	ModuleEventProvider(Module* mod, const std::string& eventid)
		: ServiceProvider(mod, eventid, SERVICE_DATA)
		, prov(mod, eventid)
	{
		prov.SetaptureAttach(this);
	}

	
	const Module* GetModule() const { return prov.creator; }

	
	const SubscriberList& GetSubscribers() const { return prov->subscribers; }

	
	void Subscribe(ModuleEventListener* subscriber)
	{
		subscribers.insert(subscriber);
		OnSubscribe(subscriber);
	}

	
	void Unsubscribe(ModuleEventListener* subscriber)
	{
		subscribers.erase(subscriber);
		OnUnsubscribe(subscriber);
	}

 private:

	void OnCapture() 
	{
		if (*prov != this)
		{
			subscribers.clear();
		}
	}

	
	virtual void OnSubscribe(ModuleEventListener* subscriber) { }

	
	virtual void OnUnsubscribe(ModuleEventListener* subscriber) { }

	
	dynamic_reference_nocheck<ModuleEventProvider> prov;

	
	SubscriberList subscribers;
};


class Events::ModuleEventListener : private ddl_base_ref::aptureAttach
{
	
	dynamic_reference_nocheck<ModuleEventProvider> prov;

	const unsigned int eventpriority;

	
	void OnCapture() 
	{
		prov->Subscribe(this);
	}

 public:
	static const unsigned int DefaultPriority = 100;

	
	ModuleEventListener(Module* mod, const std::string& eventid, unsigned int eventprio = DefaultPriority)
		: prov(mod, eventid)
		, eventpriority(eventprio)
	{
		prov.SetaptureAttach(this);

		if (prov)
		{
			ModuleEventListener::OnCapture();
		}
	}

	~ModuleEventListener()
	{
		if (prov)
		{
			prov->Unsubscribe(this);
		}
	}

	
	const Module* GetModule() const { return prov.creator; }

	
	unsigned int GetPriority() const { return eventpriority; }
};

inline bool Events::ModuleEventProvider::Comp::operator()(Events::ModuleEventListener* lhs, Events::ModuleEventListener* rhs) const
{
	return (lhs->GetPriority() < rhs->GetPriority());
}

inline bool Events::ModuleEventProvider::ElementComp::operator()(Events::ModuleEventListener* lhs, Events::ModuleEventListener* rhs) const
{
	if (lhs->GetPriority() < rhs->GetPriority())
		return true;
	if (lhs->GetPriority() > rhs->GetPriority())
		return false;
	return std::less<ModuleEventListener*>()(lhs, rhs);
}


#define NOTIFY_MODS_CUSTOM(prov, listenerclass, func, params) do { \
	if (!(prov).GetModule() || !(prov).GetModule()->exiting) \
	{ \
		const ::Events::ModuleEventProvider::SubscriberList& _handlers = (prov).GetSubscribers(); \
		for (::Events::ModuleEventProvider::SubscriberList::const_iterator _i = _handlers.begin(); _i != _handlers.end(); ++_i) \
		{ \
			listenerclass* _t = static_cast<listenerclass*>(*_i); \
			const Module* _m = _t->GetModule(); \
			if (_m && !_m->exiting) \
			_t->func params ; \
		} \
	} \
} while (0);


#define UNTIL_RESULT_CUSTOM(prov, listenerclass, func, result, params) do { \
	result = MOD_RES_SKIP; \
	if (!(prov).GetModule() || !(prov).GetModule()->exiting) \
	{ \
		const ::Events::ModuleEventProvider::SubscriberList& _handlers = (prov).GetSubscribers(); \
		for (::Events::ModuleEventProvider::SubscriberList::const_iterator _i = _handlers.begin(); _i != _handlers.end(); ++_i) \
		{ \
			listenerclass* _t = static_cast<listenerclass*>(*_i); \
			const Module* _m = _t->GetModule(); \
			if (!_m || _m->exiting) \
				continue; \
			result = _t->func params ; \
			if (result != MOD_RES_SKIP) \
				break; \
		} \
	} \
} while (0);
