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
#include "extras.h"
#include "engine.h"

#include "managers/globals.h"
#include "managers/keys.h"

/* 
 * Adds a new geo-key.
 * 
 * @parameters:
 *
 *         · name: Name to this geo key.
 *         · latitude: Valid latitude.
 *         · longitude: Valid longitude.
 * 
 * @protocol:
 *
 *         · enum: OK or ERROR.
 */ 

class CommandGeoAdd : public Command 
{
    public: 

        CommandGeoAdd(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Adds a new geo-key, only if geokey has not been
 * previously defined.
 * 
 * @parameters:
 *
 *         · string      :  Name to this geo key.
 *         · string      :  Valid latitude.
 *         · string      :  Valid longitude.
 * 
 * @protocol:
 *
 *         · enum: OK or ERROR.
 */ 

class CommandGeoAddNX : public Command 
{
    public: 

        CommandGeoAddNX(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Adds a new geo-key and then publishes it to a channel.
 * 
 * @parameters:
 *
 *         · string      :  Name to this geo key.
 *         · string      :  Channel to display info on.
 *         · string      :  Valid latitude.
 *         · string      :  Valid longitude.
 * 
 * @protocol:
 *
 *         · enum: OK or ERROR.
 */ 
 
class CommandGeoAddPub : public Command 
{
    public: 

        CommandGeoAddPub(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Gets a geokey.
 * 
 * @parameters:
 *
 *         · string     : Destination geokey.
 * 
 * @protocol:
 *
 *         · string     : Geo entry as in a { latitude, longitude } format.
 */ 

class CommandGeoGet : public Command 
{
    public: 

        CommandGeoGet(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * GFind looksup for all geo-entries in the server.
 * This command allows wildcards.
 * 
 * @parameters:
 *
 *         · string     : Multimap wildcard.
 *         · { offset, limit }
 * 
 * @protocol:
 *
 *         · enum       : ERROR.
 *         · vector     : All entries found.
 */ 
 
class CommandGFind : public Command 
{
    public: 

        CommandGFind(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Calculates distance between two defined entries, as expressed in kilometers.
 * 
 * @parameters:
 *
 *         · string     : Point a.
 *         · string     : Point b.
 * 
 * @protocol:
 *
 *         · enum       : NULL or ERROR.
 *         · double	: Distance calculated.
 */ 
 
class CommandGeoCalc : public Command 
{
    public: 

        CommandGeoCalc(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};


class CommandGeoDistance : public Command 
{
    public: 

        CommandGeoDistance(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

class CommandGeoRemove : public Command 
{
    public: 

        CommandGeoRemove(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Returns the longitude from a geo key.
 * 
 * @parameters:
 *
 *         · string   : Geo Key to retrieve data from.
 * 
 * @protocol:
 *
 *         · enum    : NULL, ERROR or OK.
 *         · string  : Longitude.
 */ 
 
class CommandGeoLoGet : public Command 
{
    public: 

        CommandGeoLoGet(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

/* 
 * Returns the latitude from a geo key.
 * 
 * @parameters:
 *
 *         · string   : Geo Key to retrieve data from.
 * 
 * @protocol:
 *
 *         · enum    : NULL, ERROR or OK.
 *         · string  : Latitude.
 */ 
 
class CommandGeoLaGet : public Command 
{
    public: 

        CommandGeoLaGet(Module* parent);

        COMMAND_RESULT Handle(User* user, const Params& parameters);
};

