#include "protocol.h"
#include "connection.h"
#include "client.h"
#include "server.h"
#include <iostream>

using namespace std;

int main(int argc, char const *argv[])
{

    //Protocol test;

	/*cout << test.wrap("_n", "", "Peru", "synonyms:Ecuador,Chile,Uruguay") << endl;
	cout << test.wrap("_l", "", "Peru,Bolivia", "synonyms:Ecuador,Chile,Uruguay;synonyms:Ecuador,Chile,Uruguay;antonyms:Cusco,Inka,Peruvian;antonyms:Veracruz,Solis,Habana") << endl;
	cout << test.wrap("_q", "2", "Peru", "") <<endl;
    cout << test.wrap("_p", "2", "Peru,Colombia,Ecuador", "synonyms,antonyms") <<endl;
    cout << test.wrap("_c", "", "Peru", "") <<endl;
    cout << test.wrap("_s", "", "", "") <<endl;
    cout << test.wrap("nn", "1", "", "") <<endl;
    cout << test.wrap("ll", "1", "", "") <<endl;
    cout << test.wrap("qq", "2", "Peru,Bolivia,Chile,Ecuador", "synonyms:Ecuador,Chile,Uruguay;synonyms:Ecuador,Chile,Uruguay;antonyms:Cusco,Inka,Peruvian;antonyms:Veracruz,Solis,Habana") <<endl;
    cout << test.wrap("pp", "2", "Peru,Bolivia,Chile,Ecuador;Ecuador,Chile,Peru;Brasil,Colombia,Paraguay", "synonyms:Ecuador,Chile,Uruguay;synonyms:Ecuador,Chile,Uruguay;antonyms:Cusco,Inka,Peruvian;antonyms:Veracruz,Solis,Habana") <<endl;*/

    /*list<chars> unwrap_mess = test.unwrap("_n100040030Perusynonyms:Ecuador,Chile,Uruguay");
    list<chars> unwrap_mess = test.unwrap("_l100200078Peru,Bolivia,Ecuadorsynonyms:Ecuador,Chile,Uruguay;synonyms:Ecuador,Chile,Uruguay;antonyms:Uruguay");
    list<chars> unwrap_mess = test.unwrap("_q200040000Peru");
    list<chars> unwrap_mess = test.unwrap("_p200210017Peru,Colombia,Ecuadorsynonyms,antonyms");
    list<chars> unwrap_mess = test.unwrap("_c100040000Peru");
    list<chars> unwrap_mess = test.unwrap("_s100000000");
    list<chars> unwrap_mess = test.unwrap("ll100000000");
    list<chars> unwrap_mess = test.unwrap("pp200700121Peru,Bolivia,Chile,Ecuador;Ecuador,Chile,Peru;Brasil,Colombia,Paraguaysynonyms:Ecuador,Chile,Uruguay;synonyms:Ecuador,Chile,Uruguay;antonyms:Cusco,Inka,Peruvian;antonyms:Veracruz,Solis,Habana");

    cout<<endl<<"PARAMETERS TO SERVER"<<endl<<endl;
    test.print_list_str(unwrap_mess);*/

    
    int port = 1101;
    char const* IP_SERVER = "192.168.9.108";
    chars path_bigramas = "../en.wiki.big";
    chars path_wordnet = "../CCR.WN";

    if(strcmp(argv[1], "s") == 0)
    {
        Server* s = new Server(port);
        s->connection();        
    }

    if(strcmp(argv[1], "c") == 0){
        Client* c = new Client(IP_SERVER, port);
        c->read_server();
    }
    else
        cout<<"Please insert a value to execute server(s) or client(c) "<<endl;

    /*Connection test;
    test.insert_node("ecuador");*/

    cout<<endl<<endl;
    return 0;
}

//g++ main.cpp -o main -std=c++11 -pthread -l sqlite3