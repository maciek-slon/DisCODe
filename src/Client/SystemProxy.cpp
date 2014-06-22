#include "SystemProxy.hpp"
#include "DisCODeClient.hpp"

#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>

namespace DisCODe {

SystemProxy::SystemProxy(Client * client) : m_client(client), m_loglvl(-1) {

}

void SystemProxy::finish() {
	std::string str = m_client->send("kill");
}

int SystemProxy::getLogLvl() {
	if (m_loglvl >= 0) return m_loglvl;

	std::string str = m_client->send("getLogLvl");
	m_loglvl = boost::lexical_cast<int>(str);
	return m_loglvl;
}



}
