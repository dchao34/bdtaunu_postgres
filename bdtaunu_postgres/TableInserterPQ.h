#ifndef _TABLEINSERTERPQ_H_
#define _TABLEINSERTERPQ_H_

#include <libpq-fe.h>
#include <string>
#include <initializer_list>

class TableInserterPQ {
  public:
    TableInserterPQ() = default;
    TableInserterPQ(
        PGconn* conn, const char *table_name, 
        int n_columns, std::initializer_list<int> default_indices);
    virtual ~TableInserterPQ();

    int exec_stmt() const;

  protected:
    const char **_param_values;

  private:
    PGconn *_conn;
    std::string _stmt_name;
    int _n_params;
};

#endif
