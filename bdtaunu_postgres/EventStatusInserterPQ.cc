#include <bdtaunu_tuple_analyzer/RootReader.h>

#include "EventStatusInserterPQ.h"

using namespace std;

EventStatusInserterPQ::EventStatusInserterPQ(PGconn *conn) :
  TableInserterPQ(conn, "EventStatus", 2, { 0 }) {
}

void EventStatusInserterPQ::set_reader_status(RootReader::Status status) {
  _max_reco_exceeded = to_string(static_cast<int>(status));
  _param_values[0] = _max_reco_exceeded.c_str();
}
