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

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "Panel_Empty.hpp"
#include "SampleClass.hpp"
#include "xdr/xdr_oarchive.hpp"
#include "xdr/xdr_iarchive.hpp"
#include "Socket.hpp"
#include "headers.h"

namespace Proxies {
namespace Mrrocpp {

using namespace cv;

struct Mrrocpp_ProxyProps: public Base::Props
{
	int port;

	bool rpcMode;
	/*!
	 * Load settings
	 *
	 * @param pt root property tree to load settings from
	 */
	virtual void load(const ptree & pt)
	{
		port = pt.get <int> ("port");
		rpcMode = pt.get <bool> ("rpcMode");
	}

	/*!
	 * Save settings
	 *
	 * @param pt root property tree to save settings
	 */
	virtual void save(ptree & pt)
	{
		pt.put("port", port);
		pt.put("rpcMode", rpcMode);
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
	void onNewMsgToSend();

	Base::EventHandler <Mrrocpp_Proxy> h_onNewMsgToSend;
	Base::Event *newMsgReceived;
	Base::DataStreamIn <xdr_oarchive <> > msgToSend;
	Base::DataStreamOut <xdr_iarchive <> > msgReceived;

	boost::shared_ptr <xdr_iarchive <> > header_iarchive;
	boost::shared_ptr <xdr_iarchive <> > iarchive;
	boost::shared_ptr <xdr_oarchive <> > header_oarchive;
	boost::shared_ptr <xdr_oarchive <> > oarchive;

	void receiveBuffersFromMrrocpp();
	void sendBuffersToMrrocpp();

	SampleClass sample;

	Socket serverSocket;
	boost::shared_ptr <Socket> clientSocket;

	Mrrocpp_ProxyProps props;

	bool clientConnected;
	bool msgSet;
	bool getReadingReceived;

	void sendMsg();

	size_t initiate_message_header_size;
};

} // namespace Mrrocpp {

}

REGISTER_PROCESSOR_COMPONENT("Mrrocpp", Proxies::Mrrocpp::Mrrocpp_Proxy, Common::Panel_Empty)

#endif /* MRROCPP_PROXY_HPP_ */
