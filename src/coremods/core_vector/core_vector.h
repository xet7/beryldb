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
#include "maker.h"
#include "engine.h"
#include "extras.h"

#include "managers/keys.h"

/* 
 * VLow returns lowest number in a vector.
 * 
 * @parameters:
 *
 *         · string   : Vector's key to check.
 * 
 * @protocol:
 *
 *         · int      : Lowest element found.
 */ 

class CommandVLow : public Command 
{
    public: 

        CommandVLow(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * VHigh returns highest number in a vector.
 * 
 * @parameters:
 *
 *         · string   : Vector's key to check.
 * 
 * @protocol:
 *
 *         · int      : Highest element found.
 */ 

class CommandVHigh : public Command 
{
    public: 

        CommandVHigh(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * VGet obtains all elements in a vector.
 * 
 * @parameters:
 *
 *         · string   : Destination vector.
 *         · { offset, limit }
 * 
 * @protocol:
 *
 *         · vector   : Returning elements.

 */ 

class CommandVGet : public Command 
{
    public: 

        CommandVGet(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * VPush pushes a new elemento a given vector.
 * 
 * @parameters:
 *
 *         · string     : Destination vector.
 *         · string     : Vector to add new element into.
 * 
 * @protocol:
 *
 *         · enum	: OK or ERROR.
 */ 

class CommandVPush : public Command 
{

    public: 

        CommandVPush(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * VExist verifies whether an element exists.
 * 
 * @parameters:
 *
 *         · string	: Destination vector.
 *         · string	: Element to check.
 * 
 * @protocol:
 *
 *         · enum	: OK or ERROR.
 */ 

class CommandVExist : public Command 
{
    public: 

        CommandVExist(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * VCount counts all elements in a vector. 
 * 
 * @parameters:
 *
 *         · string	: Vector's key to check.
 * 
 * @protocol:
 *
 *         · int	: Elements found.
 */ 
 
class CommandVCount : public Command 
{
    public: 

        CommandVCount(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * VPos returns an element in a given position in a vector.
 * 
 * @parameters:
 *
 *         · int	: Position to look for.
 * 
 * @protocol:
 *
 *         · string	: Element found in provided position.
 */ 

class CommandVPos : public Command 
{
    public: 

        CommandVPos(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * VResize resizes the size of a vector.
 * 
 * @parameters:
 *
 *         · string	: Vector key.
 *         · int	: New size of vector.
 * 
 * @protocol:
 *
 *         · enum	: NULL, OK or ERROR.
 */ 
 
class CommandVResize : public Command 
{
    public: 

        CommandVResize(Module* Creator);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * VKeys looks for all geo-keys defined in the server.
 * This command allows wildcards.
 * 
 * @parameters:
 *
 *         · string   : Pattern to lookup.
 * 
 * @protocol:
 *
 *         · vector    : Geo Keys found.
 */ 

class CommandVKeys : public Command 
{
    public: 

        CommandVKeys(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * VPopFront removes and obtains first element in a vector.
 * 
 * @parameters:
 *
 *         · string    : Vector to look for.
 * 
 * @protocol:
 *
 *         · string    : Element.
 */ 

class CommandVPopFront : public Command 
{
    public: 

        CommandVPopFront(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * VPopBack removes and obtains last element in a vector.
 * 
 * @parameters:
 *
 *         · string    : Vector to look for.
 * 
 * @protocol:
 *
 *         · string    : Element.
 */ 

class CommandVPopBack : public Command 
{
    public: 

        CommandVPopBack(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * VDel remove a given items from vector.
 * 
 * @parameters:
 *
 *         · string    : vector name to lookup.
 *         · string    : Value to delete.
 * 
 * @protocol:
 *
 *         · enum      : OK, ERROR or NULL.
 */ 

class CommandVDel : public Command 
{
    public: 

        CommandVDel(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* VReverse reverses a given vector.
 * 
 * @parameters:
 *
 *         · string    : Destination vector.
 * 
 * @protocol:
 *
 *         · enum      : NULL, OK or ERROR.
 */ 

class CommandVReverse : public Command 
{
    public: 

        CommandVReverse(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * VRepeats finds repeats in a given vecot.r
 *
 * @parameters:
 *
 *         · string	: Destination vector.
 *         · string	: Value to find.
 * 
 * @protocol:
 *
 *         · int	: Repeated items.
 */ 

class CommandVRepeats : public Command 
{
    public: 

        CommandVRepeats(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * VSorts sorts a vector.
 * 
 * @parameters:
 *
 *         · string	: Destination vector.
 * 
 * @protocol:
 *
 *         · enum	: NULL, OK or ERROR.
 */ 

class CommandVSort : public Command 
{
    public: 

        CommandVSort(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * VAvg obtains the average value of a vector.
 * 
 * @parameters:
 *
 *         · string	: Destination vector,
 * 
 * @protocol:
 *
 *         · double	: Average value.
 */
 
class CommandVAvg : public Command 
{
    public: 

        CommandVAvg(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * VSum sums all items in a vector.
 * 
 * @parameters:
 *
 *         · string	: Destination vector.
 * 
 * @protocol:
 *
 *         · double	: Summed nums.
 */
 
class CommandVSum : public Command 
{
    public: 

        CommandVSum(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * VPushNX adds a new element to a vector, only if not previously defined.
 * 
 * @parameters:
 *
 *         · string	: Destination vector.
 * 
 * @protocol:
 *
 *         · enum	: OK or ERROR.
 */

class CommandVPushNX : public Command 
{
    public: 

        CommandVPushNX(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * VFind finds items in a given vector.
 * 
 * @parameters:
 *
 *         · string	: Destination vector.
 *         · string	: String to lookup.
 * 
 * @protocol:
 *
 *         · vector	: Returning results.
 */ 

class CommandVFind : public Command 
{
    public: 

        CommandVFind(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * VFront returns the front (first) element in a vector.
 * 
 * @parameters:
 *
 *         · string	: Destination vector.
 * 
 * @protocol:
 *
 *         · enum	: OK or ERROR.
 */ 

class CommandVFront : public Command 
{
    public: 

        CommandVFront(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * VBack returns last element in a vector.
 * 
 * @parameters:
 *
 *         · string	: Destination vector.
 * 
 * @protocol:
 *
 *         · enum	: OK or ERROR.
 */ 
 
class CommandVBack : public Command 
{
    public: 

        CommandVBack(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};
