/*
 * Mrrocpp_Proxy.cpp
 *
 *  Created on: 15-10-2010
 *      Author: mateusz
 */

#include <cstdio>
#include <sys/uio.h>

#include "Mrrocpp_Proxy.hpp"

namespace Proxies {

namespace Mrrocpp {

Mrrocpp_Proxy::Mrrocpp_Proxy(const std::string & name) :
	Base::Component(name), sample(0xAA, 0xAA, "qwer", "aaa"), clientConnected(false)
{
	LOG(LFATAL) << "Mrrocpp_Proxy::Mrrocpp_Proxy\n";

	header_iarchive = boost::shared_ptr <xdr_iarchive <> >(new xdr_iarchive <> );
	iarchive = boost::shared_ptr <xdr_iarchive <> >(new xdr_iarchive <> );
	header_oarchive = boost::shared_ptr <xdr_oarchive <> >(new xdr_oarchive <> );
	oarchive = boost::shared_ptr <xdr_oarchive <> >(new xdr_oarchive <> );

	// determine imh size in XDR format
	initiate_message_header rmh;
	*header_oarchive << rmh;
	initiate_message_header_size = header_oarchive->getArchiveSize();
	header_oarchive->clear_buffer();

}

Mrrocpp_Proxy::~Mrrocpp_Proxy()
{
	LOG(LFATAL) << "Mrrocpp_Proxy::~Mrrocpp_Proxy\n";
}

bool Mrrocpp_Proxy::onStart()
{
	LOG(LFATAL) << "Mrrocpp_Proxy::onStart\n";
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
	LOG(LFATAL) << "Mrrocpp_Proxy::onStop\n";
	return true;
}

bool Mrrocpp_Proxy::onFinish()
{
	LOG(LFATAL) << "Mrrocpp_Proxy::onFinish\n";
	return true;
}

bool Mrrocpp_Proxy::onStep()
{
	LOG(LTRACE) << "Mrrocpp_Proxy::onStep\n";

	if (clientConnected) {
		LOG(LTRACE) << "if(clientConnected){\n";
		if (!clientSocket->isDataAvailable()) {
			return true;
		}
		receiveBuffersFromMrrocpp();

		string s;

		*iarchive >> s;

		LOG(LDEBUG) << "iarchive >> s: \"" << s << "\"\n";

		*oarchive << string("Mrrocpp_Proxy::onStep()");

		sendBuffersToMrrocpp();

	} else {
		LOG(LTRACE) << "if(!clientConnected){\n";
		if (!serverSocket.isDataAvailable()) {
			LOG(LTRACE) << "if (!serverSocket.isDataAvailable()) {\n";
			return true;
		}
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

void Mrrocpp_Proxy::receiveBuffersFromMrrocpp()
{
	LOG(LTRACE) << "Mrrocpp_Proxy::receiveBuffersFromMrrocpp() begin\n";
	header_iarchive->clear_buffer();
	clientSocket->read(header_iarchive->get_buffer(), initiate_message_header_size);

	initiate_message_header imh;
	*header_iarchive >> imh;

	iarchive->clear_buffer();
	clientSocket->read(iarchive->get_buffer(), imh.data_size);

	LOG(LDEBUG) << "imh.data_size: " << imh.data_size << endl;
}

void Mrrocpp_Proxy::sendBuffersToMrrocpp()
{
	LOG(LTRACE) << "sendBuffersToMrrocpp() begin\n";

	reading_message_header rmh;
	rmh.data_size = oarchive->getArchiveSize();
	rmh.is_rpc_call = false;

	*header_oarchive << rmh;

	clientSocket->writev2(header_oarchive->get_buffer(), header_oarchive->getArchiveSize(), oarchive->get_buffer(), oarchive->getArchiveSize());
	header_oarchive->clear_buffer();
	oarchive->clear_buffer();
}

} // namespace Mrrocpp {

} // namespace Proxies


