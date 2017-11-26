//#include "messages.cpp"
#include "protocol.h"

int main(int argc, char const *argv[])
{
	/*cout << wrap_message("_n", "", "Peru", "synonyms:Ecuador,Chile,Uruguay") << endl;
	cout << wrap_message("_l", "", "Peru,Bolivia", "synonyms:Ecuador,Chile,Uruguay;synonyms:Ecuador,Chile,Uruguay") << endl;
	cout << wrap_message("_q", "2", "Peru", "") << endl;*/

    Protocol test;
    //chars unwrap_mess = test.unwrap("_n100040030Perusynonyms:Ecuador,Chile,Uruguay");
    //cout<<unwrap_mess<<endl;
    chars unwrap_mess = test.unwrap("_l100120030Peru,Boliviasynonyms:Ecuador,Chile,Uruguay");
    cout<<unwrap_mess<<endl;    

	return 0;
}