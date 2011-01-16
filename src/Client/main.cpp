#include "TaskProxy.hpp"
#include "DisCODeClient.hpp"

int main() {
	DisCODe::Client client;
	DisCODe::TaskProxy proxy(&client);

	proxy.refresh();
	proxy.print();

	return 0;
}
