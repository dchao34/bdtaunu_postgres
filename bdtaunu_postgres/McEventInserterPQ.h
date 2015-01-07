#ifndef _MCEVENTINSERTERPQ_H_
#define _MCEVENTINSERTERPQ_H_

#include <string>
#include <libpq-fe.h>

#include <bdtaunu_tuple_analyzer/BDtaunuDef.h>

#include "EventInserterPQ.h"

class McEventInserterPQ : public EventInserterPQ {

  public:
    McEventInserterPQ() = default;
    McEventInserterPQ(PGconn*);
    virtual ~McEventInserterPQ() = default;

    void set_b1_mctype(bdtaunu::McBTypeCatalogue::BMcType b_mctype);
    void set_b2_mctype(bdtaunu::McBTypeCatalogue::BMcType b_mctype);
    void set_b1_tau_mctype(bdtaunu::TauMcType tau_mctype);
    void set_b2_tau_mctype(bdtaunu::TauMcType tau_mctype);

  private:
    std::string _b1_mctype;
    std::string _b2_mctype;
    std::string _b1_tau_mctype;
    std::string _b2_tau_mctype;
};

#endif
