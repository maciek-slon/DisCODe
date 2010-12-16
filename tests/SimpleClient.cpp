#include "Network/TCPClient.hpp"

#include <cstring>

int main(int argc, char* argv[])
{
	Common::TCPClient client;
	client.connect("localhost", "30000");

	if (argc < 2)
		client.send("print:ala:ma:kota", 18);
	else
		client.send(argv[1], strlen(argv[1])+1);

	//std::string reply = client.recv();

	//std::cout << reply;

	return 0;
}
