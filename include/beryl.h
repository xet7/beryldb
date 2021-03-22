
#pragma once

extern Beryl* Nucleus;

class Beryl
{
    private:
    
        /* Current time, as updated in the mainloop. */

        struct timespec TIME;

    public:

        /*
         * Await time between loop intervals. This variable
         * is used to avoid bottlenecks.
         */

        unsigned int await_time;

        /* If set to true, then our await_time var cannot be changed. */

        bool Locked;
        
        /* Refreshes TIME, this function is called in every loop. */
                
        void TimeRefresh();

        
};