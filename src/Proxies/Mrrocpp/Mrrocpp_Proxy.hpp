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

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "Panel_Empty.hpp"

#include "SampleClass.hpp"


#include "xdr/xdr_oarchive.hpp"
#include "xdr/xdr_iarchive.hpp"



namespace Proxies {
namespace Mrrocpp {

using namespace cv;

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

	Base::EventHandler<Mrrocpp_Proxy> h_onNewMsgToSend;
	Base::Event *newMsgReceived;
	Base::DataStreamIn<xdr_oarchive<> > msgToSend;
	Base::DataStreamOut<xdr_iarchive<> > msgReceived;
	SampleClass sample;
};

} // namespace Mrrocpp {

}

REGISTER_PROCESSOR_COMPONENT("Mrrocpp", Proxies::Mrrocpp::Mrrocpp_Proxy, Common::Panel_Empty)

#endif /* MRROCPP_PROXY_HPP_ */
