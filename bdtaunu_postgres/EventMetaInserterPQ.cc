#include <string>
#include <libpq-fe.h>
#include <encode_meta_data/DataCodeDef.h>

#include "EventMetaInserterPQ.h"

using namespace std;

EventMetaInserterPQ::EventMetaInserterPQ(PGconn *conn) :
  TableInserterPQ(conn, "EventMeta", 8, { 0 }) {
}

void EventMetaInserterPQ::set_event_id(std::string event_id) {
  _event_id = event_id;
  _param_values[0] = _event_id.c_str();
}

void EventMetaInserterPQ::set_data_source(DataSourceCode code) {
  _data_source = to_string(static_cast<int>(code));
  _param_values[1] = _data_source.c_str();
}

void EventMetaInserterPQ::set_data_label(DataLabelCode code) {
  _data_label = to_string(static_cast<int>(code));
  _param_values[2] = _data_label.c_str();
}

void EventMetaInserterPQ::set_ml_sample(MLSampleCode code) {
  _ml_sample = to_string(static_cast<int>(code));
  _param_values[3] = _ml_sample.c_str();
}

void EventMetaInserterPQ::set_ml_sample(std::string ml_sample) {
  _ml_sample = ml_sample;
  _param_values[3] = _ml_sample.c_str();
}

void EventMetaInserterPQ::set_division(DivisionCode code) {
  _division = to_string(static_cast<int>(code));
  _param_values[4] = _division.c_str();
}

void EventMetaInserterPQ::set_division(std::string division) {
  _division = division;
  _param_values[4] = _division.c_str();
}

void EventMetaInserterPQ::set_run(RunCode code) {
  _run = to_string(static_cast<int>(code));
  _param_values[5] = _run.c_str();
}

void EventMetaInserterPQ::set_event_weight(double event_weight) {
  _event_weight = to_string(event_weight);
  _param_values[6] = _event_weight.c_str();
}
