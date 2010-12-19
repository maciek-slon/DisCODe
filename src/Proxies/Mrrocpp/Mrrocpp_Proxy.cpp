/*!
 * \file Mrrocpp_Proxy.cpp
 * \brief MRROC++ Proxy.
 *
 * \date 15-10-2010
 * \author Mateusz Boryn
 */

#include <cstdio>
#include <sys/uio.h>

#include "Mrrocpp_Proxy.hpp"

namespace Proxies {
namespace Mrrocpp {

using namespace std;
using namespace boost::interprocess;

Mrrocpp_Proxy::Mrrocpp_Proxy(const std::string & name) :
	Base::Component(name), state(MPS_NOT_INITIALIZED)
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

	h_onNewReading.setup(this, &Mrrocpp_Proxy::onNewReading);
	registerHandler("onNewReading", &h_onNewReading);
	registerStream("reading", &reading);

	rpcCall = registerEvent("rpcCall");
	registerStream("rpcParam", &rpcParam);
	registerStream("rpcResult", &rpcResult);
	h_onRpcResult.setup(this, &Mrrocpp_Proxy::onRpcResult);
	registerHandler("onRpcResult", &h_onRpcResult);

	serverSocket.setupServerSocket(props.port);
	clientConnected = false;

	readingMessage.reset();
	rpcResultMessage.reset();

	state = MPS_LISTENING;

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
	serverSocket.closeSocket();
	clientConnected = false;

	readingMessage.reset();
	rpcResultMessage.reset();

	state = MPS_NOT_INITIALIZED;

	return true;
}

bool Mrrocpp_Proxy::onStep()
{
	scoped_lock <interprocess_mutex> lock(eventsMutex);
	LOG(LTRACE) << "Mrrocpp_Proxy::onStep\n";

	switch (state)
	{
		case MPS_LISTENING:
			tryAcceptConnection();
			break;
		case MPS_CONNECTED:
			tryReceiveFromMrrocpp();
			break;
		default:
			throw logic_error("Mrrocpp_Proxy::onStep(): wrong state");
	}

	if (clientConnected) {
		LOG(LTRACE) << "if(clientConnected){\n";

		switch (proxyState)
		{
			case PROXY_WAITING_FOR_COMMAND:

				break;
			case PROXY_WAITING_FOR_READING:
				return true;
				break;
			case PROXY_WAITING_FOR_RPC_RESULT:
				return true;
				break;
		}
	}
	return true;
}

void Mrrocpp_Proxy::tryAcceptConnection()
{
	LOG(LTRACE) << "Mrrocpp_Proxy::tryAcceptConnection()\n";
	if (!serverSocket.isDataAvailable()) {
		LOG(LTRACE) << "if (!serverSocket.isDataAvailable()) {\n";
		return;
	}
	clientSocket = serverSocket.acceptConnection();
	readingMessage.reset();
	rpcResultMessage.reset();
	LOG(LTRACE) << "clientConnected!!!!\n";
	state = MPS_CONNECTED;
}

void Mrrocpp_Proxy::tryReceiveFromMrrocpp()
{
	try {
		if (clientSocket->isDataAvailable()) {
			receiveBuffersFromMrrocpp();
			if (imh.is_rpc_call) {
					rpcParam.write(*iarchive); // send RPC param
					rpcCall->raise();
					state = MPS_WAITING_FOR_RPC_RESULT; // wait for RPC result
				} else {
					oarchive->clear_buffer();
					if (readingMessage.get() != 0) { // there is no reading ready
						rmh.is_rpc_call = false;
						readingMessage->send(oarchive);
					}

					sendBuffersToMrrocpp();
					readingMessage.reset();
				}
		}
	} catch (exception& ex) {
		LOG(LERROR) << "Mrrocpp_Proxy::tryReceiveFromMrrocpp(): Probably client disconnected: "
				<< ex.what() << endl;
		LOG(LERROR) << "Closing socket.\n";
		clientSocket->closeSocket();
		state= MPS_LISTENING;
	}
}

void Mrrocpp_Proxy::onNewReading()
{
	scoped_lock <interprocess_mutex> lock(eventsMutex);
	LOG(LNOTICE) << "Mrrocpp_Proxy::onNewReading ehehehehehehs\n";
	readingMessage = reading.read();
	readingMessage->printInfo();
	//	if (proxyState == PROXY_WAITING_FOR_READING) {
	//		LOG(LNOTICE) << "Mrrocpp_Proxy::onNewReading(): proxyState == PROXY_WAITING_FOR_READING\n";
	//		rmh.is_rpc_call = false;
	//
	//		oarchive->clear_buffer();
	//		readingMessage->send(oarchive);
	//
	//		sendBuffersToMrrocpp();
	//
	//		readingMessage.reset();
	//		proxyState = PROXY_WAITING_FOR_COMMAND;
	//	} else {
	//		LOG(LNOTICE) << "Mrrocpp_Proxy::onNewReading(): proxyState != PROXY_WAITING_FOR_READING\n";
	//	}
}

void Mrrocpp_Proxy::onRpcResult()
{
	scoped_lock <interprocess_mutex> lock(eventsMutex);
	LOG(LTRACE) << "Mrrocpp_Proxy::onRpcResult\n";
	rpcResultMessage = rpcResult.read();

	if (proxyState != PROXY_WAITING_FOR_RPC_RESULT) {
		LOG(LFATAL) << "Mrrocpp_Proxy::onRpcResult(): proxyState != PROXY_WAITING_FOR_RPC_RESULT\n";
		return;
	}

	rmh.is_rpc_call = true;

	oarchive->clear_buffer();
	*oarchive << (*rpcResultMessage);

	sendBuffersToMrrocpp();

	proxyState = PROXY_WAITING_FOR_COMMAND;
}

void Mrrocpp_Proxy::receiveCommand()
{
	LOG(LINFO) << "Mrrocpp_Proxy::receiveCommand() begin: proxyState = " << proxyState << "\n";
	receiveBuffersFromMrrocpp();


	LOG(LINFO) << "Mrrocpp_Proxy::receiveCommand() end: proxyState = " << proxyState << "\n";
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

	*header_iarchive >> imh;

	iarchive->clear_buffer();
	clientSocket->read(iarchive->get_buffer(), imh.data_size);

	LOG(LDEBUG) << "imh.data_size: " << imh.data_size << endl;

}

void Mrrocpp_Proxy::sendBuffersToMrrocpp()
{
	LOG(LTRACE) << "sendBuffersToMrrocpp() begin\n";

	rmh.data_size = oarchive->getArchiveSize();

	header_oarchive->clear_buffer();
	*header_oarchive << rmh;

	clientSocket->writev2(header_oarchive->get_buffer(), header_oarchive->getArchiveSize(), oarchive->get_buffer(), oarchive->getArchiveSize());
	header_oarchive->clear_buffer();
	oarchive->clear_buffer();
}

} // namespace Mrrocpp {

} // namespace Proxies


