//#include "messages.cpp"
#include "protocol.h"

int main(int argc, char const *argv[])
{

    Protocol test;

	cout << test.wrap("_n", "", "Peru", "synonyms:Ecuador,Chile,Uruguay") << endl;
	cout << test.wrap("_l", "", "Peru,Bolivia", "synonyms:Ecuador,Chile,Uruguay;synonyms:Ecuador,Chile,Uruguay;antonyms:Cusco,Inka,Peruvian;antonyms:Veracruz,Solis,Habana") << endl;
	cout << test.wrap("_q", "2", "Peru", "") <<endl<<endl;
    cout << test.wrap("_p", "2", "Peru,Colombia,Ecuador", "synonyms,antonyms") <<endl<<endl;

    /*list<chars> unwrap_mess = test.unwrap("_q200040000Peru");*/
    //list<chars> unwrap_mess = test.unwrap("_l100120061Peru,Boliviasynonyms:Ecuador,Chile,Uruguay;synonyms:Ecuador,Chile,Uruguay");
    list<chars> unwrap_mess = test.unwrap("_p200210017Peru,Colombia,Ecuadorsynonyms,antonyms");
    cout<<endl<<"PARAMETERS TO SERVER"<<endl<<endl;
    test.print_list_str(unwrap_mess);

    cout<<endl<<endl;
    /*list<chars> splited = splitt("hello,mother,fucker,this,is,working", ',');
    print_vec_str(splited);*/

	return 0;
}