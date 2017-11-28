//#include "messages.cpp"
#include "protocol.h"
#include "connection.h"

int main(int argc, char const *argv[])
{

  Connection connect;
  // connect.insert_node("chile");
  // connect.find_node("chile");
  // connect.update_node("chiles", "chile");
  connect.delete_node("chile");

	return 0;
}
