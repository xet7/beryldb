
#include "beryl.h"
#include "database.h"
#include "exit.h"

Database::Database(time_t dbcreated, const std::string& dbname, const std::string& dbpath) : created(dbcreated), name(dbname), path(Nucleus->Config->Paths.PrependDB(dbpath))
{

}

void Database::Close()
{
        slog("DATABASE", LOG_DEFAULT, "Closing database: %s.", this->path.c_str());

        if (!this->db)
        {
            return;
        }

        this->db = NULL;
        delete this->db;
}
