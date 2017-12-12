#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <thread>
#include <map>
#include <fstream>
#include <string>
#include <vector>
#include <iterator>
/*#include "protocol.h"*/

using namespace std;

int DEFAUL_SIZE = 255;
typedef string chars;

class Server {

    public:
        struct sockaddr_in stSockAddr;
        int Res;
        int SocketFD;
        int port;
        char const* ip_address;
        char const* ip_myself;
        int message_server;

        char message[255];
        char buffer[256];
        int n;

        int packet_size;
        int header_size;
        chars path_bigramas;
        chars path_wordnet;
        Protocol* protocol;
        Connection* db;
        map<int, pair<int, chars> > table_servers; // <name_number, number_socket>
        bool type_server = 0; // 1: server_slave, 0: server_master

        Server();
        Server(char const*, int, char const*);
        // port, header_size, packet_size,
        Server(int);        
        void print_table_servers();
        void connection();
        void read_server();
        void new_client_connection(int);
        int split(const string txt, vector<string> &strs, char ch);
        int print_vec_s(vector<string>);
        void send_data_to_server(int socket, string bigrama);
        void load_data();
};

Server::Server(){}

Server::Server(char const* ip, int port, char const* ip_myself)
{

    this->protocol = new Protocol();
    /*chars message = this->protocol->envelop("simple-message", "test text lalito");
    cout<<"envelop message: "<<message<<endl;
    chars unwrapped_messa = this->protocol->unwrap(message);
    cout<<"unwrapped message: "<<unwrapped_messa<<endl;*/

    this->ip_myself = ip_myself;
    this->ip_address = ip;
    this->port = port;
    this->SocketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    // error while we try create the token
    if (-1 == this->SocketFD)
    {
        perror("cannot create socket");
        exit(EXIT_FAILURE);
    }

    memset(&this->stSockAddr, 0, sizeof(struct sockaddr_in));

    this->stSockAddr.sin_family = AF_INET;
    this->stSockAddr.sin_port = htons(this->port);
    this->Res = inet_pton(AF_INET, ip_address, &this->stSockAddr.sin_addr);

    if (0 > this->Res)
    {
        perror("error: first parameter is not a valid address family");
        close(this->SocketFD);
        exit(EXIT_FAILURE);
    }
    else if (0 == this->Res)
    {
        perror("char string (second parameter does not contain valid ipaddress");
        close(this->SocketFD);
        exit(EXIT_FAILURE);
    }

    if (-1 == connect(this->SocketFD, (const struct sockaddr *)&this->stSockAddr, sizeof(struct sockaddr_in)))
    {
        perror("connect failed");
        close(this->SocketFD);
        exit(EXIT_FAILURE);
    }
  
}


Server::Server(int port){
    this->protocol = new Protocol();
    this->db = new Connection();

    this->SocketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    this->port = port;

    if(-1 == this->SocketFD)
    {
        perror("can not create socket");
        exit(EXIT_FAILURE);
    }

    memset(&this->stSockAddr, 0, sizeof(struct sockaddr_in));

    this->stSockAddr.sin_family = AF_INET;
    this->stSockAddr.sin_port = htons(this->port);
    this->stSockAddr.sin_addr.s_addr = INADDR_ANY;

    if(-1 == bind(this->SocketFD,(const struct sockaddr *)&this->stSockAddr, sizeof(struct sockaddr_in)))
    {
        perror("error bind failed");
        close(this->SocketFD);
        exit(EXIT_FAILURE);
    }

    if(-1 == listen(this->SocketFD, 10))
    {
        perror("error listen failed");
        close(this->SocketFD);
        exit(EXIT_FAILURE);
    }

    printf("Waiting for a connection ... \n");
}


void Server::print_table_servers(){
    map<int, pair<int, chars> >::iterator it;
    printf("********SERVERS CONNECTED********* \n");
    printf("----------------------------------\n");
    printf("ID | Num ConnectID | Number name of server \n");

    for(it=this->table_servers.begin(); it!=this->table_servers.end(); it++) {
        printf("%10d | %10d |%15s \n", it->first, it->second.first, it->second.second.c_str());
    }
}


void Server::new_client_connection(int connect_id){
    for(;;)
    {
    // do
    // {
        char* buffer;
        n = read(connect_id, buffer, DEFAUL_SIZE);
        if (n < 0) perror("ERROR reading from socket");
        
        chars mess_unwrap(buffer);
        cout<<"mess_unwrap: "<<mess_unwrap<<endl;
        list<chars> test = this->protocol->unwrap(mess_unwrap);
        cout<<"Message of client:"<<endl;
        this->protocol->print_list_str(test);
        int var = 0;
        chars word0 = "";
        chars word1 = "";
        chars word2 = "";
        chars word3 = "";
        chars word4 = "";

        for (auto v : test){
            if (var == 0){
                word0 = v;
            }
            var++;
        }
        var = 0;
        for (auto v : test){
            if (var == 1){
                word1 = v;
            }
            var++;
        }
        var = 0;
        for (auto v : test){
            if (var == 2){
                word2 = v;
            }
            var++;
        }
        var = 0;
        for (auto v : test){
            if (var == 3){
                word3 = v;
            }
            var++;
        }
        var = 0;
        for (auto v : test){
            if (var == 4){
                word4 = v;
            }
            var++;
        }
        cout<<"word0: "<<word0<<endl;
        cout<<"word1: "<<word1<<endl;
        if(word0 == "_n")
            this->db->insert_node(word1);
        if(word0 == "_ĺ")
            this->db->insert_relation(word1, word2);
        /*if(word0 == '_q')
            this->db->*/

        /*chars messa = "";
        if(strlen(buffer) > 0){
            printf("Enter message to client: ");
            scanf("%s" , this->message);
            messa = this->protocol->wrap("_l", "", this->message, "");
        }
        else {
            printf("Client desconnected !!! \n");
            break;
        }

        n = write(connect_id, messa.c_str(), messa.size());*/
        if (n < 0) perror("ERROR writing to socket");
    }
    // } while(buffer != "chao");
    shutdown(connect_id, SHUT_RDWR);
    close(connect_id);
}

