#include <iostream>

#include "../Core/ConnectionManager.hpp"

using namespace std;

int main()
{
	Core::ConnectionManager & conns = Core::ConnectionManager::instance();

	conns.get("Con_1");

    cout << "Hello world!" << endl;
    return 0;
}

