#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include <dirent.h>

#include <encode_meta_data/TableCache.h>
#include <encode_meta_data/DataCoder.h>
#include <encode_meta_data/AssignmentCsvWriter.h>

#include <bdtaunu_tuple_analyzer/BDtaunuMcReader.h>

#include <custom_cpp_utilities/CsvReader.h>

#include <libpq-fe.h>
#include <bdtaunu_postgres/pq_wrappers.h>
#include <bdtaunu_postgres/EventMetaInserterPQ.h>
#include <bdtaunu_postgres/EventStatusInserterPQ.h>
#include <bdtaunu_postgres/McEventInserterPQ.h>
#include <bdtaunu_postgres/CandidateMetaInserterPQ.h>
#include <bdtaunu_postgres/McCandidateInserterPQ.h>
#include <bdtaunu_postgres/PrSpRouter.h>

using namespace std;
using namespace custom_cpp_utilities;

//! Exit nicely and print a usage message
static void exit_nicely() {
  cout << "Usage: ./insert_postgres ";
  cout << "INPUT_BASE_DIR DB_NAME DATA_LABEL RUN" << endl;
  exit(EXIT_FAILURE);
}

// Get all file names in an ntp directory
int get_fnames(vector<string> &fname_list, string dirname) {

  DIR *dir;
  struct dirent *ent;

  // open directory
  if ((dir = opendir(dirname.c_str())) == NULL) return 1;

  // print all files and directories in the given folder. 
  while ((ent = readdir(dir)) != NULL) {
    string fname(ent->d_name);
    if (fname == "." || fname == "..") continue;
    fname_list.push_back(dirname + "/" + fname);
  }
  closedir(dir);
  return 0;
}

//! Parse command line
static void parse_command_line(
    int argc, char **argv, 
    vector<string> &fname_list, 
    string &dbname,
    int &data_label, int &run) {

  if (argc != 5) exit_nicely();

  string base_dirname = string(argv[1]);
  dbname = string(argv[2]);
  data_label = atoi(argv[3]);
  run = atoi(argv[4]);

  string prefix = "pr";
  if (PrSpRouter::is_mc(data_label)) prefix = "sp";
  string data_base = prefix + to_string(data_label) + "r" + to_string(run);
  string dirname = base_dirname + "/" + data_base + "/" + "root";

  if (get_fnames(fname_list, dirname)) {
    perror(""); 
    exit_nicely();
  } 
}

