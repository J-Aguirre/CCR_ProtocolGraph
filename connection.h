#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <string>
#include <vector>
#include <string.h>

using namespace std;

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
	for(int i = 0; i<argc; i++) {
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
  	}
  	printf("\n");

  	return 0;
}

static int callback_find(void *data, int argc, char **argv, char **azColName) {
  	vector< vector<string> > *records = static_cast< vector< vector<string> >* >(data);
	try {
    	records->emplace_back(argv, argv + argc);
	}
	catch (...) {
	// abort select on failure, don't let exception propogate thru sqlite3 call-stack
		return 1;
	}
	return 0;
}

class Connection{
  public:

    Connection();
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;
    string sql;
    string toLower(string attr);
    bool insert_node(string attr);
    void update_node(string attr, string new_attr);
    void delete_node(string attr);
    string find_node_id(string attr);
    string find_node_name(string id);

    bool insert_relation(string attr1, string attr2);
    vector<string> find_relations(string attr);

    void insert_attribute(string attr, string name_attribute, string name_value);
    void find_attribute(string attr);

    ~Connection(){
      sqlite3_close(db);
    }
};

Connection::Connection(){
     /* Open database */
     rc = sqlite3_open("/home/elard/CCR_ProtocolGraph/protocol_db.db", &db);
     if(rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
     } else {
        fprintf(stderr, "Opened database successfully\n");
     }

}

string Connection::toLower(string attr){
  string tmp ="";
  for (int i = 0; i < attr.size(); ++i){
    tmp += tolower(attr[i]);
  }
  return tmp;
}

bool Connection::insert_node(string attr){

  attr = toLower(attr);
  sql = "insert into nodes (name) values('"+attr+"');";
  const char *cstr_sql = sql.c_str();
  cout <<cstr_sql<<endl;
  /* Execute SQL statement */
  rc = sqlite3_exec(db, cstr_sql, callback, 0, &zErrMsg);
  // cout <<"despues de ingresar"<<rc<<endl;
  delete [] cstr_sql;
  if( rc != SQLITE_OK ) {
     return false;
  } else {
     return true;
  }
}

string Connection::find_node_id(string attr){
	vector< vector<string> > records;
	string id = "";
	attr = toLower(attr);

	sql = "select id from nodes where name ='"+attr+"';";
	rc = sqlite3_exec(db, sql.c_str(), callback_find, &records, &zErrMsg);

	if( rc != SQLITE_OK ) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	} else {
		fprintf(stdout, "Operation done successfully\n");
	}

	int i=0, j=0;
	if(i<records.size() && j<records[i].size())
		id = records[0][0];
	cout<<"ID: "<<id<<endl;
	return id;
}

string Connection::find_node_name(string id){
  	vector< vector<string> > records;
	string name = "";
	//attr = toLower(attr);

	sql = "select name from nodes where id ='"+id+"';";
	rc = sqlite3_exec(db, sql.c_str(), callback_find, &records, &zErrMsg);

	if( rc != SQLITE_OK ) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	} else {
		fprintf(stdout, "Operation done successfully\n");
	}

	int i=0, j=0;
	if(i<records.size() && j<records[i].size())
		name = records[0][0];
	cout<<"NAME: "<<id<<endl;
	return name;
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

bool Connection::insert_relation(string attr1, string attr2)
{
  attr1 = toLower(attr1);
  attr2 = toLower(attr2);
  string id1 = find_node_id(attr1);
  string id2 = find_node_id(attr2);

  cout<<id1<<"  "<<id2<<endl;
  if(id1 == "" || id2 == ""){
    cout <<"no exite un valor"<<endl;
    return 0;
  }

  sql = "insert into relations (node1,node2) values('"+id1+"','"+id2+"');";
  rc = sqlite3_exec(db, sql.c_str(), callback,0, &zErrMsg);

  if( rc != SQLITE_OK ) {
     return false;
  } else {
     return true;
  }

}

vector<string> Connection::find_relations(string attr){
	vector< vector<string> > records;
  	vector<string> nodes_two;
	attr = toLower(attr);
	string node_id = find_node_id(attr);
	sql = "select n2.name from relations inner join nodes as n1 on relations.node1 = n1.id inner join nodes as n2 on relations.node2 = n2.id where node1 = '"+node_id+"';";
	rc = sqlite3_exec(db, sql.c_str(), callback_find, &records, &zErrMsg);

	for(int i=0; i<records.size(); i++){
		for(int j=0; j<records[i].size(); j++){
			nodes_two.push_back(records[i][j]);
		}
	}

	if( rc != SQLITE_OK ) {
	  	fprintf(stderr, "SQL error: %s\n", zErrMsg);
	  	sqlite3_free(zErrMsg);
	} else {
	  	fprintf(stdout, "Operation done successfully\n");
	}

	return nodes_two;
}

void Connection::insert_attribute(string attr, string name_attribute, string value_attribute){
	attr = toLower(attr);
  string id_node = find_node_id(attr);
  cout<<attr<<"->"<<name_attribute<<"->"<<value_attribute<<endl;
	// sql = "insert into attributes (node_id,name_attribute,value) values('"+id_node+"','"+name_attribute+"','"+value_attribute+"');";
	// rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
	// if( rc != SQLITE_OK ) {
 //      fprintf(stderr, "SQL error: %s\n", zErrMsg);
 //      sqlite3_free(zErrMsg);
 //   	} else {
 //      fprintf(stdout, "Operation done successfully\n");
 //   	}
 //   	cout<<"Attributes:"<<endl;
 //   	cout<<id_node<<","<<id_node<<","<<name_attribute<<","<<value_attribute<<endl;
}

void Connection::find_attribute(string attr){
	vector< vector<string> > records;
	attr = toLower(attr);

	string id_node = find_node_id(attr);
	sql = "select name_attribute,value from attributes where node_id ='"+id_node+"';";
  	char* data;
  	const char *cstr_sql = sql.c_str();

  	rc = sqlite3_exec(db, cstr_sql, callback_find, &records, &zErrMsg);

  	for(int i=0; i<records.size(); i++){
  		for(int j=0; j<records[i].size(); j++){
  			cout<<records[i][j]<<" ";
  		}
  		cout<<endl;
  	}
  	if( rc != SQLITE_OK ) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
  	} else {
     	fprintf(stdout, "Operation done successfully\n");
  	}
}