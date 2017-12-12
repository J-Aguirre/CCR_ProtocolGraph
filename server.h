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

//#include "protocol.h"

using namespace std;

int DEFAUL_SIZE = 255;
typedef string chars;

class Server {

    public:
        struct sockaddr_in stSockAddr;
        struct sockaddr_in stSockAddr_c;
        int Res;
        int Res_c;
        int SocketFD;
        int SocketFD_c;
        int port;
        int port_c;
        char const* ip_address;
        char const* ip_address_c;
        char const* ip_myself;
        char const* ip_myself_c;
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
        bool STATE_REQUEST_c; // if server is doing some request
        const char* REQUEST;


        Server();
        Server(char const*, int, char const*);
        Server(int, char const*);

        void print_table_servers();
        void connection();
        void read_from_server_master();
        void new_server_slave_connection(int, int);
        int split(const string txt, vector<string> &strs, char ch);
        int print_vec_s(vector<string>);
        void send_data_to_server(int socket, string bigrama);
        chars access_list(list<chars>, int);
        void analize_request_and_send(chars);
        void init_to_client(int, char const*);
        void connection_c_to_s();
        void new_client_connection(int);
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


void Server::init_to_client(int port_c, char const* ip_myself)
{
    this->STATE_REQUEST_c = false;
    printf("constructor STATE_REQUEST_c : %d\n", this->STATE_REQUEST_c);

    this->SocketFD_c = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    this->port_c = port_c;

    if(-1 == this->SocketFD_c)
    {
        perror("can not create socket");
        exit(EXIT_FAILURE);
    }

    memset(&this->stSockAddr_c, 0, sizeof(struct sockaddr_in));

    this->stSockAddr_c.sin_family = AF_INET;
    this->stSockAddr_c.sin_port = htons(this->port);
    this->stSockAddr_c.sin_addr.s_addr = INADDR_ANY;

    if(-1 == bind(this->SocketFD_c,(const struct sockaddr *)&this->stSockAddr_c, sizeof(struct sockaddr_in)))
    {
        perror("error bind failed");
        close(this->SocketFD_c);
        exit(EXIT_FAILURE);
    }

    if(-1 == listen(this->SocketFD_c, 10))
    {
        perror("error listen failed");
        close(this->SocketFD_c);
        exit(EXIT_FAILURE);
    }

    printf("Waiting for a connection ... \n");  
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


void Server::new_server_slave_connection(int connect_id, int num_server){
    printf("num_server: %d\n", num_server);

    for(;;)
    {
        int stat;
        const char* type_message = "";
        printf("STATE_REQUEST: %d\n", this->STATE_REQUEST);

        if(this->STATE_REQUEST == true){
            printf("Ready to sent request to SS\n");
            type_message = "request";
            do{
                stat = write(connect_id, &type_message, sizeof(char)*10);
            }while(stat<0);
            if(stat < 0)
                printf("Someproblem writing a TYPE_MESSAGE in slave server\n");

            chars message_buff = "";
            printf("CREATE A NEW NODE:\n");
            cin>>message_buff;
            cout<<"Node to send: "<<message_buff<<endl;

            chars wrap_mess = this->protocol->wrap("_n", "", message_buff, "");
            const char* mess_buff = wrap_mess.c_str();
            printf("mess_buff: %s\n", mess_buff);

            int size_buff = wrap_mess.size();
            printf("size_buff: %d\n", size_buff);
            do{
                stat = write(connect_id, (void*)&size_buff, sizeof(int));
            }while(stat<0);
            if (stat  < 0)
                printf("Some problem writing SIZE of REQUEST in master server\n");

            do{
                cout<<"mess_buff bucle: "<< mess_buff<<endl;
                stat = write(connect_id, &mess_buff, size_buff);
            }while(stat < 0);
            if (stat  < 0)
                printf("Some problem writing request in master server\n");
        }
        else{
        /*if(this->STATE_REQUEST == false){*/
            type_message = "num_server";
            printf("type_message: %s\n", type_message);
            do{
                stat = write(connect_id, &type_message, sizeof(char)*10);
            }while(stat<0);
            if(stat < 0)
                printf("Someproblem writing a TYPE_MESSAGE in slave server\n");

            do{
                stat = write(connect_id, &num_server, sizeof(int));
            }while(stat<0);
            if (stat  < 0)
                printf("Some problem writing NUMBER_SERVER in slave server\n");
            this->STATE_REQUEST = true;
            continue;
        }

    }
    // } while(buffer != "chao");
    shutdown(connect_id, SHUT_RDWR);
    close(connect_id);
}

void Server::connection(){

    struct sockaddr_in client_addr;
    socklen_t len;
    char buffer[256];
    char ident[2];
    for(;;){

        int ConnectFD = accept(this->SocketFD, NULL, NULL);
        cout<<"this->SocketFD: "<<this->SocketFD<<endl;

        if(0 > ConnectFD)
        {
            perror("error accept failed");
            close(this->SocketFD);
            exit(EXIT_FAILURE);
        }

        else{
            bzero(ident,2);
            n = read(ConnectFD, ident, 2);
            if (n < 0) perror("ERROR reading from socket");
            if(ident[0]=='0'){

                printf("Server connected !!! \n");
                
                bzero(buffer,256);
                n = read(ConnectFD, buffer, 16);
                if (n < 0) perror("ERROR reading from socket");

                chars ip_server_connected(buffer);
                pair<int, chars> element(ConnectFD, ip_server_connected);
                int num_server = this->table_servers.size();
                this->table_servers[num_server] = element;
                this->print_table_servers();
                const char* request = "_n100040030Perusynonyms:Ecuador,Chile,Uruguay"; // example of request _n
                printf("this->STATE_REQUEST: %d\n", this->STATE_REQUEST);

                thread t(&Server::new_server_slave_connection, this, ConnectFD, num_server);
                t.detach();
            }
            else if(ident[0]=='1'){ 
                printf("Client connected !!! \n");
                bzero(buffer,256);
                n = read(ConnectFD, buffer, 256);
                if (n < 0) perror("ERROR reading from socket");

                chars ip_client_connected(buffer);
                cout<<"ip_client_connected: "<<ip_client_connected<<endl;

                list<chars> prtcl = this->protocol->unwrap(ip_client_connected);
                list<chars>::iterator i;
                i = prtcl.begin();
                
                if(*i == "_n"){
                    i++;
                    chars word = *i;
                    db->insert_node(word);
                    //thread t(db->insert_node,this,word);
                    t.detach();
                    list<chars> attributes;
                    for(;i != prtcl.end(); i++){
                        attributes = splitt(*i,':');
                        cout<<attributes.front()<<":"<<attributes.back()<<endl;
                        //db->insert_attribute(word,attributes.front(),attributes.back());
                        //thread t2(db->insert_attribute,word,attributes.front(),attributes.back());
                        //t2.detach();
                    }
                }

                // if(*i == "_q"){
                //     i++;
                //     chars word = *i;
                //     // hash conexion esclavo
                //     db->find_node(word);
                //     list<chars> attributes;
                //     for(;i != prtcl.end(); i++){
                //         attributes = splitt(*i,':');
                //         cout<<attributes.front()<<":"<<attributes.back()<<endl;
                //         db->insert_attribute(word,attributes.front(),attributes.back());
                //     }
                // }
                    // case "_l":
                    //     i++;
                        
                    //     /*map<int, pair<int, chars> >::iterator it;
                    //     printf("\n**************SERVERS CONNECTED**************\n");
                    //     printf("---------------------------------------------\n");
                    //     printf("%5s | %13s | %15s \n", "ID", "Num ConnectID", "IP name of server");
                    //     printf("---------------------------------------------\n");

                    //     for(it=this->table_servers.begin(); it!=this->table_servers.end(); it++) {
                    //         printf("%5d | %13d |%15s \n", it->first, it->second.first, it->second.second.c_str());
                    //     }
                    //     printf("\n");*/

                    //     map<int, pair<int, chars> >::iterator it = this->table_servers.begin();
                    //     char buff[256];
                    //     do
                    //     {

                    //     }while(1)

                    //     //db->insert_relation(*i,*i);
                    //     break;

                    // case "_q":
                    //     i++;
                    //     db->find_node_id(*i);
                    //     break;

                    // case "_p":
                    //     i++;
                    //     //db->find_node_id(*i);
                    //     break;

                    // case "_c":
                    //     i++;
                    //     //db->find_node_id(*i);
                    //     break;

                    // case "_s":
                    //     i++;
                    //     //db->find_node_id(*i);
                    //     break;

                    // default:
                    //     cout<<"ERROR"<<endl;
                    //     break;

                //sacar protocolo

            }
        }
        printf("Waiting for another connection ... \n");
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
        //this->db->insert_node(word);
        this->db->insert_relation(word, word2);

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


void Server::connection_c_to_s(){

    for(;;){
        int ConnectFD = accept(this->SocketFD_c, NULL, NULL);
        cout<<"this->SocketFD: "<<this->SocketFD_c<<endl;

        if(0 > ConnectFD)
        {
            perror("error accept failed");
            close(this->SocketFD_c);
            exit(EXIT_FAILURE);
        }
        printf("Client connected !!! \n");

        char buffer[256];
        bzero(buffer,256);
        n = read(ConnectFD, buffer, 16);
        if (n < 0) perror("ERROR reading from socket");

        /*chars ip_server_connected(buffer);
        pair<int, chars> element(ConnectFD, ip_server_connected);
        int size_table = this->table_servers.size();
        this->table_servers[size_table + 1] = element;*/

        thread t(&Server::new_client_connection, this, ConnectFD);
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
    char ident[] = "0";
    n = write(this->SocketFD, ident, 2);

    for(;;)
    {
        do{
            n = write(this->SocketFD, this->ip_myself, 15);
        }while(n < 0);
        if(n < 0) perror("ERROR writing to socket");

        const char* type_message_buff = "";
        //do{
            stat = read(this->SocketFD, &type_message_buff, 10);
        //}while(stat<0);

        printf("type_message_buff: %s\n", type_message_buff);

        if (type_message_buff == "request"){    

            int size_buffer = 0;
            printf("Waiting for request from SIZE REQUEST\n");
            do{
                stat = read(this->SocketFD, &size_buffer, sizeof(int));
            }while(stat<0);
            printf("size_buffer SS: %d\n", size_buffer);

            const char* message_buffer = "";
            printf("Waiting for request from SERVER MASTER\n");
            do{
                stat = read(this->SocketFD, &message_buffer, size_buffer);
                printf("stat: %d\n", stat);
            }while(stat<0);
            cout<<"message_buffer: "<<message_buffer<<endl;

            chars mess_unwrap(message_buffer);
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
            continue;
        }

        

        /*printf("Enter a message to server: ");
        scanf("%s" , this->message);
        chars messa = this->protocol->wrap("_n", "", this->message, "");

        n = write(this->SocketFD, messa.c_str(), messa.size());
        if (n < 0) perror("ERROR writing to socket");
        
        bzero(this->buffer, 255);
        this->message_server = read(this->SocketFD, this->buffer, 255);
        if (this->message_server < 0) perror("ERROR reading from socket");

        list<chars> test = this->protocol->unwrap(this->buffer);
        cout<<"Message of client:"<<endl;
        this->protocol->print_list_str(test);*/
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
