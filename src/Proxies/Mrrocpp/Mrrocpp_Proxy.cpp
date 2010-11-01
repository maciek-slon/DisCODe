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
	Base::Component(name), sample(0xAA, 0xAA, "qwer", "aaa")
{
	LOG(TRACE)<<"Mrrocpp_Proxy::Mrrocpp_Proxy\n";
}

Mrrocpp_Proxy::~Mrrocpp_Proxy()
{
	LOG(TRACE)<<"Mrrocpp_Proxy::~Mrrocpp_Proxy\n";
}

bool Mrrocpp_Proxy::onStart()
{
	LOG(TRACE)<<"Mrrocpp_Proxy::onStart\n";
	return true;
}

bool Mrrocpp_Proxy::onInit()
{
	LOG(TRACE)<<"Mrrocpp_Proxy::onInit\n";

	h_onNewMsgToSend.setup(this, &Mrrocpp_Proxy::onNewMsgToSend);
	registerHandler("onNewMsgToSend", &h_onNewMsgToSend);

	newMsgReceived = registerEvent("newMsgReceived");

	registerStream("msgToSend", &msgToSend);
	registerStream("msgReceived", &msgReceived);
	return true;
}

bool Mrrocpp_Proxy::onStop()
{
	LOG(TRACE)<<"Mrrocpp_Proxy::onStop\n";
	return true;
}

bool Mrrocpp_Proxy::onFinish()
{
	LOG(TRACE)<<"Mrrocpp_Proxy::onFinish\n";
	return true;
}

bool Mrrocpp_Proxy::onStep()
{
	LOG(FATAL)<<"Mrrocpp_Proxy::onStep\n";

	xdr_oarchive<> oarchive;
	oarchive << sample;

	FILE *f = fopen("pliczek.bin", "ab");
	if(f == NULL){
		return true;
	}

	fwrite(oarchive.get_buffer(), oarchive.getArchiveSize(), 1, f);

	fclose(f);

	xdr_iarchive<> iarchive(oarchive.get_buffer(), oarchive.getArchiveSize());
	SampleClass sc;
	iarchive >> sc;
	LOG(DEBUG) << "sc.numberOne: " << sc.numberOne <<endl;
	LOG(DEBUG) << "sc.numberTwo: " << sc.numberTwo <<endl;
	LOG(DEBUG) << "sc.text: " << sc.text <<endl;
	LOG(DEBUG) << "sc.str: " << sc.str <<endl;

	++sample.numberOne;
	--sample.numberTwo;
	sample.str += "a";

	return true;
}

void Mrrocpp_Proxy::onNewMsgToSend()
{
	msgToSend.read();
	LOG(TRACE)<<"Mrrocpp_Proxy::onNewMsgToSend\n";
}

} // namespace Mrrocpp {

} // namespace Proxies


