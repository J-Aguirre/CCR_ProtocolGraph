#include <iostream>
#include <vector>
#include <math.h>
#include <map>
#include <string>
#include "messages.cpp"

using namespace std;

typedef string chars;
typedef list<chars> (*func)(chars);

class Protocol{

    public:
        // char* message;
        int size_header_type_message;
        int type_message; // 8 bits in decimal, so max of 255
        map<chars, func> type_messages_envelop; //<val_char*_of_func, func>
        map<chars, func> type_messages_unwrap; // <value_int_of_func, func>

        Protocol();

        chars wrap(chars, chars, chars, chars);

        // (data)
        list<chars> unwrap(chars);
        
        void print_list_str(list<chars>);

};

Protocol::Protocol(){
    this->type_messages_unwrap["_n"] = unwrap_new_node;
    this->type_messages_unwrap["_l"] = unwrap_new_link;
    this->type_messages_unwrap["_q"] = unwrap_query_deepness;
    this->type_messages_unwrap["_p"] = unwrap_sentence_deepness;
    this->type_messages_unwrap["_c"] = unwrap_info_node;
    this->type_messages_unwrap["_s"] = unwrap_server_online;

    this->type_messages_unwrap["nn"] = unwrap_node_link_answer;
    this->type_messages_unwrap["ll"] = unwrap_node_link_answer;
    this->type_messages_unwrap["qq"] = unwrap_query_answer;
    this->type_messages_unwrap["pp"] = unwrap_sentence_answer;
    /*this->type_messages_unwrap["cc"] = unwrap_info_answer;
    this->type_messages_unwrap["ss"] = unwrap_servers_answer;*/

}

chars Protocol::wrap(chars action, chars deepness, chars data, chars attribu){

    return wrap_message(action, deepness, data, attribu);
}

void Protocol::print_list_str(list<chars> msgs){
    for (auto v : msgs)
        cout << v << "\n";
}

list<chars> Protocol::unwrap(chars data){

    chars action = data.substr(0, 2);
    map<chars, func>::iterator it;
    // searching a func in map of type of messages to unwrap with func found
    it = this->type_messages_unwrap.find(action);

    return it->second(data);
}

