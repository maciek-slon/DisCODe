/*
 * Handler.hpp
 *
 *  Created on: Nov 8, 2010
 *      Author: mboryn
 */

#ifndef HANDLER_HPP_
#define HANDLER_HPP_

#include "Mrrocpp_Proxy.hpp"

namespace Proxies {

namespace Mrrocpp {

class Handler
{
public:
	Handler(Mrrocpp_Proxy* mrrocProxy);
	virtual ~Handler();

//	virtual void handleReceived

protected:
	Mrrocpp_Proxy* mrrocProxy;
};

} // namespace Mrrocpp

} // namespace Proxies

#endif /* HANDLER_HPP_ */
