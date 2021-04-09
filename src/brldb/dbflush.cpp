
#include "beryldb.h"
#include "brldb/database.h"
#include "brldb/datathread.h"
#include "brldb/dbnumeric.h"
#include "brldb/dbflush.h"
#include "algo.h"

std::mutex DataFlush::mute;

void DataFlush::AddResult(std::shared_ptr<query_base> signal)
{
      std::lock_guard<std::mutex> lg(DataFlush::mute);
      signal->user->notifications.push_back(signal);
}
