#include <vector>
#include <string>
#include <iostream>
#include <libpq-fe.h>

#include "pq_wrappers.h"
#include "TableInserterPQ.h"

using namespace std;

TableInserterPQ::TableInserterPQ(
  PGconn *conn, 
  const char *table_name,
  int n_columns, 
  std::initializer_list<int> default_indices) :
  _conn(conn) {

  _n_params = n_columns - default_indices.size();
  _param_values = new const char*[_n_params];

  vector<string> query_columns(n_columns, "");
  for (auto i : default_indices) query_columns[i] = "DEFAULT";
  int query_param_index = 1;
  auto it = query_columns.begin();
  while (it != query_columns.end()) {
    if (it->empty()) {
      *it = "$" + to_string(query_param_index++);
    }
    ++it;
  }

  string query = "INSERT INTO ";
  query += string(table_name);
  query += " VALUES ";
  query += "(";
  it = query_columns.begin();
  while (it != query_columns.end()) {
    query += *it;
    if (it + 1 != query_columns.end()) query += (", ");
    ++it;
  }
  query += ");";
  _stmt_name = string(table_name) + "InserterPQ";
  Prepare(_conn, _stmt_name.c_str(), _n_params, query.c_str());

}

TableInserterPQ::~TableInserterPQ() {
  delete[] _param_values;
}

int TableInserterPQ::exec_stmt() const {

  // The insertion can fail, but could be recoverable. 
  // Wrap save points around it. 
  CreateSavepoint(_conn, "sp");
  int status = Execprepared(_conn, _stmt_name.c_str(), _n_params, _param_values);
  ReleaseSavepoint(_conn, "sp");
  CreateSavepoint(_conn, "sp");
  RollbackSavepoint(_conn, "sp");
  ReleaseSavepoint(_conn, "sp");
  return status;
}
