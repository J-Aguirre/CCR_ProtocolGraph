//#include "messages.cpp"
#include "protocol.h"

int main(int argc, char const *argv[])
{

    Protocol test;

	cout << test.wrap("_n", "", "Peru", "synonyms:Ecuador,Chile,Uruguay") << endl;
	cout << test.wrap("_l", "", "Peru,Bolivia", "synonyms:Ecuador,Chile,Uruguay;synonyms:Ecuador,Chile,Uruguay") << endl;
	cout << test.wrap("_q", "2", "Peru", "") <<endl<<endl;

    //list<chars> unwrap_mess = test.unwrap("_q200040000Peru");
    //list<chars> unwrap_mess = test.unwrap("	_l100120061Peru,Boliviasynonyms:Ecuador,Chile,Uruguay;synonyms:Ecuador,Chile,Uruguay");
    //cout<<endl<<"PARAMETERS TO SERVER"<<endl<<endl;
    //test.print_list_str(unwrap_mess);

    cout<<endl<<endl;
    list<chars> unwrap_mess = test.unwrap("_c100040000Peru");
    test.print_list_str(unwrap_mess);
    /*list<chars> splited = splitt("hello,mother,fucker,this,is,working", ',');
    print_vec_str(splited);*/

	return 0;
}