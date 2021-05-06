
#pragma once

/* Only static functions. */

class Externalize Helpers
{
    public:
    
       static bool as_bool(const std::string &key, bool def = false);
       
       static std::string to_bool(bool boolean);
           
};