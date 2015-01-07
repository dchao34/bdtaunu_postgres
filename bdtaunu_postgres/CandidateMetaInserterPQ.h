#ifndef _CANDIDATEMETAINSERTERPQ_H_
#define _CANDIDATEMETAINSERTERPQ_H_

#include <string>
#include <libpq-fe.h>
#include <bdtaunu_tuple_analyzer/BDtaunuDef.h>

#include "TableInserterPQ.h"

class CandidateMetaInserterPQ : public TableInserterPQ {
  public:
    CandidateMetaInserterPQ() = default;
    CandidateMetaInserterPQ(PGconn*);
    virtual ~CandidateMetaInserterPQ() = default;

    void set_idx(int i);
    void set_reco_type(bdtaunu::CandType code);

  private:
    std::string _idx;
    std::string _reco_type;
};

#endif
