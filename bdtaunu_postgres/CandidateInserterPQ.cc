#include <string>
#include <libpq-fe.h>
#include <bdtaunu_tuple_analyzer/BDtaunuDef.h>

#include "CandidateInserterPQ.h"

using namespace std;

CandidateInserterPQ::CandidateInserterPQ(PGconn *conn) :
  TableInserterPQ(conn, "Candidate", 30, { 0 }) {
}

CandidateInserterPQ::CandidateInserterPQ(
  PGconn *conn,
  const char *table_name,
  int n_columns) :
  TableInserterPQ(conn, table_name, n_columns, { 0 }) {
}

void CandidateInserterPQ::set_idx(int idx) {
  _idx = to_string(idx);
  _param_values[0] = _idx.c_str();
} 

void CandidateInserterPQ::set_mmiss_prime2(double mmiss_prime2) {
  _mmiss_prime2 = to_string(mmiss_prime2);
  _param_values[1] = _mmiss_prime2.c_str();
}

void CandidateInserterPQ::set_eextra50(double eextra50) {
  _eextra50 = to_string(eextra50);
  _param_values[2] = _eextra50.c_str();
}

void CandidateInserterPQ::set_cosThetaT(double cosThetaT) {
  _cosThetaT = to_string(cosThetaT);
  _param_values[3] = _cosThetaT.c_str();
}

void CandidateInserterPQ::set_tag_lp3(double tag_lp3) {
  _tag_lp3 = to_string(tag_lp3);
  _param_values[4] = _tag_lp3.c_str();
}

void CandidateInserterPQ::set_tag_cosBY(double tag_cosBY) {
  _tag_cosBY = to_string(tag_cosBY);
  _param_values[5] = _tag_cosBY.c_str();
}

void CandidateInserterPQ::set_tag_cosThetaDl(double tag_cosThetaDl) {
  _tag_cosThetaDl = to_string(tag_cosThetaDl);
  _param_values[6] = _tag_cosThetaDl.c_str();
}

void CandidateInserterPQ::set_tag_Dmass(double tag_Dmass) {
  _tag_Dmass = to_string(tag_Dmass);
  _param_values[7] = _tag_Dmass.c_str();
}

void CandidateInserterPQ::set_tag_deltaM(double tag_deltaM) {
  _tag_deltaM = to_string(tag_deltaM);
  _param_values[8] = _tag_deltaM.c_str();
}

void CandidateInserterPQ::set_tag_cosThetaDSoft(double tag_cosThetaDSoft) {
  _tag_cosThetaDSoft = to_string(tag_cosThetaDSoft);
  _param_values[9] = _tag_cosThetaDSoft.c_str();
}

void CandidateInserterPQ::set_tag_softP3MagCM(double tag_softP3MagCM) {
  _tag_softP3MagCM = to_string(tag_softP3MagCM);
  _param_values[10] = _tag_softP3MagCM.c_str();
}

void CandidateInserterPQ::set_tag_Dtype(bdtaunu::RecoDTypeCatalogue::DType tag_Dtype) {
  _tag_Dtype = to_string(static_cast<int>(tag_Dtype));
  _param_values[11] = _tag_Dtype.c_str();
}

void CandidateInserterPQ::set_tag_Dstartype(bdtaunu::RecoDTypeCatalogue::DstarType tag_Dstartype) {
  _tag_Dstartype = to_string(static_cast<int>(tag_Dstartype));
  _param_values[12] = _tag_Dstartype.c_str();
}

void CandidateInserterPQ::set_sig_hp3(double sig_hp3) {
  _sig_hp3 = to_string(sig_hp3);
  _param_values[13] = _sig_hp3.c_str();
}

void CandidateInserterPQ::set_sig_cosBY(double sig_cosBY) {
  _sig_cosBY = to_string(sig_cosBY);
  _param_values[14] = _sig_cosBY.c_str();
}

void CandidateInserterPQ::set_sig_cosThetaDtau(double sig_cosThetaDtau) {
  _sig_cosThetaDtau = to_string(sig_cosThetaDtau);
  _param_values[15] = _sig_cosThetaDtau.c_str();
}

void CandidateInserterPQ::set_sig_vtxB(double sig_vtxB) {
  _sig_vtxB = to_string(sig_vtxB);
  _param_values[16] = _sig_vtxB.c_str();
}

void CandidateInserterPQ::set_sig_Dmass(double sig_Dmass) {
  _sig_Dmass = to_string(sig_Dmass);
  _param_values[17] = _sig_Dmass.c_str();
}

void CandidateInserterPQ::set_sig_deltaM(double sig_deltaM) {
  _sig_deltaM = to_string(sig_deltaM);
  _param_values[18] = _sig_deltaM.c_str();
}

void CandidateInserterPQ::set_sig_cosThetaDSoft(double sig_cosThetaDSoft) {
  _sig_cosThetaDSoft = to_string(sig_cosThetaDSoft);
  _param_values[19] = _sig_cosThetaDSoft.c_str();
}

void CandidateInserterPQ::set_sig_softP3MagCM(double sig_softP3MagCM) {
  _sig_softP3MagCM = to_string(sig_softP3MagCM);
  _param_values[20] = _sig_softP3MagCM.c_str();
}

void CandidateInserterPQ::set_sig_hmass(double sig_hmass) {
  _sig_hmass = to_string(sig_hmass);
  _param_values[21] = _sig_hmass.c_str();
}

void CandidateInserterPQ::set_sig_vtxh(double sig_vtxh) {
  _sig_vtxh = to_string(sig_vtxh);
  _param_values[22] = _sig_vtxh.c_str();
}

void CandidateInserterPQ::set_sig_Dtype(bdtaunu::RecoDTypeCatalogue::DType sig_Dtype) {
  _sig_Dtype = to_string(static_cast<int>(sig_Dtype));
  _param_values[23] = _sig_Dtype.c_str();
}

void CandidateInserterPQ::set_sig_Dstartype(bdtaunu::RecoDTypeCatalogue::DstarType sig_Dstartype) {
  _sig_Dstartype = to_string(static_cast<int>(sig_Dstartype));
  _param_values[24] = _sig_Dstartype.c_str();
}

void CandidateInserterPQ::set_tag_l_ePid(int tag_l_ePid) {
  _tag_l_ePid = to_string(tag_l_ePid);
  _param_values[25] = _tag_l_ePid.c_str();
}

void CandidateInserterPQ::set_tag_l_muPid(int tag_l_muPid) {
  _tag_l_muPid = to_string(tag_l_muPid);
  _param_values[26] = _tag_l_muPid.c_str();
}

void CandidateInserterPQ::set_sig_h_ePid(int sig_h_ePid) {
  _sig_h_ePid = to_string(sig_h_ePid);
  _param_values[27] = _sig_h_ePid.c_str();
}

void CandidateInserterPQ::set_sig_h_muPid(int sig_h_muPid) {
  _sig_h_muPid = to_string(sig_h_muPid);
  _param_values[28] = _sig_h_muPid.c_str();
}
