//#include "messages.cpp"
#include "protocol.h"
#include "connection.h"

int main(int argc, char const *argv[])
{

  sqlite3 *db;
    char *zErrMsg = 0;
    int rc;

    rc = sqlite3_open("pasdadad.db", &db);
    cout<< rc;
    if( rc == 0 ) {
       fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
       return(0);
    } else {
       fprintf(stderr, "Opened database successfully\n");
    }
    sqlite3_close(db);

	return 0;
}
