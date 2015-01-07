#ifndef _EVENTMETAINSERTERPQ_H_
#define _EVENTMETAINSERTERPQ_H_

#include <string>
#include <libpq-fe.h>
#include <encode_meta_data/DataCodeDef.h>

#include "TableInserterPQ.h"

class EventMetaInserterPQ : public TableInserterPQ {
  public:
    EventMetaInserterPQ() = default;
    EventMetaInserterPQ(PGconn*);
    virtual ~EventMetaInserterPQ() = default;

    void set_event_id(std::string);
    void set_data_source(DataSourceCode code);
    void set_data_label(DataLabelCode code);
    void set_ml_sample(MLSampleCode code);
    void set_ml_sample(std::string code);
    void set_division(DivisionCode code);
    void set_division(std::string code);
    void set_run(RunCode code);
    void set_event_weight(double event_weight);

  private:
    std::string _event_id;
    std::string _data_source;
    std::string _data_label;
    std::string _ml_sample;
    std::string _division;
    std::string _run;
    std::string _event_weight;
};

#endif
