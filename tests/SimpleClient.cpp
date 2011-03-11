#include "Network/TCPClient.hpp"

#include <cstring>
#include <iostream>

int check(const unsigned char * msg, int size) {
//	std::cout << "Check...\n";
//	int ss = msg[0] * 256 + msg[1];
//	std::cout << "Computed: " << ss << ", got: " << size << std::endl;
//	std::cout << msg+2 << std::endl;
	return size > 1 ? msg[0] * 256 + msg[1] : 2;
}

int service(const unsigned char * msg, int size) {
//	std::cout << "Service...\n";
	std::cout << msg+2 << std::endl;
	return 0;
}

int main(int argc, char* argv[])
{
	Common::TCPClient client;
	client.setServiceHook(service);
	client.setCompletionHook(check);
	client.connect("localhost", "30000");

	unsigned char buf[256];
	std::string msg;

	if (argc < 2)
		msg = "print:ala:ma:kota";
	else
		msg = argv[1];

	buf[0] = msg.size() / 256;
	buf[1] = msg.size() % 256;

	strcpy((char*)buf+2, msg.c_str());
	buf[msg.size()+2] = 0;

	client.send(buf, msg.size()+3);

	std::cout << "Sent. Waiting for reply...\n";

	client.recv();

	return 0;
}
