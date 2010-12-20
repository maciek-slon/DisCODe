/*!
 * \file Mrrocpp_Proxy.hpp
 * \brief MRROC++ Proxy.
 *
 * \date 15-10-2010
 * \author Mateusz Boryn
 */

#ifndef MRROCPP_PROXY_HPP_
#define MRROCPP_PROXY_HPP_

#include <cv.h>
#include <boost/shared_ptr.hpp>
#include <boost/thread/mutex.hpp>

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "Panel_Empty.hpp"
#include "xdr/xdr_oarchive.hpp"
#include "xdr/xdr_iarchive.hpp"
#include "Socket.hpp"
#include "headers.h"
#include "Reading.hpp"

/**
 * \defgroup Mrrocpp Proxy
 * \ingroup Proxies
 *
 * Proxy to the mrrocpp system.
 *
 * \par Data streams:
 *
 * \streamin{reading,Reading}
 * Reading to send to mrrocpp as soon as mrroc asks for it.
 *
 * \streamin{rpcResult,Reading}
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
 * \prop{port,port,&nbsp;}
 * Port on which to listen.
 *
 *
 * \see http://www.youtube.com/watch?v=sKxy5Vst7Mo&feature=player_embedded
 * \see http://robotyka.ia.pw.edu.pl/twiki/bin/view/Projects/Mrrocpp
 *
 * @{
 *
 * @}
 */

namespace Proxies {
namespace Mrrocpp {

using namespace cv;

struct Mrrocpp_ProxyProps: public Base::Props
{
	int port;

	/*!
	 * Load settings
	 *
	 * @param pt root property tree to load settings from
	 */
	virtual void load(const ptree & pt)
	{
		port = pt.get <int> ("port");
	}

	/*!
	 * Save settings
	 *
	 * @param pt root property tree to save settings
	 */
	virtual void save(ptree & pt)
	{
		pt.put("port", port);
	}
};

/*!
 * \defgroup Mrrocpp Mrrocpp
 * \ingroup Proxies
 *
 * @{
 *
 * }@
 */
class Mrrocpp_Proxy: public Base::Component
{
public:
	Mrrocpp_Proxy(const std::string & name = "");
	virtual ~Mrrocpp_Proxy();
	virtual Base::Props * getProperties();

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
	Base::DataStreamInPtr <Reading> reading;

	/**
	 * Event handler called when RPC call has finished.
	 */
	void onRpcResult();
	void serviceRpc();

	Base::Event *rpcCall;
	Base::DataStreamOut <xdr_iarchive <> > rpcParam;
	Base::DataStreamInPtr <Reading> rpcResult;
	Base::EventHandler <Mrrocpp_Proxy> h_onRpcResult;

	boost::shared_ptr <xdr_iarchive <> > header_iarchive;
	boost::shared_ptr <xdr_iarchive <> > iarchive;
	boost::shared_ptr <xdr_oarchive <> > header_oarchive;
	boost::shared_ptr <xdr_oarchive <> > oarchive;

	boost::shared_ptr <Reading> readingMessage;
	boost::shared_ptr <Reading> rpcResultMessage;

	void receiveBuffersFromMrrocpp();
	void sendBuffersToMrrocpp();

	initiate_message_header imh;
	reading_message_header rmh;

	Socket serverSocket;
	boost::shared_ptr <Socket> clientSocket;

	Mrrocpp_ProxyProps props;

	void tryAcceptConnection();
	void tryReceiveFromMrrocpp();

//	bool clientConnected;
//	bool msgSet;
//	bool getReadingReceived;

//	enum
//	{
//		PROXY_NOT_CONFIGURED, PROXY_WAITING_FOR_COMMAND, PROXY_WAITING_FOR_READING, PROXY_WAITING_FOR_RPC_RESULT
//	} proxyState;

	size_t initiate_message_header_size;

	boost::mutex eventsMutex;
};

} // namespace Mrrocpp {

}

REGISTER_PROCESSOR_COMPONENT("Mrrocpp", Proxies::Mrrocpp::Mrrocpp_Proxy, Common::Panel_Empty)

#endif /* MRROCPP_PROXY_HPP_ */
