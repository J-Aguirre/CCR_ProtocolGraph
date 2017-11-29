/*
    PLEASE READ THE FILE "protocol.txt" WHERE IS DEFINED THE STRUCTURE OF EACH TYPE OF MESSAGE OF THIS FILE !!!!!!!

    (S) --> begin section of comment
    (E) --> end section of comment
*/

#include <sstream>
#include <string.h>
#include <iostream>
#include <list>

#define FIELD_WORD_SIZE 4
#define FIELD_ATRIBUTE_SIZE 4
#define ACTION_NEW_NODE "_n"

using namespace std;

//typedef char const* chars;
typedef string chars;
const int DIGITS_TO_SIZE_IN_BYTES_ALL = 6; // this variable is for all type of message, because is the number of digits where the size of ALL message is 

void print_list_str(list<chars> words){
    for (auto v : words)
        cout << v << "\n";
}

list<chars> splitt(chars s, char c)
{
    chars buff = "";
    list<string> v;
    
    for(auto n:s)
    {
        if(n != c) buff+=n; else
        if(n == c && buff != "") { v.push_back(buff); buff = ""; }
    }
    if(buff != "") v.push_back(buff);
    
    return v;
}

int number_digits(int number){
    int digits = 0;
    if(number == 0) return 1;
    while (number != 0) { number /= 10; digits++; }

    return digits;
}

chars fill_zeros(int number, int how_many){
    stringstream strs;
    for (int i = 0; i < how_many; ++i)
        strs<<"0";
    strs<<number;

    return strs.str();
}

chars wrap_message(chars action, chars deepness, chars word, chars attributes){
    if (deepness == "")
        deepness = "1";
    int word_size = word.size();
    int attribute_size = attributes.size();
    int word_number = FIELD_WORD_SIZE - number_digits(word_size);
    int attribute_number = FIELD_ATRIBUTE_SIZE - number_digits(attribute_size);

    return action + deepness + fill_zeros(word_size, word_number) + fill_zeros(attribute_size, attribute_number) + word + attributes;
}


//list:<action, word, attributes>
list<chars> unwrap_new_node(chars message){
    list<chars> answer;
    chars action = message.substr(0, 2);
    answer.push_back(action);

    chars word_size_str = message.substr(3, 4);
    int word_size = stoi(word_size_str);
    cout<<"word_size: "<<word_size<<endl;
    chars word = message.substr(11, word_size);
    cout<<"word: "<<word<<endl;
    answer.push_back(word);

    chars attributes_size_str = message.substr(9, 4);
    int attributes_size = stoi(attributes_size_str);
    cout<<"attributes_size: "<<attributes_size<<endl;
    chars attributes = message.substr(11 + word_size, attributes_size);
    cout<<"attributes: "<<attributes<<endl;
    answer.push_back(attributes);

    return answer;
}

// the first 2 will be the principal words (word1,word2), the next will be the attributes one for eachother
// word1 => attribute1.1, attribute 1.2 ...
// word2 => attribute2.1, attribute 2.2 ...
// <Valu1, value2, attribute1.1, attribute2.1, attribute1.2, attribute2.2 ...>
list<chars> unwrap_new_link(chars message){
    list<chars> answer, answer1, answer2;
    chars action = message.substr(0, 2);

    chars words_size_str = message.substr(3,4);
    int words_size = stoi(words_size_str);
    chars attributes_size_str = message.substr(5,4);
    int attributes_size = stoi(attributes_size_str);

    message = message.substr(11);
    chars words = message.substr(0,words_size);

    chars attributes = message.substr(words_size,attributes_size);
    answer = splitt(words,',');
    answer.push_front(action);
    answer2 = splitt(attributes,';');
    answer.merge(answer2);
    print_list_str(answer);

    return answer;
}

// list:<action, deepness, word>
list<chars> unwrap_query_deepness(chars message){
    list<chars> answer;
    chars action = message.substr(0, 2);
    answer.push_back(action);

    chars deepness = message.substr(2, 1);
    answer.push_back(deepness);

    chars word_size_str = message.substr(3, 4);
    int word_size = stoi(word_size_str);
    //cout<<"word_size: "<<word_size<<endl;
    chars word = message.substr(11, word_size);
    //cout<<"word: "<<word<<endl;
    answer.push_back(word);

    return answer;    
}

