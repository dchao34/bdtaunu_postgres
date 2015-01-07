#include <iostream>
#include <string>

#include <libpq-fe.h>

#include "pq_wrappers.h"

using namespace std;

static void 
exit_nicely(PGconn *conn) {
  PQfinish(conn);
  exit(EXIT_FAILURE);
}

PGconn *Connectdb(const char *conninfo) {
  PGconn *conn = PQconnectdb(conninfo);
  if (PQstatus(conn) != CONNECTION_OK) {
    cerr << "Connection to database failed: " << PQerrorMessage(conn);
    exit_nicely(conn);
  }
  return conn;
}

void Exec(PGconn *conn, const char *cmd) {
  PGresult *res = PQexec(conn, cmd);
  if (PQresultStatus(res) != PGRES_COMMAND_OK) {
    cerr << cmd << " failed: " << PQerrorMessage(conn);
    PQclear(res);
    exit_nicely(conn);
  }
  PQclear(res);
}

void Prepare(PGconn *conn, 
             const char *stmtName, 
             int nparams,
             const char *query) { 
  PGresult *res = PQprepare(conn, stmtName, query, nparams, nullptr);
  if (PQresultStatus(res) != PGRES_COMMAND_OK) {
    cerr << "Prepare failed: " << PQerrorMessage(conn);
    PQclear(res);
    exit_nicely(conn);
  }
  PQclear(res);
}

void CreateSavepoint(PGconn *conn, const char *spname) {
  string cmd = "SAVEPOINT " + string(spname);
  PGresult *res = PQexec(conn, cmd.c_str());
  PQclear(res);
}

void ReleaseSavepoint(PGconn *conn, const char *spname) {
  string cmd = "RELEASE " + string(spname);
  PGresult *res = PQexec(conn, cmd.c_str());
  PQclear(res);
}

void RollbackSavepoint(PGconn *conn, const char *spname) {
  string cmd = "ROLLBACK TO SAVEPOINT " + string(spname);
  PGresult *res = PQexec(conn, cmd.c_str());
  PQclear(res);
}

int Execprepared(PGconn *conn, 
                 const char *stmtName, 
                 int nparams, 
                 const char * const *paramValues) {
  PGresult *res = PQexecPrepared(conn, stmtName, nparams, 
                                 paramValues, nullptr, nullptr, 0);
  if (PQresultStatus(res) != PGRES_COMMAND_OK) {

    cerr << "Execprepared failed when executing prepared statement "; 
    cerr << "\"" << stmtName << "\":" << endl; 
    cerr << PQerrorMessage(conn);

    string errmsg = string(PQerrorMessage(conn));

    // want to ignore duplicated unique keys when encountered after the first time.
    if (errmsg.find("ERROR:  duplicate key value violates unique constraint")
        != string::npos) {

      // return a status flag to indicate that a recoverable error has occured. 
      PQclear(res);
      return 1;

    // fatal error otherwise
    } else {
      PQclear(res);
      exit_nicely(conn);
    }

  }
  PQclear(res);
  return 0;
}
