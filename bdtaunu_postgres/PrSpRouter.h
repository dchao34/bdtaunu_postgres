#ifndef __PRSPROUTER_H__
#define __PRSPROUTER_H__

#include <string>

#include <libpq-fe.h>

#include <bdtaunu_tuple_analyzer/RootReader.h>
#include <bdtaunu_tuple_analyzer/BDtaunuReader.h>
#include <bdtaunu_tuple_analyzer/BDtaunuMcReader.h>
#include "EventInserterPQ.h"
#include "McEventInserterPQ.h"
#include "CandidateInserterPQ.h"
#include "McCandidateInserterPQ.h"

class PrSpRouter {
  public:
    static bool is_mc(int data_label) {
      switch (data_label) {
        case 0:
        case 1:
          return false;
          break;
        case 998:
        case 1005:
        case 1235:
        case 1237:
        case 11444:
        case 11445:
        case 11446:
        case 11447:
          return true;
          break;
      }
      throw "Invalid data label.";
    }

    static bool is_generic_mc(int data_label) {
      switch (data_label) {
        case 0:
        case 1:
        case 11444:
        case 11445:
        case 11446:
        case 11447:
          return false;
          break;
        case 998:
        case 1005:
        case 1235:
        case 1237:
          return true;
          break;
      }
      throw "Invalid data label.";
    }

    static bool is_signal_mc(int data_label) {
      switch (data_label) {
        case 0:
        case 1:
        case 998:
        case 1005:
        case 1235:
        case 1237:
          return false;
          break;
        case 11444:
        case 11445:
        case 11446:
        case 11447:
          return true;
          break;
      }
      throw "Invalid data label.";
    }

    static BDtaunuReader* CreateRootReader(std::string fname, int data_label) {
      if (!is_mc(data_label)) {
        return new BDtaunuReader(fname.c_str());
      } else {
        return new BDtaunuMcReader(fname.c_str());
      }
    }

    static EventInserterPQ* CreateEventInserter(PGconn *conn, int data_label) {
      if (!is_mc(data_label)) {
        return new EventInserterPQ(conn);
      } else {
        return new McEventInserterPQ(conn);
      }
    }

    static CandidateInserterPQ* CreateCandidateInserter(PGconn *conn, int data_label) {
      if (!is_mc(data_label)) {
        return new CandidateInserterPQ(conn);
      } else {
        return new McCandidateInserterPQ(conn);
      }
    }
};

#endif
