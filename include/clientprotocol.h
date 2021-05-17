/*
 * BerylDB - A modular database.
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

#include "event.h"

namespace ProtocolTrigger
{
	class EventHook;
	class MessageSource;
	struct BRLDEvents;
	struct ParseOutput;
	class TagSelection;
}

struct ProtocolTrigger::ParseOutput
{
	std::string cmd;

	ProtocolTrigger::ParamList params;

	ProtocolTrigger::TagMap tags;
};

class ProtocolTrigger::TagSelection
{

 private:
 
	std::bitset<64> selection;

 public:

	bool IsSelected(const TagMap& tags, TagMap::const_iterator it) const
	{
		const size_t index = std::distance(tags.begin(), it);
		return ((index < selection.size()) && (selection[index]));
	}

	void Select(const TagMap& tags, TagMap::const_iterator it)
	{
		const size_t index = std::distance(tags.begin(), it);

		if (index < selection.size())
		{
			selection[index] = true;
		}
	}

	
	bool operator==(const TagSelection& other) const
	{
		return (this->selection == other.selection);
	}
};

class ProtocolTrigger::MessageSource
{
 private:
 
	User* sourceuser;
	const std::string* sourcestr;

 public:
	
	MessageSource(User* Sourceuser = NULL)
	{
		SetSourceUser(Sourceuser);
	}

	
	MessageSource(const std::string& Sourcestr, User* Sourceuser = NULL)
	{
		SetSource(Sourcestr, Sourceuser);
	}

	
	const std::string* GetSource() const
	{
		if (sourcestr)
		{
			return sourcestr;
		}
		
		if (sourceuser)
		{
			return &sourceuser->GetHostFormat();
		}
		
		return NULL;
	}

	User* GetSourceUser() const 
	{ 
		return sourceuser; 
	}
	
	void SetSourceUser(User* Sourceuser)
	{
		sourceuser = Sourceuser;
		sourcestr = NULL;
	}

	
	void SetSource(const std::string& Sourcestr, User* Sourceuser = NULL)
	{
		sourcestr = &Sourcestr;
		sourceuser = Sourceuser;
	}

	
	void SetSource(const MessageSource& other)
	{
		sourcestr = other.sourcestr;
		sourceuser = other.sourceuser;
	}
};


class ProtocolTrigger::Message : public ProtocolTrigger::MessageSource
{
 public:
	
	struct SerializedData
	{
		const Serializer* serializer;
		TagSelection tagwl;

		
		SerializedData(const Serializer* Ser, const TagSelection& Tagwl)
			: serializer(Ser)
			, tagwl(Tagwl)
		{
		}

		
		bool operator==(const SerializedData& other) const
		{
			return ((serializer == other.serializer) && (tagwl == other.tagwl));
		}
	};

	class Param
	{
		const std::string* ptr;
		brld::aligned_storage<std::string> str;
		bool owned;

		void InitFrom(const Param& other)
		{
			owned = other.owned;
			if (owned)
				new(str) std::string(*other.str);
			else
				ptr = other.ptr;
		}

	 public:
		operator const std::string&() const { return (owned ? *str : *ptr); }

		Param()
			: ptr(NULL)
			, owned(false)
		{
		}

		Param(const std::string& s)
			: ptr(&s)
			, owned(false)
		{
		}

		Param(int, const char* s)
			: ptr(NULL)
			, owned(true)
		{
			new(str) std::string(s);
		}

		Param(int, const std::string& s)
			: ptr(NULL)
			, owned(true)
		{
			 new(str) std::string(s);
		}

		Param(const Param& other)
		{
			InitFrom(other);
		}

		~Param()
		{
			using std::string;
			if (owned)
				str->~string();
		}

		Param& operator=(const Param& other)
		{
			if (&other == this)
				return *this;

			using std::string;
			if (owned)
				str->~string();
			InitFrom(other);
			return *this;
		}

		bool IsOwned() const { return owned; }
	};

	typedef std::vector<Param> ParamList;

 private:
	typedef std::vector<std::pair<SerializedData, SerializedMessage> > SerializedList;

	ParamList params;
	TagMap tags;
	std::string command;
	bool msginit_done;
	mutable SerializedList serlist;
	bool sideeffect;

 protected:
	
	void SetCommand(const char* cmd)
	{
		command.clear();
		if (cmd)
			command = cmd;
	}

 public:
	
	Message(const char* cmd, User* Sourceuser = NULL)
		: ProtocolTrigger::MessageSource(Sourceuser)
		, command(cmd ? cmd : std::string())
		, msginit_done(false)
		, sideeffect(false)
	{
		params.reserve(8);
		serlist.reserve(8);
	}

	
	Message(const char* cmd, const std::string& Sourcestr, User* Sourceuser = NULL)
		: ProtocolTrigger::MessageSource(Sourcestr, Sourceuser)
		, command(cmd ? cmd : std::string())
		, msginit_done(false)
		, sideeffect(false)
	{
		params.reserve(8);
		serlist.reserve(8);
	}

	
	const ParamList& GetParams() const { return params; }

	
	const TagMap& GetTags() const { return tags; }

	const char* GetCommand() const 
	{ 
		return command.c_str(); 
	}

	
	void PushParam(const char* str) { params.push_back(Param(0, str)); }

	
	void PushParam(const std::string& str) { params.push_back(Param(0, str)); }

	
	void PushParamRef(const std::string& str) { params.push_back(str); }

	
	void PushParamPlaceholder() { params.push_back(Param()); }

	
	void ReplaceParam(unsigned int index, const char* str) { params[index] = Param(0, str); }

	
	void ReplaceParam(unsigned int index, const std::string& str) { params[index] = Param(0, str); }

	
	void ReplaceParamRef(unsigned int index, const std::string& str) { params[index] = Param(str); }

	
	void AddTag(const std::string& tagname, MessageDataProvider* tagprov, const std::string& val, void* tagdata = NULL)
	{
		tags.insert(std::make_pair(tagname, MessageProvider(tagprov, val, tagdata)));
	}

	
	void AddTags(const ProtocolTrigger::TagMap& newtags)
	{
		tags.insert(newtags.begin(), newtags.end());
	}

	
	const SerializedMessage& GetSerialized(const SerializedData& serializeinfo) const;

	
	void ClearParams()
	{
		msginit_done = false;
		params.clear();
		tags.clear();
		ResetCache();
	}

	
	void ResetCache()
	{
		serlist.clear();
	}

	void CopyAll()
	{
		size_t j = 0;
		for (ParamList::iterator i = params.begin(); i != params.end(); ++i, j++)
		{
			Param& curr = *i;
			if (!curr.IsOwned())
				ReplaceParam(j, curr);
		}
	}

	void SetSideEffect(bool Sideeffect) { sideeffect = Sideeffect; }
	bool Valid_SIDeEffect() const { return sideeffect; }

	friend class Serializer;
};


class ProtocolTrigger::Event
{
	EventProvider* event;
	Message* initialmsg;
	const MessageList* initialmsglist;
	bool eventinit_done;

 public:
	
	Event(EventProvider& protoeventprov)
		: event(&protoeventprov)
		, initialmsg(NULL)
		, initialmsglist(NULL)
		, eventinit_done(false)
	{
	}

	
	Event(EventProvider& protoeventprov, ProtocolTrigger::Message& msg)
		: event(&protoeventprov)
		, initialmsg(&msg)
		, initialmsglist(NULL)
		, eventinit_done(false)
	{
	}

	
	void SetMessage(Message* msg)
	{
		initialmsg = msg;
		initialmsglist = NULL;
	}

	
	void SetMessageList(const MessageList& msglist)
	{
		initialmsg = NULL;
		initialmsglist = &msglist;
	}

	
	void get_messagesForUser(LocalUser* user, MessageList& messagelist);
};

class ProtocolTrigger::MessageTagEvent
	: public Events::ModuleEventProvider
{
 public:
	MessageTagEvent(Module* mod)
		: ModuleEventProvider(mod, "event/messagetag")
	{
	}
};


class ProtocolTrigger::MessageDataProvider : public Events::ModuleEventListener
{
 public:
	
	MessageDataProvider(Module* mod)
		: Events::ModuleEventListener(mod, "event/messagetag")
	{
	}

	
	virtual void OnPopulateTags(ProtocolTrigger::Message& msg)
	{
	}

	
	virtual ModuleResult OnProcessTag(User* user, const std::string& tagname, std::string& tagvalue)
	{
		return MOD_RES_SKIP;
	}

	
	virtual bool ShouldSendTag(LocalUser* user, const MessageProvider& tagdata) = 0;
};


class ProtocolTrigger::EventHook : public Events::ModuleEventListener
{
 public:
	static std::string GetEventName(const std::string& name)
	{
		return "event/protoevent_" + name;
	}

	
	EventHook(Module* mod, const std::string& name, unsigned int priority = Events::ModuleEventListener::DefaultPriority)
		: Events::ModuleEventListener(mod, GetEventName(name), priority)
	{
	}

	
	virtual void OnEventInit(const ProtocolTrigger::Event& ev)
	{
	}

	
	virtual ModuleResult OnPreEventSend(LocalUser* user, const ProtocolTrigger::Event& ev, ProtocolTrigger::MessageList& messagelist) = 0;
};


class ProtocolTrigger::EventProvider : public Events::ModuleEventProvider
{
 public:
	
	EventProvider(Module* Mod, const std::string& eventname)
		: Events::ModuleEventProvider(Mod, ProtocolTrigger::EventHook::GetEventName(eventname))
	{
	}
};


struct ProtocolTrigger::BRLDEvents
{
	EventProvider numeric;
	EventProvider join;
	EventProvider part;
	EventProvider quit;
	EventProvider login;
	EventProvider publish;
	EventProvider ping;
	EventProvider pong;
	EventProvider error;

	BRLDEvents() : numeric(NULL, "NUMERIC")
				, join(NULL, "JOIN")
				, part(NULL, "PART")
				, quit(NULL, "QUIT")
				, login(NULL, "LOGIN")
				, publish(NULL, "PUBLISH")
				, ping(NULL, "PING")
				, pong(NULL, "PONG")
				, error(NULL, "ERROR")
	{

	}
};


class Externalize ProtocolTrigger::Serializer : public DataProvider
{
 private:

	ProtocolTrigger::MessageTagEvent evprov;
	
	TagSelection MakeTagWhitelist(LocalUser* user, const TagMap& tagmap) const;

 public:
	
	Serializer(Module* mod, const char* Name);
	
	bool HandleTag(LocalUser* user, const std::string& tagname, std::string& tagvalue, TagMap& tags) const;

	
	const SerializedMessage& SerializeForUser(LocalUser* user, Message& msg);

	
	virtual std::string Serialize(const Message& msg, const TagSelection& tagwl) const = 0;

	
	virtual bool Parse(LocalUser* user, const std::string& line, ParseOutput& parseoutput) = 0;
};

inline ProtocolTrigger::MessageProvider::MessageProvider(MessageDataProvider* prov, const std::string& val, void* data)
	: tagprov(prov)
	, value(val)
	, provdata(data)
{

}
