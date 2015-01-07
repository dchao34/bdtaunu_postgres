#ifndef _PQ_WRAPPER_H_
#define _PQ_WRAPPER_H_

#include <libpq-fe.h>

PGconn *Connectdb(const char *conninfo);
void Exec(PGconn *conn, const char *cmd);
void Prepare(PGconn *conn, const char *stmtName, int nparams, const char *query);
void Execprepared(PGconn *conn, const char *stmtName, int nparams, const char * const *paramValues);

#endif