//list: <antion, word>
list<chars> unwrap_info_node(chars message){
    list<chars> answer;
    chars action = message.substr(0, 2);
    answer.push_back(action);

    chars word_size_str = message.substr(3, 4);
    int word_size = stoi(word_size_str);
    cout<<"word_size: "<<word_size<<endl;
    chars word = message.substr(11, word_size);
    answer.push_back(word);

    return answer;
}

//list: <action, deepness, ,data, data, ... ,attr, attr, ... >
list<chars> unwrap_sentence_deepness(chars message){
    list<chars> words_l, attributes_l;
    chars action = message.substr(0, 2);
    chars deepness = message.substr(2, 1);

    chars word_size_str = message.substr(3, 4);
    int word_size = stoi(word_size_str);
    cout<<"word_size: "<<word_size<<endl;
    chars word = message.substr(11, word_size);

    chars attributes_size_str = message.substr(7,4);
    cout<<"attributes_size_str: "<<attributes_size_str<<endl;
    int attributes_size = stoi(attributes_size_str);
    cout<<"attributes_size: "<<attributes_size<<endl;
    chars attributes = message.substr(11 + word_size, attributes_size);
    cout << "attributes: " << attributes << endl;

    words_l =  splitt(word, ',');
    attributes_l = splitt(attributes, ',');
    words_l.merge(attributes_l);
    words_l.push_front(deepness);
    words_l.push_front(action);

    return words_l;
}


//list: <antion, word>
list<chars> unwrap_server_online(chars message){
    list<chars> answer;
    chars action = message.substr(0, 2);
    answer.push_back(action);

    return answer;
}

//list: <action, state>
// State would be "Ok" and "Error" identified as 1 or 0
// the 0 and 1 are setted and read in Deepness.
list<chars> unwrap_node_link_answer(chars message){
    list<chars> answer;
    chars action = message.substr(0, 2);
    answer.push_back(action);

    answer.push_back(message.substr(2,1));

    return answer;
}

list<chars> unwrap_query_answer(chars message){
    list<chars> answer;
    chars action = message.substr(0, 2);

    chars deepness = message.substr(2, 1);
    cout<<"initial deepness: "<<deepness<<endl;
    int deepness_number = stoi(deepness) - 1;
    deepness = to_string(deepness_number);
    cout<<"new deepness: "<<deepness<<endl;
    

    chars word_size_str = message.substr(3, 4);
    int word_size = stoi(word_size_str);
    chars attributes_size_str = message.substr(7, 4);
    int attributes_size = stoi(attributes_size_str);

    message = message.substr(11);
    chars words = message.substr(0, word_size);
    answer = splitt(words, ',');

    chars attributes = message.substr(word_size, attributes_size);
    list<chars> answer_attributes = splitt(attributes, ';');
    answer.merge(answer_attributes);
    answer.push_front(deepness);
    answer.push_front(action);

    return answer;
}

list<chars> unwrap_sentence_answer(chars message){
    list<chars> answer;
    chars action = message.substr(0, 2);

    chars deepness = message.substr(2, 1);
    cout<<"initial deepness: "<<deepness<<endl;
    int deepness_number = stoi(deepness) - 1;
    deepness = to_string(deepness_number);
    cout<<"new deepness: "<<deepness<<endl;
    

    chars word_size_str = message.substr(3, 4);
    int word_size = stoi(word_size_str);
    chars attributes_size_str = message.substr(7, 4);
    int attributes_size = stoi(attributes_size_str);

    message = message.substr(11);
    chars words = message.substr(0, word_size);
    answer = splitt(words, ';');

    chars attributes = message.substr(word_size, attributes_size);
    list<chars> answer_attributes = splitt(attributes, ';');
    answer.merge(answer_attributes);
    answer.push_front(deepness);
    answer.push_front(action);

    return answer;
}