#include <iostream>
#include <assert.h>
#include "sqlite3.h"
#include "log.h"

using namespace std;

typedef int (*callback)(void *data, int argc, char **argv, char **azColName);

class SqliteDemo
{
public:
  SqliteDemo(string dbName) : db(NULL), errMsg(NULL)
  {
    int rc = sqlite3_open(dbName.c_str(), &db);
    if (rc)
    {
      LOG_ERR("open db:%s failed", dbName.c_str());
    }
  }
  ~SqliteDemo()
  {
    if (db)
    {
      sqlite3_close(db);
    }
  }

  bool CreateTable()
  {
    string sql = "create table if not exists demo_table ("
                 "id int primary key not null, "
                 "name text,"
                 "address text"
                 ");";
    return ReturnExecMsg(sql);
  }

  bool DropTable()
  {
    string sql = "drop table demo_table";
    return ReturnExecMsg(sql);
  }

  bool InsertData(string id, string name, string address)
  {
    string sql = "insert into demo_table (ID, name, address) values (" + id + ",'" + name + "'," + "'" + address + "'" + ");";
    return ReturnExecMsg(sql);
  }

  bool Select()
  {
    string sql = "select * from demo_table";
    return ExecStep(sql, NULL) == SQLITE_OK ? true : false;
  }

protected:
  bool ReturnExecMsg(const string &sql)
  {
    if (SQLITE_OK != Exec(sql.c_str(), SqliteDemo::ExecCB, this, &errMsg))
    {
      LOG_ERR("exec sql:%s failed:%s", sql.c_str(), errMsg);
      return false;
    }
    return true;
  }

  int Exec(const string &sql, callback cb, void *data, char **errmsg)
  {
    return sqlite3_exec(db, sql.c_str(), cb, 0, errmsg);
  }

  int ExecStep(const string &sql, void *data)
  {
    int nCol;
    sqlite3_stmt *statement;
    int ret = sqlite3_prepare(db, sql.c_str(), -1, &statement, NULL);
    if (ret != SQLITE_OK)
    {
      LOG_ERR("prepare error, ret:%d", ret);
      return ret;
    }
    while (sqlite3_step(statement) == SQLITE_ROW)
    {

      nCol = sqlite3_column_count(statement);
      for (int i = 0; i < nCol; i++)
      {
        LOG_DEBUG("%s:%s", sqlite3_column_name(statement, i), sqlite3_column_text(statement, i));
      }
    }
    sqlite3_finalize(statement);
    return ret;
  }

  static int ExecCB(void *data, int argc, char **argv, char **azColName)
  {
    for (int i = 0; i < argc; i++)
    {
      LOG_DEBUG("%s = %s \n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    return 0;
  }

private:
  sqlite3 *db;
  char *errMsg;
};

int main(int argc, char *argv[])
{
  SqliteDemo demo("demo_db");
  assert(demo.CreateTable());
  for (int i = 0; i < 10; i++)
  {
    assert(demo.InsertData(to_string(i), "name" + to_string(i), "address" + to_string(i)));
  }
  assert(demo.Select());
  assert(demo.DropTable());
  return 0;
}