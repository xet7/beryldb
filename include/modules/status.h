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

#include "event.h"

namespace Status
{
	class Context;
	class EventListener;
	class Row;
}

class Status::EventListener : public Events::ModuleEventListener
{
 public:

	EventListener(Module* mod) : ModuleEventListener(mod, "event/status")
	{
	
	}

	virtual ModuleResult OnStatus(Status::Context& status) = 0;
};

class Status::Row : public Numeric::Numeric
{
	public:
	
		Row(unsigned int num) : Numeric(num)
		{
			
		}
};

class Status::Context
{
	
	User* const source;

	
	std::vector<Row> rows;

	
	const char symbol;

 public:
	
	Context(User* src, char sym) : source(src), symbol(sym)
	{
	
	}
	
	User* GetSource() const 
	{ 
		return source; 
	}
	
	const std::vector<Row>& GetRows() const 
	{ 
		return rows; 
	}
	
	char GetSymbol() const 
	{ 
		return symbol; 
	}

	void AppendLine(const Row& row) { rows.push_back(row); }

	template <typename T1>
	void AppendLine(unsigned int numeric, T1 p1)
	{
		Row n(numeric);
		n.push(p1);
		AppendLine(n);
	}

	template <typename T1, typename T2>
	void AppendLine(unsigned int numeric, T1 p1, T2 p2)
	{
		Row n(numeric);
		n.push(p1);
		n.push(p2);
		AppendLine(n);
	}

	template <typename T1, typename T2, typename T3>
	void AppendLine(unsigned int numeric, T1 p1, T2 p2, T3 p3)
	{
		Row n(numeric);
		n.push(p1);
		n.push(p2);
		n.push(p3);
		AppendLine(n);
	}

	template <typename T1, typename T2, typename T3, typename T4>
	void AppendLine(unsigned int numeric, T1 p1, T2 p2, T3 p3, T4 p4)
	{
		Row n(numeric);
		n.push(p1);
		n.push(p2);
		n.push(p3);
		n.push(p4);
		AppendLine(n);
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5>
	void AppendLine(unsigned int numeric, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5)
	{
		Row n(numeric);
		n.push(p1);
		n.push(p2);
		n.push(p3);
		n.push(p4);
		n.push(p5);
		AppendLine(n);
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
	void AppendLine(unsigned int numeric, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6)
	{
		Row n(numeric);
		n.push(p1);
		n.push(p2);
		n.push(p3);
		n.push(p4);
		n.push(p5);
		n.push(p6);
		AppendLine(n);
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
	void AppendLine(unsigned int numeric, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7)
	{
		Row n(numeric);
		n.push(p1);
		n.push(p2);
		n.push(p3);
		n.push(p4);
		n.push(p5);
		n.push(p6);
		n.push(p7);
		AppendLine(n);
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
	void AppendLine(unsigned int numeric, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8)
	{
		Row n(numeric);
		n.push(p1);
		n.push(p2);
		n.push(p3);
		n.push(p4);
		n.push(p5);
		n.push(p6);
		n.push(p7);
		n.push(p8);
		AppendLine(n);
	}
};
