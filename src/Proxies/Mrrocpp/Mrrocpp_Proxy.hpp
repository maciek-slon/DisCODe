/*!
 * \file Mrrocpp_Proxy.hpp
 * \brief MRROC++ Proxy.
 *
 * \date 15-10-2010
 * \author Mateusz Boryn
 */

#ifndef MRROCPP_PROXY_HPP_
#define MRROCPP_PROXY_HPP_

#include <ctime>

#include <boost/shared_ptr.hpp>
#include <boost/thread/mutex.hpp>

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "Panel_Empty.hpp"
#include "Property.hpp"
#include "xdr/xdr_oarchive.hpp"
#include "xdr/xdr_iarchive.hpp"
#include "Socket.hpp"
#include "headers.h"
#include "Mrrocpp_Proxy/Reading.hpp"

/*!
 * \defgroup Mrrocpp_Proxy Mrrocpp_Proxy
 * \ingroup Proxies
 *
 * Proxy to MRROC++.
 * This proxy listens on specified TCP/IP port for connection from MRROC++.
 * Only one incoming client connection at a time will be accepted.
 * Then Proxies::Mrrocpp::Mrrocpp_Proxy will wait for requests from MRROC++.
 * There are two types of requests: request for reading and RPC call.
 *
 * Request for reading makes Mrrocpp_Proxy to send as soon as possible latest reading.
 * If there is no reading then empty messege is sent back to MRROC++
 * to let MRROC++ know that there was no reading.
 *
 * RPC call is a call that makes Mrrocpp_Proxy generate rpcCall event and wait for onRpcResult.
 * Between generating rpcCall and receiving onRpcResult no reading requests will be received
 * and no readings will be sent to MRROC++.
 *
 * This is SDL diagram of Mrrocpp_Proxy on initializing.
 * After Mrrocpp_Proxy::onInit() has been called, proxy waits for client to connect.
 * When new client has connected, proxy changes its state to MPS_CONNECTED.
 *
\htmlonly
<img src="images/visual_servoing/automat_discode_MPS_NOT_INITIALIZED.png" style="margin: 5px; " alt=""/>
\endhtmlonly
 *
 * This is SDL diagram of processing requests from MRROC++.
 * When proxy is in MPS_CONNECTED state, it waits for request for reading or RPC call.
 * If reading request has been received, then reply is sent back.
 * If RPC call has been received, rpcCall event is raised and proxy waits for RPC result (onRpcResult event).
 *
\htmlonly
<img src="images/visual_servoing/automat_discode_MPS_CONNECTED.png" style="margin: 5px; " alt=""/>
\endhtmlonly
 *
 * When RPC result has been received, proxy sends it immediately to MRROC++.
 * Then proxy waits for another request.
\htmlonly
<img src="images/visual_servoing/automat_discode_MPS_WAITING_FOR_RPC_RESULT.png" style="margin: 5px; " alt=""/>
\endhtmlonly
 *
 * \par Data streams:
 *
 * \streamin{reading,Types::Mrrocpp_Proxy::Reading}
 * Reading to send to mrrocpp as soon as mrroc asks for it.
 *
 * \streamin{rpcResult,Types::Mrrocpp_Proxy::Reading}
 * RPC result send in response to RPC param
 *
 * \streamout{rpcParam,xdr_iarchive <> }
 * RPC param received from mrrocpp
 *
 * \par Events:
 *
 * \event{rpcCall}
 * RPC called. rpcParam datastream should be read.
 *
 *
 * \par Event handlers:
 *
 * \handler{onNewReading}
 * New reading is ready to be read from reading data stream.
 *
 * \handler{onRpcResult}
 * RPC result is ready.
 *
 * \par Properties:
 *
 * \prop{port,port,""}
 * Port on which to listen.
 *
 *
 * \see http://robotyka.ia.pw.edu.pl/twiki/bin/view/Projects/Mrrocpp
 *
 * @{
 *
 * @}
 */

namespace Proxies {
namespace Mrrocpp {

/**
 * See \link Mrrocpp_Proxy \endlink.
 */
class Mrrocpp_Proxy: public Base::Component
{
public:
	Mrrocpp_Proxy(const std::string & name = "");
	virtual ~Mrrocpp_Proxy();

protected:
	virtual bool onStart();

	/*!
	 * Method called when component is stopped
	 * \return true on success
	 */
	virtual bool onStop();

	/*!
	 * Method called when component is initialized
	 * \return true on success
	 */
	virtual bool onInit();

	/*!
	 * Method called when component is finished
	 * \return true on success
	 */
	virtual bool onFinish();

	/*!
	 * Method called when step is called
	 * \return true on success
	 */
	virtual bool onStep();
private:
	/**
	 * MPS (short form of MrrocppProxyState)
	 */
	enum MrrocppProxyState {
		MPS_NOT_INITIALIZED, MPS_LISTENING, MPS_CONNECTED, MPS_WAITING_FOR_READING, MPS_WAITING_FOR_RPC_RESULT
	};

	MrrocppProxyState state;
	/**
	 * Event handler called when new reading is ready.
	 */
	void onNewReading();
	void serviceReading();

	Base::EventHandler <Mrrocpp_Proxy> h_onNewReading;
	Base::DataStreamInPtr <Types::Mrrocpp_Proxy::Reading> reading;

	/**
	 * Event handler called when RPC call has finished.
	 */
	void onRpcResult();
	void serviceRpc();

	Base::Event *rpcCall;
	Base::DataStreamOut <xdr_iarchive <> > rpcParam;
	Base::DataStreamInPtr <Types::Mrrocpp_Proxy::Reading> rpcResult;
	Base::DataStreamIn<struct timespec> in_timestamp;
	Base::EventHandler <Mrrocpp_Proxy> h_onRpcResult;

	boost::shared_ptr <xdr_iarchive <> > header_iarchive;
	boost::shared_ptr <xdr_iarchive <> > iarchive;
	boost::shared_ptr <xdr_oarchive <> > header_oarchive;
	boost::shared_ptr <xdr_oarchive <> > oarchive;

	boost::shared_ptr <Types::Mrrocpp_Proxy::Reading> readingMessage;
	struct timespec readingTimestamp;
	boost::shared_ptr <Types::Mrrocpp_Proxy::Reading> rpcResultMessage;

	void receiveBuffersFromMrrocpp();
	void sendBuffersToMrrocpp();

	initiate_message_header imh;
	reading_message_header rmh;

	Socket serverSocket;
	boost::shared_ptr <Socket> clientSocket;

	void tryAcceptConnection();
	void tryReceiveFromMrrocpp();

	size_t initiate_message_header_size;

	boost::mutex readingMutex;
	boost::mutex rpcCallMutex;

	Base::Property<int> port;
};

} // namespace Mrrocpp {

}

REGISTER_PROCESSOR_COMPONENT("Mrrocpp", Proxies::Mrrocpp::Mrrocpp_Proxy, Common::Panel_Empty)

#endif /* MRROCPP_PROXY_HPP_ */
