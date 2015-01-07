#ifndef _CANDIDATEINSERTERPQ_H_
#define _CANDIDATEINSERTERPQ_H_

#include <string>
#include <libpq-fe.h>
#include <bdtaunu_tuple_analyzer/BDtaunuDef.h>

#include "TableInserterPQ.h"

class CandidateInserterPQ : public TableInserterPQ {
  public:
    CandidateInserterPQ() = default;
    CandidateInserterPQ(PGconn*);
    CandidateInserterPQ(PGconn*, const char*, int);
    virtual ~CandidateInserterPQ() = default;

    void set_idx(int); 
    void set_mmiss_prime2(double);
    void set_eextra50(double);
    void set_cosThetaT(double);
    void set_tag_lp3(double);
    void set_tag_cosBY(double);
    void set_tag_cosThetaDl(double);
    void set_tag_Dmass(double);
    void set_tag_deltaM(double);
    void set_tag_cosThetaDSoft(double);
    void set_tag_softP3MagCM(double);
    void set_tag_Dtype(bdtaunu::RecoDTypeCatalogue::DType);
    void set_tag_Dstartype(bdtaunu::RecoDTypeCatalogue::DstarType);
    void set_sig_hp3(double);
    void set_sig_cosBY(double);
    void set_sig_cosThetaDtau(double);
    void set_sig_vtxB(double);
    void set_sig_Dmass(double);
    void set_sig_deltaM(double);
    void set_sig_cosThetaDSoft(double);
    void set_sig_softP3MagCM(double);
    void set_sig_hmass(double);
    void set_sig_vtxh(double);
    void set_sig_Dtype(bdtaunu::RecoDTypeCatalogue::DType);
    void set_sig_Dstartype(bdtaunu::RecoDTypeCatalogue::DstarType);
    void set_tag_l_ePid(int);
    void set_tag_l_muPid(int);
    void set_sig_h_ePid(int);
    void set_sig_h_muPid(int);

  private:

    std::string _idx; 
    std::string _mmiss_prime2;
    std::string _eextra50;
    std::string _cosThetaT;
    std::string _tag_lp3;
    std::string _tag_cosBY;
    std::string _tag_cosThetaDl;
    std::string _tag_Dmass;
    std::string _tag_deltaM;
    std::string _tag_cosThetaDSoft;
    std::string _tag_softP3MagCM;
    std::string _tag_Dtype;
    std::string _tag_Dstartype;
    std::string _sig_hp3;
    std::string _sig_cosBY;
    std::string _sig_cosThetaDtau;
    std::string _sig_vtxB;
    std::string _sig_Dmass;
    std::string _sig_deltaM;
    std::string _sig_cosThetaDSoft;
    std::string _sig_softP3MagCM;
    std::string _sig_hmass;
    std::string _sig_vtxh;
    std::string _sig_Dtype;
    std::string _sig_Dstartype;
    std::string _tag_l_ePid;
    std::string _tag_l_muPid;
    std::string _sig_h_ePid;
    std::string _sig_h_muPid;
};

#endif
