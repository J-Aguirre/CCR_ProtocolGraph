//#include "messages.cpp"
#include "protocol.h"

int main(int argc, char const *argv[])
{
	/*cout << wrap_message("_n", "", "Peru", "synonyms:Ecuador,Chile,Uruguay") << endl;
	cout << wrap_message("_l", "", "Peru,Bolivia", "synonyms:Ecuador,Chile,Uruguay;synonyms:Ecuador,Chile,Uruguay") << endl;
	cout << wrap_message("_q", "2", "Peru", "") << endl;*/

    Protocol test;
    /*list<chars> unwrap_mess = test.unwrap("_n100080030Colombiasynonyms:Ecuador,Chile,Uruguay");
    cout<<endl<<"PARAMETERS TO SERVER"<<endl<<endl;
    test.print_list_str(unwrap_mess);
    //cout<<unwrap_mess<<endl;
*/
    //cout << wrap_message("_l", "", "Peru,Bolivia", "synonyms:Ecuador,Chile,Uruguay;synonyms:Ecuador,Chile,Uruguay") << endl;
    list<chars> unwrap_mess = test.unwrap("_l100120061Peru,Boliviasynonyms:Ecuador,Chile,Uruguay;synonyms:Ecuador,Chile,Uruguay");
    test.print_list_str(unwrap_mess);

	return 0;
}