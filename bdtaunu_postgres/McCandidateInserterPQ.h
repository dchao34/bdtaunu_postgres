#ifndef _MCCANDIDATEINSERTERPQ_H_
#define _MCCANDIDATEINSERTERPQ_H_

#include <string>
#include <libpq-fe.h>

#include <bdtaunu_tuple_analyzer/BDtaunuDef.h>

#include "CandidateInserterPQ.h"

class McCandidateInserterPQ : public CandidateInserterPQ {

  public:
    McCandidateInserterPQ() = default;
    McCandidateInserterPQ(PGconn*);
    virtual ~McCandidateInserterPQ() = default;

    void set_truth_match(int);

  private:
    std::string _truth_match;
};

#endif
