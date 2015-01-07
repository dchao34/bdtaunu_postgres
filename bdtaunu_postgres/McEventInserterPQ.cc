#include <libpq-fe.h>

#include <bdtaunu_tuple_analyzer/BDtaunuDef.h>

#include "McEventInserterPQ.h"

using namespace std;
using namespace bdtaunu;

McEventInserterPQ::McEventInserterPQ(PGconn *conn) :
  EventInserterPQ(conn, "McEvent", 8) {
}

void McEventInserterPQ::set_b1_mctype(McBTypeCatalogue::BMcType b_mctype) {
  _b1_mctype = to_string(static_cast<int>(b_mctype));
  _param_values[3] = _b1_mctype.c_str();
}

void McEventInserterPQ::set_b2_mctype(McBTypeCatalogue::BMcType b_mctype) {
  _b2_mctype = to_string(static_cast<int>(b_mctype));
  _param_values[4] = _b2_mctype.c_str();
}

void McEventInserterPQ::set_b1_tau_mctype(TauMcType tau_mctype) {
  _b1_tau_mctype = to_string(static_cast<int>(tau_mctype));
  _param_values[5] = _b1_tau_mctype.c_str();
}

void McEventInserterPQ::set_b2_tau_mctype(TauMcType tau_mctype) {
  _b2_tau_mctype = to_string(static_cast<int>(tau_mctype));
  _param_values[6] = _b2_tau_mctype.c_str();
}
