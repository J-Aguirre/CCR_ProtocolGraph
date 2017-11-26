#include "messages.cpp"

int main(int argc, char const *argv[])
{
	cout << wrap_message("_n", "", "Peru", "synonyms:Ecuador,Chile,Uruguay") << endl;
	cout << wrap_message("_l", "", "Peru,Bolivia", "synonyms:Ecuador,Chile,Uruguay;synonyms:Ecuador,Chile,Uruguay") << endl;
	cout << wrap_message("_q", "2", "Peru", "") << endl;
	//cout << fill_zeros(4, 3);
	return 0;
}