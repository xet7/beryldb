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

#ifdef ENABLE_TESTOFFICE

/*
 * This class is meant to be used for testing purposes.
 * Developers should implement their own functions inside 
 * Run();
 *
 * NOTE: This class is not meant to be used in a production setting.
 */
 
class TestOffice
{
   public:

        TestOffice();

        ~TestOffice();
        
        void Run();
};

#endif

