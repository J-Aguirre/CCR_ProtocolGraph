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
        int number_server;
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
        bool STATE_REQUEST; // if server is doing some request
        const char* REQUEST;

        Server();
        Server(char const*, int, char const*);
        Server(int, char const*);

        void print_table_servers();
        void connection();
        void read_from_server_master();
        void new_client_connection(int, int);
        int split(const string txt, vector<string> &strs, char ch);
        int print_vec_s(vector<string>);
        void send_data_to_server(int socket, string bigrama);
        chars access_list(list<chars>, int);
        void analize_request_and_send(chars);
};

Server::Server(){}

Server::Server(char const* ip, int port, char const* ip_myself)
{

    this->protocol = new Protocol();
    this->db = new Connection();

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


Server::Server(int port, const char* ip_myself){
    this->protocol = new Protocol();
    this->db = new Connection();
    this->STATE_REQUEST = false;
    printf("constructor STATE_REQUEST : %d\n", this->STATE_REQUEST);

    this->SocketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    this->port = port;

    chars ip_myself_s(ip_myself);
    pair<int, chars> element(SocketFD, ip_myself_s);
    this->table_servers[0] = element;

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
    printf("\n**************SERVERS CONNECTED**************\n");
    printf("---------------------------------------------\n");
    printf("%5s | %13s | %15s \n", "ID", "Num ConnectID", "IP name of server");
    printf("---------------------------------------------\n");

    for(it=this->table_servers.begin(); it!=this->table_servers.end(); it++) {
        printf("%5d | %13d |%15s \n", it->first, it->second.first, it->second.second.c_str());
    }
    printf("\n");
}


chars Server::access_list(list<chars> test, int pos){
    int var = 0;
    chars word = "";

    for (auto v : test){
        if (var == pos){
            word = v;
        }
        var++;
    }

    return word;
}


void Server::new_client_connection(int connect_id, int num_server){
    printf("num_server: %d\n", num_server);

    for(;;)
    {
        int stat;
        const char* type_message = "";
        printf("STATE_REQUEST: %d\n", this->STATE_REQUEST);

        if(this->STATE_REQUEST == true){
            type_message = "request";
            do{
                stat = write(connect_id, &type_message, sizeof(char));
            }while(stat<0);
            if(stat < 0)
                printf("Someproblem writing a TYPE_MESSAGE in slave server\n");

            do{
                stat = write(connect_id, &this->REQUEST, sizeof(char));
            }while(stat<0);
            if(stat  < 0)
                printf("Some problem writing a REQUEST in slave server\n");
        }
        else{
            type_message = "num_server";
            printf("type_message: %s\n", type_message);
            do{
                stat = write(connect_id, &type_message, sizeof(char));
            }while(stat<0);
            if(stat < 0)
                printf("Someproblem writing a TYPE_MESSAGE in slave server\n");

            do{
                stat = write(connect_id, &num_server, sizeof(int));
            }while(stat<0);
            if (stat  < 0)
                printf("Some problem writing NUMBER_SERVER in slave server\n");
        }


    // do
    // {
        char* buffer;
        cout<<"connect_id"<<connect_id<<endl;
        n = read(connect_id, buffer, DEFAUL_SIZE);
        if (n < 0) perror("ERROR reading from socket");
        
        chars mess_unwrap(buffer);
        cout<<"mess_unwrap: "<<mess_unwrap<<endl;
        list<chars> test = this->protocol->unwrap(mess_unwrap);
        cout<<"Message of client:"<<endl;
        this->protocol->print_list_str(test);

        int var = 0;
        chars word = "";
        chars word2 = "";
        for (auto v : test){
            var++;
            if (var == 2){
                word = v;
            }
        }
        var = 0;
        for (auto v : test){
            var++;
            if (var == 3){
                word2 = v;
            }
        }


        /*chars word = this->access_list(test, 2);*/
        this->db->insert_node(word);
        /*chars word2 = this->access_list(test, 3);*/
        /*this->db->insert_relation(word, word2);*/

        chars messa = "";
        if(strlen(buffer) > 0){
            printf("Enter message to client: ");
            scanf("%s" , this->message);
            messa = this->protocol->wrap("_n", "", this->message, "");
        }
        else {
            printf("Client desconnected !!! \n");
            break;
        }

        n = write(connect_id, messa.c_str(), messa.size());
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
        printf("Server connected !!! \n");

        char buffer[256];
        bzero(buffer,256);
        n = read(ConnectFD, buffer, 16);
        if (n < 0) perror("ERROR reading from socket");

        chars ip_server_connected(buffer);
        pair<int, chars> element(ConnectFD, ip_server_connected);
        int num_server = this->table_servers.size();
        this->table_servers[num_server] = element;
        this->print_table_servers();
        const char* request = "_n100040030Perusynonyms:Ecuador,Chile,Uruguay"; // example of request _n
        printf("this->STATE_REQUEST: %s\n", this->STATE_REQUEST);
        thread t(&Server::new_client_connection, this, ConnectFD, num_server);
        t.detach();

        printf("Waiting for another connection ... \n");        

    }
}

void Server::analize_request_and_send(chars request){

    if (request.substr(0, 2) == "_n"){
        cout<<"request: "<<request<<endl;
    }
}

void Server::read_from_server_master()
{
    int stat;

    for(;;)
    {
        n = write(this->SocketFD, this->ip_myself, 15);
        if (n < 0) perror("ERROR writing to socket");
        printf("nsadsad: %d asdad\n", n);

            printf("inside loop to get type_message_buff");
        const char* type_message_buff = "";
        do{
            stat = read(this->SocketFD, &type_message_buff, 10);
        }while(stat<0);

        printf("type_message_buff: %s\n", type_message_buff);

        if (type_message_buff == "request"){
            //(S) READING ALL MESSAGE REQUEST FROM SERVER
            const char* buffer_c = "";
            do{
                stat = read(this->SocketFD, &buffer_c, 2);
            }while(stat<0);

            printf("ACTION MYSELF: %s\n", buffer_c);
            //(E) READING ALL MESSAGE REQUEST FROM SERVER
        }
        else{

            //(S) READING NUMBER SERVER FROM SERVER
            int bufferr = 0;
            do{
                stat = read(this->SocketFD, &bufferr, 4);
            }while(stat<0);

            this->number_server = bufferr;
            printf("NUMBER_SERVER MYSELF: %d\n", this->number_server);
            //(E) READING NUMBER SERVER FROM SERVER
        }

        printf("Enter a message to server: ");
        scanf("%s" , this->message);
        chars messa = this->protocol->wrap("_n", "", this->message, "");

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
