#include "Network/TCPClient.hpp"

#include <cstring>
#include <iostream>

int check(const char * msg, int size) {
	return size>0?size:1;
}

int service(const char * msg, int size) {
	std::cout << msg << std::endl;
	return 0;
}

int main(int argc, char* argv[])
{
	Common::TCPClient client;
	client.connect("localhost", "30000");

	if (argc < 2)
		client.send("print:ala:ma:kota", 18);
	else
		client.send(argv[1], strlen(argv[1])+1);

	char buf[256];
	client.recv();

	std::cout << buf << std::endl;

	return 0;
}
