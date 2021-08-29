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
#include "engine.h"
#include "maker.h"
#include "extras.h"
#include "managers/keys.h"

/* 
 * LHigh returns highest number in a list. 
 * Keep in mind that this function requires all list' elements
 * to be numeric.
 * 
 * @parameters:
 *
 *         · string	: List key to check.
 * 
 * @protocol:
 *
 *         · int	: Highest element found.
 */ 

class CommandLHigh : public Command 
{
    public: 

        CommandLHigh(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * LLow returns lowest number in a list.
 * 
 * @parameters:
 *
 *         · string	: List key to check.
 * 
 * @protocol:
 *
 *         · int	: Lowest element found.
 */ 

class CommandLLow : public Command 
{

    public: 

        CommandLLow(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * LAvg calculates the average of a given list.
 * 
 * @parameters:
 *
 *         · string	: List key to average.
 * 
 * @protocol:
 *
 *         · double	: Avg number.
 */ 

class CommandLAvg : public Command 
{
    public: 

        CommandLAvg(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * LPushNX adds an element to the list, only if has NOT
 * been previously defined.
 * 
 * @parameters:
 *
 *	   · string	: List name.
 *         · string	: Element to push.
 * 
 * @protocol:
 *
 *         · enum: OK or ERROR.
 */ 

class CommandLPushNX : public Command 
{
    public: 

        CommandLPushNX(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * LPush adds an element to the list, regardless if it has
 * been previously defined.
 * 
 * @parameters:
 *
 *	   · string	: List name to push.
 *         · string	: Element to push.
 * 
 * @protocol:
 *
 *         · enum: OK or ERROR.
 */ 

class CommandLPush : public Command 
{

    public: 

        CommandLPush(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * LGet returns all elements in a list.
 * 
 * @parameters:
 *
 *         · string	: Destination list.
 *         · { offset, limit }
 * 
 * @protocol:
 *
 *         · vector : Returning elements.
 */ 

class CommandLGet : public Command 
{
    public: 

        CommandLGet(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * LPopFront returns and removes the front (first) element in a
 * list.
 * 
 * @parameters:
 *
 *         · string	: List to check.
 * 
 * @protocol:
 *
 *         · string	: Element.
 */ 

class CommandLPopFront : public Command 
{
    public: 

        CommandLPopFront(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * LPopBack returns and removes the last element in a
 * list.
 * 
 * @parameters:
 *
 *         · string	: List to check.
 * 
 * @protocol:
 *
 *         · string	: Element.
 */ 

class CommandLPopBack : public Command 
{
    public: 

        CommandLPopBack(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * LPopAll removes all matching values in a list.
 * 
 * @parameters:
 *
 *         · string    : List name to lookup.
 *         · string    : Value to delete.
 * 
 * @protocol:
 *
 *         · enum     : OK, ERROR or NULL.
 */ 

class CommandPopAll : public Command 
{
    public: 

        CommandPopAll(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Checks if a given element is part of a list.
 * 
 * @parameters:
 *
 *         · string	: List key to find item on.
 *         · value	: Value to verify.
 * 
 * @protocol:
 *
 *         · enum	: 0 or 1
 */ 

class CommandLExist : public Command 
{
    public: 

        CommandLExist(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Counts elements in a list.
 * 
 * @parameters:
 *
 *         · string   : List to count items on.
 * 
 * @protocol:
 *
 *         · int      : Size of list.
 */ 
 
class CommandLCount : public Command 
{
    public: 

        CommandLCount(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * LResize resizes a list' length.
 * 
 * @parameters:
 *
 *         · string   : list to resize.
 *         · int      : New list's size.
 * 
 * @protocol:
 *
 *         · enum     : ERROR, OK or NULL.
 */ 

class CommandLResize : public Command 
{
    public: 

        CommandLResize(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * LKeys looks for all lists defined in the server.
 * This command allows wildcards.
 * 
 * @parameters:
 *
 *         · string   : Pattern to lookup.
 * 
 * @protocol:
 *
 *         · vector   : List founds.
 */ 

class CommandLKeys : public Command 
{
    public: 

        CommandLKeys(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * LFind finds items in a given list.
 * 
 * @parameters:
 *
 *         · string   : Destination list.
 *         · string   : string to lookup.
 * 
 * @protocol:
 *
 *         · vector   : Returning results.
 */ 

class CommandLFind : public Command 
{
    public: 

        CommandLFind(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * LReverse reverses the order of a given list.
 * 
 * @parameters:
 *
 *         · string   : List name.
 * 
 * @protocol:
 *
 *         · enum     : NULL, OK or ERROR.
 */ 

class CommandLReverse : public Command 
{
    public: 

        CommandLReverse(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * LSort sorts a list.
 * 
 * @parameters:
 *
 *         · string  : List name.
 * 
 * @protocol:
 *
 *         · enum    : NULL, OK or ERROR.
 */ 

class CommandLSort : public Command 
{
    public: 

        CommandLSort(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * LPos returns an element in a given position in a list.
 * 
 * @parameters:
 *
 *         · int    : Position to look for.
 * 
 * @protocol:
 *
 *         · string : Element found in provided position.
 */ 

class CommandLPos : public Command 
{
    public: 

        CommandLPos(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * LDel remove a given items from list.
 * 
 * @parameters:
 *
 *         · string    : List name to lookup.
 *         · string    : Value to delete.
 * 
 * @protocol:
 *
 *         · enum     : OK, ERROR or NULL.
 */ 

class CommandLDel : public Command 
{
    public: 

        CommandLDel(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * LRepeats counts everytime a given item repeats.
 * 
 * @parameters:
 *
 *         · string: List to check.
 * 
 * @protocol:
 *
 *         · int: Repeated items.
 */ 

class CommandLRepeats : public Command 
{
    public: 

        CommandLRepeats(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * LROP Returns last element and then removes it.
 * 
 * @parameters:
 *
 *         · string   : Key to process.
 * 
 * @protocol:
 *
 *         · enum     : ERROR or NULL.
 *         · string   : Front element.
 */ 

class CommandLRop : public Command 
{
    public: 

        CommandLRop(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * LFPOP Returns front element and then removes it.
 * 
 * @parameters:
 *
 *         · string   : Key to process.
 * 
 * @protocol:
 *
 *         · enum     : ERROR or NULL.
 *         · string   : Last element.
 */ 

class CommandFRop : public Command 
{
    public: 

        CommandFRop(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * LBack returns last element in a list.
 * 
 * @parameters:
 *
 *         · string   : Element to check.
 * 
 * @protocol:
 *
 *         · string   : Last element.
 */ 

class CommandLBack : public Command 
{
    public: 

        CommandLBack(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * LFront returns first element in a list.
 * 
 * @parameters:
 *
 *         · string   : Element to check.
 * 
 * @protocol:
 *
 *         · string   : First element.
 */ 

class CommandLFront : public Command 
{
    public: 

        CommandLFront(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

