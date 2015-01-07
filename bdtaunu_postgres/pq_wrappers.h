#ifndef _PQ_WRAPPER_H_
#define _PQ_WRAPPER_H_

#include <libpq-fe.h>

PGconn *Connectdb(const char *conninfo);
void Exec(PGconn *conn, const char *cmd);
void Prepare(PGconn *conn, const char *stmtName, int nparams, const char *query);
int Execprepared(PGconn *conn, const char *stmtName, int nparams, const char * const *paramValues);
void CreateSavepoint(PGconn *conn, const char *spname);
void ReleaseSavepoint(PGconn *conn, const char *spname);
void RollbackSavepoint(PGconn *conn, const char *spname);

#endif
