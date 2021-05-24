
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

#include  <random>
#include  <iterator>

/* 
 * Returns a random item from provided template.
 * 
 * @parameters:
 *
 *         · Start: Starting iter item.
 *         · End: Ending iter item.
 * 
 * @return:
 *
 *         · Iter: Random Iter.
 */ 

template<typename Iter, typename R> Iter random_item(Iter start, Iter end, R& g) 
{
    std::uniform_int_distribution<> dis(0, std::distance(start, end) - 1);
    std::advance(start, dis(g));
    return start;
}

template<typename Iter> Iter random_item(Iter start, Iter end) 
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    return random_item(start, end, gen);
}