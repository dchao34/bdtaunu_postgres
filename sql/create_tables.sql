-- Schema definitions for the postgreSQL database used in the bdtaunu analysis.

CREATE SEQUENCE eid INCREMENT BY 1 MINVALUE 0;

CREATE TABLE EventMeta (
  eid bigint PRIMARY KEY DEFAULT nextval('eid'),
  event_id text UNIQUE NOT NULL,
  data_source smallint NOT NULL, 
  data_label integer NOT NULL, 
  ml_sample smallint NOT NULL,
  division smallint NOT NULL,
  run smallint NOT NULL,
  event_weight double precision NOT NULL
);

CREATE TABLE EventStatus (
  eid bigint PRIMARY KEY REFERENCES EventMeta (eid) ON UPDATE CASCADE ON DELETE CASCADE DEFAULT currval('eid'),
  max_reco_exceeded integer NOT NULL
);

CREATE TABLE Event (
  eid bigint PRIMARY KEY REFERENCES EventMeta (eid) ON UPDATE CASCADE ON DELETE CASCADE DEFAULT currval('eid'),
  nY integer NOT NULL,
  nTrk integer NOT NULL, 
  R2 real NOT NULL
);

CREATE TABLE McEvent (
  b1_mctype integer NOT NULL,
  b2_mctype integer NOT NULL,
  b1_tau_mctype integer NOT NULL,
  b2_tau_mctype integer NOT NULL
) INHERITS (Event);

CREATE TABLE CandidateMeta (
  eid bigint REFERENCES EventMeta (eid) ON UPDATE CASCADE ON DELETE CASCADE DEFAULT currval('eid'),
  idx integer NOT NULL,
  reco_type integer NOT NULL,
  PRIMARY KEY (eid, idx)
);

CREATE TABLE Candidate (
  eid bigint DEFAULT currval('eid'),
  idx integer,
  mmiss_prime2 real NOT NULL,
  eextra50 real NOT NULL,
  cosThetaT real NOT NULL,
  tag_lp3 real NOT NULL,
  tag_cosBY real NOT NULL,
  tag_cosThetaDl real NOT NULL,
  tag_Dmass real NOT NULL,
  tag_deltaM real NOT NULL,
  tag_cosThetaDSoft real NOT NULL,
  tag_softP3MagCM real NOT NULL,
  tag_Dtype integer NOT NULL,
  tag_Dstartype integer NOT NULL,
  sig_hp3 real NOT NULL,
  sig_cosBY real NOT NULL,
  sig_cosThetaDtau real NOT NULL,
  sig_vtxB real NOT NULL,
  sig_Dmass real NOT NULL,
  sig_deltaM real NOT NULL,
  sig_cosThetaDSoft real NOT NULL,
  sig_softP3MagCM real NOT NULL,
  sig_hmass real NOT NULL,
  sig_vtxh real NOT NULL,
  sig_Dtype integer NOT NULL,
  sig_Dstartype integer NOT NULL,
  tag_l_ePid integer NOT NULL,
  tag_l_muPid integer NOT NULL,
  sig_h_ePid integer NOT NULL,
  sig_h_muPid integer NOT NULL,
  PRIMARY KEY (eid, idx),
  FOREIGN KEY (eid, idx) REFERENCES CandidateMeta (eid, idx) ON UPDATE CASCADE ON DELETE CASCADE
);

CREATE TABLE McCandidate (
  truth_match integer NOT NULL
) INHERITS (Candidate);
