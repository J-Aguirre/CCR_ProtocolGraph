#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <string>
#include <string.h>

using namespace std;
static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
  int i;
  for(i = 0; i<argc; i++) {
    printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
  }
  printf("\n");
  return 0;
}

class Connection{
  public:

    Connection();
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;
    string sql;
    void insert_node(string attr);
    void update_node(string attr, string new_attr);
    void delete_node(string attr);
    void find_node(string attr);

    ~Connection(){
      sqlite3_close(db);
    }
};

Connection::Connection(){
     /* Open database */
     rc = sqlite3_open("protocol_db.db", &db);
     if(rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
     } else {
        fprintf(stderr, "Opened database successfully\n");
     }

}

void Connection::insert_node(string attr){
  sql = "insert into nodes (name) values('"+attr+"');";
  const char *cstr_sql = sql.c_str();
  cout <<cstr_sql<<endl;
  /* Execute SQL statement */
  rc = sqlite3_exec(db, cstr_sql, callback, 0, &zErrMsg);
  // cout <<"despues de ingresar"<<rc<<endl;
  delete [] cstr_sql;
}

void Connection::find_node(string attr){
  sql = "select * from nodes where name ='"+attr+"';";
  // cout <<sql<<endl;
  const char *cstr_sql = sql.c_str();
  rc = sqlite3_exec(db, cstr_sql, callback,0, &zErrMsg);

  if( rc != SQLITE_OK ) {
     fprintf(stderr, "SQL error: %s\n", zErrMsg);
     sqlite3_free(zErrMsg);
  } else {
     fprintf(stdout, "Operation done successfully\n");
  }
}

void Connection::update_node(string attr, string new_attr){
  sql = "update nodes set name = '"+new_attr +"' where name='"+attr+"';";
  /* Execute SQL statement */
  const char *cstr_sql = sql.c_str();
  rc = sqlite3_exec(db, cstr_sql, callback,0, &zErrMsg);

   if( rc != SQLITE_OK ) {
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   } else {
      fprintf(stdout, "Operation done successfully\n");
   }

}
void Connection::delete_node(string attr){
  sql = "delete from nodes where name='"+attr+"';";
  /* Execute SQL statement */
  const char *cstr_sql = sql.c_str();
  rc = sqlite3_exec(db, cstr_sql, callback,0, &zErrMsg);

   if( rc != SQLITE_OK ) {
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   } else {
      fprintf(stdout, "Operation done successfully\n");
   }

}
