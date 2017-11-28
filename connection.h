#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <string>

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
    void insert_node();
    void update_node();
    void delete_node();
    void find_node();

    ~Connection(){}
};

Connection::Connection(){
     /* Open database */
     sqlite3 *db;
     char *zErrMsg = 0;
     int rc;
     const char *sql;
     rc = sqlite3_open("asdasdasdasd.db", &db);
     cout << rc;

}
