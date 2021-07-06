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

#include <math.h>

#define earth_radius_km 6371.0

/* 
 * Degrees to radians.
 * 
 * @parameters:
 *
 *         · deg: Degrees
 * 
 * @return:
 *
 *         · double: Calculation.
 */ 

double deg2rad(double deg) 
{
       return (deg * M_PI / 180);
}

/* 
 * Radians to degrees.
 * 
 * @parameters:
 *
 *         · rad: Radians.
 * 
 * @return:
 *
 *         · double: Calculation.
 */ 

double rad2deg(double rad) 
{
       return (rad * 180 / M_PI);
}

/* 
 * Calculates distance between two geopoints.
 * 
 * @parameters:
 *
 *         · lat1d: Latitude 1.
 *         · lon1d: Longitude 1.
 *           ....
 * 
 * @return:
 *
 *         · double: Calculation.
 */ 

double CalculateDistance(double lat1d, double lon1d, double lat2d, double lon2d) 
{
       double lat1r, lon1r, lat2r, lon2r, u, v;
       lat1r = deg2rad(lat1d);
       lon1r = deg2rad(lon1d);
       lat2r = deg2rad(lat2d);
       lon2r = deg2rad(lon2d);
       u = sin((lat2r - lat1r)/2);
       v = sin((lon2r - lon1r)/2);
       return 2.0 * earth_radius_km * asin(sqrt(u * u + cos(lat1r) * cos(lat2r) * v * v));
}