int main(int argc, char **argv) {

  // Command line parsing
  // --------------------

  // Parse command line and get a list of ntp to process
  vector<string> fname_list;
  string dbname;
  int data_label, run;
  parse_command_line(argc, argv, fname_list, dbname, data_label, run);

  // Setup 
  // -----

  // Connect to the database
  PGconn *conn = Connectdb(("dbname=" + dbname).c_str());
  Exec(conn, "BEGIN");

  // Construct table inserters
  EventMetaInserterPQ *eventmeta_inserter = new EventMetaInserterPQ(conn);
  EventStatusInserterPQ *eventstatus_inserter = new EventStatusInserterPQ(conn);
  EventInserterPQ *event_inserter = PrSpRouter::CreateEventInserter(conn, data_label);
  CandidateMetaInserterPQ *candidatemeta_inserter = new CandidateMetaInserterPQ(conn);
  CandidateInserterPQ *candidate_inserter = PrSpRouter::CreateCandidateInserter(conn, data_label);

  // Cache pre-assigned event ID's as well as event weights.
  TableCache cache;
  if (PrSpRouter::is_signal_mc(data_label)) {
    cache.cache_events(__CACHED_SIGMC_PATHNAME);
  } else if (PrSpRouter::is_generic_mc(data_label)) {
    cache.cache_events(__CACHED_GENERIC_PATHNAME);
  }
  cache.cache_event_weights(__CACHED_EVENTWEIGHTS_PATHNAME);

  // Construct a DataCoder to encode newly encountered events.
  DataCoder coder;

  // Database insertion
  // ------------------

  for (auto fname : fname_list) {

    cout << "."; cout.flush();

    BDtaunuReader *reader = PrSpRouter::CreateRootReader(fname.c_str(), data_label);
    RootReader::Status s;
    while ((s = reader->next_record()) != RootReader::Status::kEOF) {

      // EventMeta
      string event_id = reader->get_eventId();

      eventmeta_inserter->set_event_id(event_id);
      if (cache.is_cached_event(event_id)) {
        eventmeta_inserter->set_ml_sample(cache.get_ml_sample(event_id));
        eventmeta_inserter->set_division(cache.get_division(event_id));
      } else {
        eventmeta_inserter->set_ml_sample(coder.AssignMLSample(data_label));
        eventmeta_inserter->set_division(coder.AssignDivision());
      }
      eventmeta_inserter->set_data_source(coder.CodeDataSource(data_label));
      eventmeta_inserter->set_data_label(coder.CodeDataLabel(data_label));
      eventmeta_inserter->set_run(coder.CodeRun(run));
      eventmeta_inserter->set_event_weight(cache.get_event_weight(data_label, run));

      eventmeta_inserter->exec_stmt();

      // EventStatus
      eventstatus_inserter->set_reader_status(s);
      eventstatus_inserter->exec_stmt();

      // Event
      event_inserter->set_nY(reader->get_nY());
      event_inserter->set_nTrk(reader->get_nTrk());
      event_inserter->set_R2(reader->get_R2All());

      McEventInserterPQ *mcevent_inserter = nullptr;
      BDtaunuMcReader *mcreader = nullptr;
      if (
           (mcevent_inserter = dynamic_cast<McEventInserterPQ*>(event_inserter)) &&
           (mcreader = dynamic_cast<BDtaunuMcReader*>(reader))
         ) {
           mcevent_inserter->set_b1_mctype(mcreader->get_b1_mctype());
           mcevent_inserter->set_b2_mctype(mcreader->get_b2_mctype());
           mcevent_inserter->set_b1_tau_mctype(mcreader->get_b1_tau_mctype());
           mcevent_inserter->set_b2_tau_mctype(mcreader->get_b2_tau_mctype());
      }

      event_inserter->exec_stmt();

      // CandidateMeta
      auto Ycand = reader->get_upsilon_candidates();
      for (unsigned i = 0; i < Ycand.size(); i++) {
        candidatemeta_inserter->set_idx(i);
        candidatemeta_inserter->set_reco_type(Ycand[i].get_cand_type());
        candidatemeta_inserter->exec_stmt();
      }

      // Candidate
      for (unsigned i = 0; i < Ycand.size(); i++) {

        candidate_inserter->set_idx(i); 
        candidate_inserter->set_mmiss_prime2(Ycand[i].get_mmiss_prime2());
        candidate_inserter->set_eextra50(Ycand[i].get_eextra50());
        candidate_inserter->set_cosThetaT(Ycand[i].get_cosThetaT());
        candidate_inserter->set_tag_lp3(Ycand[i].get_tag_lp3());
        candidate_inserter->set_tag_cosBY(Ycand[i].get_tag_cosBY());
        candidate_inserter->set_tag_cosThetaDl(Ycand[i].get_tag_cosThetaDl());
        candidate_inserter->set_tag_Dmass(Ycand[i].get_tag_Dmass());
        candidate_inserter->set_tag_deltaM(Ycand[i].get_tag_deltaM());
        candidate_inserter->set_tag_cosThetaDSoft(Ycand[i].get_tag_cosThetaDSoft());
        candidate_inserter->set_tag_softP3MagCM(Ycand[i].get_tag_softP3MagCM());
        candidate_inserter->set_tag_Dtype(Ycand[i].get_tag_d_mode());
        candidate_inserter->set_tag_Dstartype(Ycand[i].get_tag_dstar_mode());
        candidate_inserter->set_sig_hp3(Ycand[i].get_sig_hp3());
        candidate_inserter->set_sig_cosBY(Ycand[i].get_sig_cosBY());
        candidate_inserter->set_sig_cosThetaDtau(Ycand[i].get_sig_cosThetaDtau());
        candidate_inserter->set_sig_vtxB(Ycand[i].get_sig_vtxB());
        candidate_inserter->set_sig_Dmass(Ycand[i].get_sig_Dmass());
        candidate_inserter->set_sig_deltaM(Ycand[i].get_sig_deltaM());
        candidate_inserter->set_sig_cosThetaDSoft(Ycand[i].get_sig_cosThetaDSoft());
        candidate_inserter->set_sig_softP3MagCM(Ycand[i].get_sig_softP3MagCM());
        candidate_inserter->set_sig_hmass(Ycand[i].get_sig_hmass());
        candidate_inserter->set_sig_vtxh(Ycand[i].get_sig_vtxh());
        candidate_inserter->set_sig_Dtype(Ycand[i].get_sig_d_mode());
        candidate_inserter->set_sig_Dstartype(Ycand[i].get_sig_dstar_mode());
        candidate_inserter->set_tag_l_ePid(Ycand[i].get_l_ePidMap());
        candidate_inserter->set_tag_l_muPid(Ycand[i].get_l_muPidMap());
        candidate_inserter->set_sig_h_ePid(Ycand[i].get_h_ePidMap());
        candidate_inserter->set_sig_h_muPid(Ycand[i].get_h_muPidMap());


        if (McCandidateInserterPQ *mccandidate_inserter = dynamic_cast<McCandidateInserterPQ*>(candidate_inserter)) {
          mccandidate_inserter->set_truth_match(Ycand[i].get_truth_match());
        }

        candidate_inserter->exec_stmt();
      }

    }
    delete reader;
  }

  // Cleanup
  // -------

  // Free objects
  delete eventmeta_inserter;
  delete eventstatus_inserter;
  delete event_inserter;
  delete candidatemeta_inserter;
  delete candidate_inserter;

  // Close connection to database
  Exec(conn, "COMMIT");
  PQfinish(conn);

  return 0;
}
