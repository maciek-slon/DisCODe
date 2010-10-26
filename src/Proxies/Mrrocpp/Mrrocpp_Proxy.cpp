/*
 * Mrrocpp_Proxy.cpp
 *
 *  Created on: 15-10-2010
 *      Author: mateusz
 */

#include "Mrrocpp_Proxy.hpp"

namespace Proxies {

namespace Mrrocpp {

Mrrocpp_Proxy::Mrrocpp_Proxy(const std::string & name) :
	Base::Component(name)
{
	LOG(FATAL)<<"Mrrocpp_Proxy::Mrrocpp_Proxy\n";
}

Mrrocpp_Proxy::~Mrrocpp_Proxy()
{
	LOG(FATAL)<<"Mrrocpp_Proxy::~Mrrocpp_Proxy\n";
}

bool Mrrocpp_Proxy::onStart()
{
	LOG(FATAL)<<"Mrrocpp_Proxy::onStart\n";
	return true;
}

bool Mrrocpp_Proxy::onInit()
{
	LOG(FATAL)<<"Mrrocpp_Proxy::onInit\n";

	h_onNewImage.setup(this, &Mrrocpp_Proxy::onNewImage);
	registerHandler("onNewImage", &h_onNewImage);

	registerStream("in_img", &in_img);
	return true;
}

bool Mrrocpp_Proxy::onStop()
{
	LOG(FATAL)<<"Mrrocpp_Proxy::onStop\n";
	return true;
}

bool Mrrocpp_Proxy::onFinish()
{
	LOG(FATAL)<<"Mrrocpp_Proxy::onFinish\n";
	return true;
}

bool Mrrocpp_Proxy::onStep()
{
	LOG(FATAL)<<"Mrrocpp_Proxy::onStep\n";
	return true;
}

void Mrrocpp_Proxy::onNewImage()
{
	in_img.read();
	LOG(FATAL)<<"Mrrocpp_Proxy::onNewImage\n";
}

} // namespace Mrrocpp {

}


