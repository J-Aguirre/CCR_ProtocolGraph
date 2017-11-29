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
        int SocketFD;
        int port;

        char message[255];
        int n;

        int packet_size;
        int header_size;
        chars path_bigramas;
        chars path_wordnet;
        Protocol* protocol;
        Connection* db;
        map<int, int> table_clients; // <name_number, number_socket>

        Server();
        // port, header_size, packet_size,
        Server(int);        
        void print_table_clients();
        void connection();
        void new_client_connection(int);
        int split(const string txt, vector<string> &strs, char ch);
        int print_vec_s(vector<string>);
        void send_data_to_server(int socket, string bigrama);
        void load_data();
};

Server::Server(){}

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


void Server::print_table_clients(){
    map<int, int>::iterator it;
    printf("********SERVERS CONNECTED********* \n");
    printf("----------------------------------\n");
    printf("Number name of server | Num Socket \n");

    for(it=this->table_clients.begin(); it!=this->table_clients.end(); it++) {
        printf("%10d | %2d \n", it->first, it->second);
    }
}


void Server::new_client_connection(int connect_id){
    for(;;)
    {
    // do
    // {

        /*printf("DIGITS_TO_SIZE_IN_BYTES_ALL: %d\n", DIGITS_TO_SIZE_IN_BYTES_ALL);
        char header_buffer[DIGITS_TO_SIZE_IN_BYTES_ALL]; 
        bzero(header_buffer, DIGITS_TO_SIZE_IN_BYTES_ALL);
        n = read(connect_id, header_buffer, DIGITS_TO_SIZE_IN_BYTES_ALL);
        if (n < 0) perror("ERROR reading from socket");
        int int_header_buffer = 255;
        try{
            int_header_buffer = chars_to_int(header_buffer);
            printf("int_header_buffer: %d\n", int_header_buffer);
        }
        catch (int e)
        {
            printf("An exception occurred. Exception Nr. %d\n", e);            
            printf("header_buffer: %s\n", header_buffer);
        }*/

        int int_header_buffer = DEFAUL_SIZE;
        char buffer[int_header_buffer];
        bzero(buffer, int_header_buffer);
        n = read(connect_id, buffer, int_header_buffer);
        if (n < 0) perror("ERROR reading from socket");
        /*printf("buffer: %s\n", buffer);
        printf("sizeof buffer: %d\n", sizeof(buffer));*/
        chars mess_unwrap(buffer);
        cout<<"mess_unwrap: "<<mess_unwrap<<endl;
        list<chars> test = this->protocol->unwrap(mess_unwrap);
        cout<<"Message of client:"<<endl;
        this->protocol->print_list_str(test);
        int var = 0;
        chars word = "";
        for (auto v : test){
            var++;
            if (var == 2){
                word = v;
            }
        }
        /*auto test_front = test.begin();
        advance(test_front, 2);*/
        cout<<"word: "<<word<<endl;
        this->db->insert_node(word);

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
        printf("Client connected !!! \n");
        int size_table_clients = table_clients.size();
        table_clients[size_table_clients + 1] = ConnectFD;
        this->print_table_clients();


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

void Server::load_data(){
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

            int socket_id = this->table_clients[server_counter];
            printf("socket_id: %d\n", socket_id);
            this->send_data_to_server(socket_id, line);
            previous_first_word = line_vec[0];

            // printf ("%s\n", line.c_str());
        }
        bigramas.close();
    }
}