/*!
 * \file
 * \brief
 */

#include <memory>
#include <string>

#include "VisualServoIB_Processor.hpp"
#include "Common/Logger.hpp"

namespace Processors {
namespace VisualServoIB {

using Types::ImagePosition;
using Types::Mrrocpp_Proxy::IBReading;

VisualServoIB_Processor::VisualServoIB_Processor(const std::string & name) :
	Base::Component(name)
{
	LOG(LTRACE) << "Hello VisualServoIB_Processor\n";
}

VisualServoIB_Processor::~VisualServoIB_Processor()
{
	LOG(LTRACE) << "Good bye VisualServoIB_Processor\n";
}

bool VisualServoIB_Processor::onInit()
{
	LOG(LTRACE) << "VisualServoIB_Processor::initialize\n";

	// Register data streams, events and event handlers HERE!
	h_onObjectLocated.setup(this, &VisualServoIB_Processor::onObjectLocated);
	registerHandler("onObjectLocated", &h_onObjectLocated);

	h_onObjectNotFound.setup(this, &VisualServoIB_Processor::onObjectNotFound);
	registerHandler("onObjectNotFound", &h_onObjectNotFound);

	registerStream("in_position", &in_position);
	registerStream("out_reading", &out_reading);

	readingReady = registerEvent("readingReady");

	return true;
}

bool VisualServoIB_Processor::onFinish()
{
	LOG(LTRACE) << "VisualServoIB_Processor::finish\n";

	return true;
}

bool VisualServoIB_Processor::onStep()
{
	LOG(LTRACE) << "VisualServoIB_Processor::step\n";
	return true;
}

bool VisualServoIB_Processor::onStop()
{
	return true;
}

bool VisualServoIB_Processor::onStart()
{
	return true;
}

void VisualServoIB_Processor::onObjectLocated()
{
	try{
		LOG(LTRACE) << "VisualServoIB_Processor::onObjectLocated()\n";
		IBReading ibr;
		ibr.objectVisible = true;
		ibr.imagePosition = in_position.read();

		LOG(LDEBUG)<<"ibr.imagePosition = "<<ibr.imagePosition;


		out_reading.write(ibr);
		readingReady->raise();
	}catch(exception& ex){
		LOG(LERROR) << "VisualServoIB_Processor::onObjectLocated(): " << ex.what();
	}
}

void VisualServoIB_Processor::onObjectNotFound()
{
	LOG(LTRACE) << "VisualServoIB_Processor::onObjectNotFound()\n";
	IBReading ibr;
	ibr.objectVisible = false;
	for (int i = 0; i < ImagePosition::elementsSize; ++i) {
		ibr.imagePosition.elements[i]= 0;
	}
	out_reading.write(ibr);

	readingReady->raise();
}

}//: namespace VisualServoIB
}//: namespace Processors
