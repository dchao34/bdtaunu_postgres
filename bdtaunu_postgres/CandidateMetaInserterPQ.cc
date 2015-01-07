#include <string>
#include <libpq-fe.h>
#include <bdtaunu_tuple_analyzer/BDtaunuDef.h>

#include "CandidateMetaInserterPQ.h"

using namespace std;

CandidateMetaInserterPQ::CandidateMetaInserterPQ(PGconn *conn) :
  TableInserterPQ(conn, "CandidateMeta", 3, { 0 }) {
}

void CandidateMetaInserterPQ::set_idx(int i) {
  _idx = to_string(i);
  _param_values[0] = _idx.c_str();
}

void CandidateMetaInserterPQ::set_reco_type(bdtaunu::CandType code) {
  _reco_type = to_string(static_cast<int>(code));
  _param_values[1] = _reco_type.c_str();
}
