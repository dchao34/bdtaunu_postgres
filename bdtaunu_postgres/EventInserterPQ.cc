#include "EventInserterPQ.h"

using namespace std;

EventInserterPQ::EventInserterPQ(PGconn *conn) :
  TableInserterPQ(conn, "Event", 4, { 0 }) {
}

EventInserterPQ::EventInserterPQ(
  PGconn *conn, 
  const char *table_name, 
  int n_columns) :
  TableInserterPQ(conn, table_name, n_columns, { 0 }) {
}

void EventInserterPQ::set_nY(int nY) {
  _nY = to_string(nY);
  _param_values[0] = _nY.c_str();
}

void EventInserterPQ::set_nTrk(int nTrk) {
  _nTrk = to_string(nTrk);
  _param_values[1] = _nTrk.c_str();
}

void EventInserterPQ::set_R2(double R2) {
  _R2 = to_string(R2);
  _param_values[2] = _R2.c_str();
}
