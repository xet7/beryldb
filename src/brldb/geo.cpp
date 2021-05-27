
#include "beryl.h"
#include "brldb/database.h"
#include "brldb/query.h"
#include "brldb/dbnumeric.h"


void geo_add::Run()
{
    if (this->key.empty())
    {
            this->access_set(DBL_MISS_ARGS);
            return;
    }

//      rocksdb::Status fstatus2 =  this->database->GetAddress()->Put(rocksdb::WriteOptions(), where, entry_value);
      this->SetOK();
}

