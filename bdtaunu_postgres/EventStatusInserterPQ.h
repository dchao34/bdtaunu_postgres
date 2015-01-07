#ifndef _EVENTSTATUSINSERTERPQ_H_
#define _EVENTSTATUSINSERTERPQ_H_

#include <string>
#include <libpq-fe.h>
#include <bdtaunu_tuple_analyzer/RootReader.h>

#include "TableInserterPQ.h"

class EventStatusInserterPQ : public TableInserterPQ {
  public:
    EventStatusInserterPQ() = default;
    EventStatusInserterPQ(PGconn*);
    virtual ~EventStatusInserterPQ() = default;

    void set_reader_status(RootReader::Status status);

  private:
    std::string _max_reco_exceeded;
};

#endif
