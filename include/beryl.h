
#pragma once

extern Beryl* Nucleus;

class Beryl
{
    public:

        /*
         * Await time between loop intervals. This variable
         * is used to avoid bottlenecks.
         */

        unsigned int await_time;

        /* If set to true, then our await_time var cannot be changed. */

        bool Locked;
        
        
};