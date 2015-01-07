#include <libpq-fe.h>

#include <bdtaunu_tuple_analyzer/BDtaunuDef.h>

#include "McCandidateInserterPQ.h"

using namespace std;
using namespace bdtaunu;

McCandidateInserterPQ::McCandidateInserterPQ(PGconn *conn) :
  CandidateInserterPQ(conn, "McCandidate", 31) {
}

void McCandidateInserterPQ::set_truth_match(int truth_match) {
  _truth_match = to_string(truth_match);
  _param_values[29] = _truth_match.c_str();
}
