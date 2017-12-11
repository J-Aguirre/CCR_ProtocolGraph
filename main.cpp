#include "protocol.h"
#include "connection.h"
#include "client.h"
#include "server.h"
#include <iostream>

using namespace std;

int main(int argc, char const *argv[])
{

    /*Protocol test;

	cout << test.wrap("_n", "", "Peru", "synonyms:Ecuador,Chile,Uruguay") << endl;*/
    /*
	cout << test.wrap("_l", "", "Peru,Bolivia", "synonyms:Ecuador,Chile,Uruguay;synonyms:Ecuador,Chile,Uruguay;antonyms:Cusco,Inka,Peruvian;antonyms:Veracruz,Solis,Habana") << endl;
	cout << test.wrap("_q", "2", "Peru", "") <<endl;
    cout << test.wrap("_p", "2", "Peru,Colombia,Ecuador", "synonyms,antonyms") <<endl;
    cout << test.wrap("_c", "", "Peru", "") <<endl;
    cout << test.wrap("_s", "", "", "") <<endl;
    cout << test.wrap("nn", "1", "", "") <<endl;
    cout << test.wrap("ll", "1", "", "") <<endl;
    cout << test.wrap("qq", "2", "Peru,Bolivia,Chile,Ecuador", "synonyms:Ecuador,Chile,Uruguay;synonyms:Ecuador,Chile,Uruguay;antonyms:Cusco,Inka,Peruvian;antonyms:Veracruz,Solis,Habana") <<endl;
    cout << test.wrap("pp", "2", "Peru,Bolivia,Chile,E/cuador;Ecuador,Chile,Peru;Brasil,Colombia,Paraguay", "synonyms:Ecuador,Chile,Uruguay;synonyms:Ecuador,Chile,Uruguay;antonyms:Cusco,Inka,Peruvian;antonyms:Veracruz,Solis,Habana") <<endl;
	cout << test.wrap("cc", "", "0,1,4,5", "") <<endl;

    /*list<chars> unwrap_mess = test.unwrap("_n100040030Perusynonyms:Ecuador,Chile,Uruguay");
    list<chars> unwrap_mess = test.unwrap("_l100200078Peru,Bolivia,Ecuadorsynonyms:Ecuador,Chile,Uruguay;synonyms:Ecuador,Chile,Uruguay;antonyms:Uruguay");
    list<chars> unwrap_mess = test.unwrap("_q200040000Peru");
    list<chars> unwrap_mess = test.unwrap("_p200210017Peru,Colombia,Ecuadorsynonyms,antonyms");
    list<chars> unwrap_mess = test.unwrap("_c100040000Peru");
    list<chars> unwrap_mess = test.unwrap("_s100000000");
    list<chars> unwrap_mess = test.unwrap("ll100000000");
    list<chars> unwrap_mess = test.unwrap("pp200700121Peru,Bolivia,Chile,Ecuador;Ecuador,Chile,Peru;Brasil,Colombia,Paraguaysynonyms:Ecuador,Chile,Uruguay;synonyms:Ecuador,Chile,Uruguay;antonyms:Cusco,Inka,Peruvian;antonyms:Veracruz,Solis,Habana");
	list<chars> unwrap_mess = test.unwrap("cc1000700000,1,4,5");

    cout<<endl<<"PARAMETERS TO SERVER"<<endl<<endl;
    test.print_list_str(unwrap_mess);*/

    int port = 1101;
    char const* IP_SERVER = "127.0.0.1";
    char const* IP_MYSELF = "127.0.0.1";
    chars path_wordnet = "../CCR.WN";

    if(strcmp(argv[1], "sm") == 0)
    {
        Server* s = new Server(port, IP_MYSELF);
        s->connection();
    }

    if(strcmp(argv[1], "ss") == 0){
        Server* c = new Server(IP_SERVER, port, IP_MYSELF);
        c->read_from_server_master();
    }

    if(strcmp(argv[1], "c") == 0){
        Client* c = new Client(IP_SERVER, port);
        c->read_server();
    }
    else
        cout<<"Please insert a value to execute server(s) or client(c) "<<endl;

    // Connection test;
    // bool var_rt;
    // var_rt = test.insert_node("GuyANA");
    // cout <<"ok? -> "<< var_rt<<endl;
    // test.insert_node("Peru");
    // bool a = test.insert_node("uruguay");
    // test.find_node_id("URUguay");
    /*cout<<"RELATIONS: "<<endl;
    vector<string> relations = test.find_relations("nerd");
    for(int i=0; i<relations.size(); i++)
        cout<<relations[i]<<" ";*/
        /*cout<<test.find_node_name(relations[i])<<" ";*/
    /*cout<<"relations[i].size(): "<<relations.size()<<endl;*/

    // cout<<"Found: "<<test.find_node_id("Peru")<<endl;
  	//test.insert_relation("uruguay","colommbia");

    cout<<endl<<endl;
    return 0;
}

//g++ main.cpp -o main -std=c++11 -pthread -l sqlite3