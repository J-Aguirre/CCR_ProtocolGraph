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
    void insert_node();
    void update_node();
    void delete_node();
    void find_node();

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

void Connection::insert_node(){
  sql = "INSERT INTO nodes (name) "  \
        "VALUES ('Peru');";
  /* Execute SQL statement */
  rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
}
