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

        // (total_message)
        // chars all_message_size(chars);

        // (action, data) 
        chars wrap(chars, chars, chars, chars);

        // (data)
        list<chars> unwrap(chars);

        // transform first character to bits
        /*vector<int> transform_char_to_bits(unsigned char);

        int transform_bits_to_decimal(vector<int>);
        void print_binary(vector<int>);*/
        void print_list_str(list<chars>);

};

Protocol::Protocol(){
    /*this->size_header_type_message = 8; // 8 bits in decimal, so max of 255
    this->type_messages_envelop["simple-message"] = simple_message_env; // key for this envelop func is "simple-message" 
    this->type_messages_unwrap[1] = simple_message_unwr; // key for this unwarp func is "1"*/
    this->type_messages_unwrap["_n"] = unwrap_new_node;
    this->type_messages_unwrap["_l"] = unwrap_new_link;
    this->type_messages_unwrap["_q"] = unwrap_query_deepness;
    this->type_messages_unwrap["_c"] = unwrap_info_node;
}/**/
/*
void Protocol::print_binary(vector<int> v){
    for(int i=0; i<v.size(); i++)
        cout<<v[i]<<" ";
}

// Is saving in reverse way
vector<int> Protocol::transform_char_to_bits(unsigned char type){
    vector<int> binary;
    for(int i = 0; i < this->size_header_type_message; i++)
        binary.push_back((type >> i) & 1 );
    //this->print_binary(binary);

    return binary;
}

int Protocol::transform_bits_to_decimal(vector<int> v){
    int result = 0;
    int j=0;
    for(int i=0; i<v.size(); i++)
    {
        if(v[i])
            result += pow(2, i);
    }

    return result;
}


chars Protocol::all_message_size(chars total_messa){
    // int SIZE_ALL = strlen(total_messa) + DIGITS_TO_SIZE_IN_BYTES_ALL;
    int SIZE_ALL = strlen(total_messa);
    int n_digits_SIZE_ALL = number_digits(SIZE_ALL);
    chars MAIN_HEADER = fill_zeros(SIZE_ALL, DIGITS_TO_SIZE_IN_BYTES_ALL - n_digits_SIZE_ALL);

    return MAIN_HEADER;
}
*/

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

