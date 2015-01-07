#ifndef _EVENTINSERTERPQ_H_
#define _EVENTINSERTERPQ_H_

#include <string>
#include <libpq-fe.h>

#include "TableInserterPQ.h"

class EventInserterPQ : public TableInserterPQ {
  public:
    EventInserterPQ() = default;
    EventInserterPQ(PGconn*);
    EventInserterPQ(PGconn*, const char*, int);
    virtual ~EventInserterPQ() = default;

    void set_nY(int nY);
    void set_nTrk(int nTrk);
    void set_R2(double R2);

  private:
    std::string _nY;
    std::string _nTrk;
    std::string _R2;
};

#endif
