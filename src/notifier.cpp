
#include "beryl.h"
#include "notifier.h"

Notifier::Notifier()
{

}

bool NotifyStream::Add(User* user)
{
        if (!user || user->IsQuitting())
        {
            return false;
        }
        
        this->users.push_back(user);
        return true;
}

bool NotifyStream::Has(User* user)
{
        if (std::find(users.begin(), users.end(), user) != users.end())
        {
             return true;
        }
        
        return false;
}

void NotifyStream::Remove(User* user)
{

}

void Notifier::Create(const std::string& name)
{
  
}

void Notifier::AttachType(const std::string &type)
{

}


void Notifier::Flush()
{

}