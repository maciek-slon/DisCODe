#include "Network/TCPClient.hpp"

int main(int argc, char* argv[])
{
	Common::TCPClient client;
	client.connect("localhost", "30000");

	if (argc < 2)
		client.send("print:ala:ma:kota");
	else
		client.send(argv[1]);

	std::string reply = client.recv();

	std::cout << reply;

	return 0;
}
