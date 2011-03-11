#include "SystemProxy.hpp"
#include "DisCODeClient.hpp"

#include <boost/foreach.hpp>

namespace DisCODe {

SystemProxy::SystemProxy(Client * client) : m_client(client) {

}

void SystemProxy::finish() {
	std::string str = m_client->send("kill");
}



}
