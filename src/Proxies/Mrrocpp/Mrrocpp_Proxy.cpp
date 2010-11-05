/*
 * Mrrocpp_Proxy.cpp
 *
 *  Created on: 15-10-2010
 *      Author: mateusz
 */

#include <cstdio>

#include "Mrrocpp_Proxy.hpp"

namespace Proxies {

namespace Mrrocpp {

Mrrocpp_Proxy::Mrrocpp_Proxy(const std::string & name) :
	Base::Component(name), sample(0xAA, 0xAA, "qwer", "aaa"), clientConnected(false)
{
	LOG(LFATAL)<<"Mrrocpp_Proxy::Mrrocpp_Proxy\n";
}

Mrrocpp_Proxy::~Mrrocpp_Proxy()
{
	LOG(LFATAL)<<"Mrrocpp_Proxy::~Mrrocpp_Proxy\n";
}

bool Mrrocpp_Proxy::onStart()
{
	LOG(LFATAL)<<"Mrrocpp_Proxy::onStart\n";
	return true;
}

bool Mrrocpp_Proxy::onInit()
{
	LOG(LTRACE) << "Mrrocpp_Proxy::onInit\n";

	h_onNewMsgToSend.setup(this, &Mrrocpp_Proxy::onNewMsgToSend);
	registerHandler("onNewMsgToSend", &h_onNewMsgToSend);

	newMsgReceived = registerEvent("newMsgReceived");

	registerStream("msgToSend", &msgToSend);
	registerStream("msgReceived", &msgReceived);

	serverSocket.setupServerSocket(props.port);
	clientConnected = false;

	return true;
}

bool Mrrocpp_Proxy::onStop()
{
	LOG(LFATAL)<<"Mrrocpp_Proxy::onStop\n";
	return true;
}

bool Mrrocpp_Proxy::onFinish()
{
	LOG(LFATAL)<<"Mrrocpp_Proxy::onFinish\n";
	return true;
}

bool Mrrocpp_Proxy::onStep()
{
	LOG(LFATAL) << "Mrrocpp_Proxy::onStep\n";

	if (!serverSocket.isDataAvailable()) {
		LOG(LTRACE) << "if (!serverSocket.isDataAvailable()) {\n";
		return true;
	}
	if(clientConnected){
		LOG(LFATAL) << "if(clientConnected){\n";
	} else {
		LOG(LFATAL) << "if(!clientConnected){\n";
		clientSocket = serverSocket.acceptConnection();
		clientConnected = true;
		LOG(LFATAL) << "clientConnected!!!!\n";
	}
	return true;
}

void Mrrocpp_Proxy::onNewMsgToSend()
{
	msgToSend.read();
	LOG(LTRACE) << "Mrrocpp_Proxy::onNewMsgToSend\n";
}

Base::Props * Mrrocpp_Proxy::getProperties()
{
	return &props;
}

} // namespace Mrrocpp {

} // namespace Proxies


