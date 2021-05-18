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

class Module;

/* 
 * This timer class will run timers that had been added through the
 * TickManager class. In order to create a timer, you must
 * inherit a Timer class. 
 */
 
class Externalize Timer
{ 
    private:
		
	/* Time at which a given timer is scheduled to be executed. */
		
	time_t schedule;

	/* Secs between intervals. */
	
	unsigned int secs;

	/* Set to true when a timer is repeating. */
		
	bool repeat;

   public:

	/* Constructs a timer. */
		
	Timer(unsigned int add_secs, bool repeating = false);

	/* Destructor. */
	
	virtual ~Timer();

	time_t GetSchedule() const
	{
		return this->schedule;
	}


	void SetSchedule(time_t nextschedule)
	{
		schedule = nextschedule;
	}

	void SetNextTick(unsigned int tick);

	/* 
	 * Run() ticks a timer. This function must return
	 * a false or true depending whether you want it to be run again.
	 */

	 virtual bool Run(time_t TIME) = 0;

	 /* Set to true when a timer is meant to repeat. */
	 
	 bool GetRepeat() const
	 {
		return repeat;
	 }
		
 	 unsigned int as_interval() const
 	 {
		return secs;
	 }

	/* Stops repeating a timer. */
	
	void StopRepeating()
	{
		repeat = false;
	}
};

class Externalize TickManager : public safecast<TickManager>
{
  private:
  
	typedef std::multimap<time_t,  std::shared_ptr<Timer>> TimerMap;

	/* Pending timers. */
	
	TimerMap Timers;

  public:
	
	/* Flushes all pending timers. */
	
	void Flush(time_t TIME);

	/* Adds a new timer */
	
	void Add(std::shared_ptr<Timer> timer);

	/* Removes a timer. */
	
	void Remove(std::shared_ptr<Timer> timer);
};