void Server::connection(){

    for(;;){

        int ConnectFD = accept(this->SocketFD, NULL, NULL);
        cout<<"this->SocketFD: "<<this->SocketFD<<endl;

        if(0 > ConnectFD)
        {
            perror("error accept failed");
            close(this->SocketFD);
            exit(EXIT_FAILURE);
        }
        printf("Client connected !!! \n");

        char buffer[256];
        bzero(buffer,256);
        n = read(ConnectFD, buffer, 16);
        if (n < 0) perror("ERROR reading from socket");

        chars ip_server_connected(buffer);
        pair<int, chars> element(ConnectFD, ip_server_connected);
        int size_table = this->table_servers.size();
        this->table_servers[size_table + 1] = element;
        this->print_table_servers();


        thread t(&Server::new_client_connection, this, ConnectFD);
        t.detach();

        /*char answer;
        printf("Did you want load bigramas to servers? (Y/n): ");
        scanf("%c" , &answer);
        printf("answer: %c", answer);
        if(answer == 'y' || answer == 'Y'){

            this->load_data();
        }
        else{*/
            printf("Waiting for another connection ... \n");
        /*}*/

    }
}

void Server::read_server()
{
    for(;;)
    {
        n = write(this->SocketFD, this->ip_myself, 15);
        if (n < 0) perror("ERROR writing to socket");

        int option = 0;
        chars action = "";
        chars deepness = "";
        chars attributes = "";
        cout<<"PUT 1: if you want to insert a node: "<<endl;
        cout<<"PUT 2: if you want to insert a relation: "<<endl;
        cout<<"PUT 3: if you want to insert a query deepness: "<<endl;
        cout<<"PUT 4: if you want to insert a sentence deepness: "<<endl;
        cin>>option;
        if(option == 1){
            action = "_n";
        }
        if(option == 2){
            action = "_l";
        }
        if(option == 3){
            action = "_q";
            deepness = "2";
        }
        if(option == 4){
            action = "_p";
            deepness = "2";
        }
        printf("Enter a message to server: ");
        scanf("%s" , this->message);

        chars messa = this->protocol->wrap(action, deepness, this->message, attributes);

        n = write(this->SocketFD, messa.c_str(), messa.size());
        if (n < 0) perror("ERROR writing to socket");
        
        bzero(this->buffer, 255);
        this->message_server = read(this->SocketFD, this->buffer, 255);
        if (this->message_server < 0) perror("ERROR reading from socket");

        list<chars> test = this->protocol->unwrap(this->buffer);
        cout<<"Message of client:"<<endl;
        this->protocol->print_list_str(test);
    }

    shutdown(this->SocketFD, SHUT_RDWR);
    close(this->SocketFD);
}

int Server::split(const string txt, vector<string> &strs, char ch)
{
    string s;
    istringstream text(txt);
    while (getline(text, s, ' ')) {
        strs.push_back(s.c_str());
    }
}

int Server::print_vec_s(vector<string> vec){
    for(int i=0; i<vec.size(); i++)
        printf("%s - ", vec[i].c_str());
    printf("\n");
}

void Server::send_data_to_server(int socket, string bigrama){
    printf("bigrama in send_data_to_server: %s\n", bigrama.c_str());
    n = write(socket, bigrama.c_str(), 255);
    if (n < 0) perror("ERROR writing to socket");
}

/*void Server::load_data(){
    printf("load_data \n");
    ifstream bigramas(this->path_bigramas);

    string line;
    if(bigramas.is_open())
    {
        int server_counter = 1;
        string previous_first_word = "";
        for(int i=0; i<10; i++)
        {
            getline(bigramas, line);
            vector<string> line_vec;
            this->split(line, line_vec, ' ');
            this->print_vec_s(line_vec);
            printf("something!!!\n");
            printf("i: %d\n", i);
            if(i != 0){
                printf("previous_first_word: %s\n", previous_first_word.c_str());
                printf("line_vec[0]: %s\n", line_vec[0].c_str());
                if(previous_first_word != line_vec[0]){
                    printf("Are differents!!!!!!\n");
                    server_counter++;
                }
                else{
                    printf("Are the SAME!!!!!!\n");
                }
            }

            int socket_id = this->table_servers[server_counter];
            printf("socket_id: %d\n", socket_id);
            this->send_data_to_server(socket_id, line);
            previous_first_word = line_vec[0];

            // printf ("%s\n", line.c_str());
        }
        bigramas.close();
    }
}*/