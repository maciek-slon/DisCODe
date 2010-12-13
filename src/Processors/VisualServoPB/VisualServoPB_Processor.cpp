/*!
 * \file
 * \brief
 */

#include <memory>
#include <string>

#include "VisualServoPB_Processor.hpp"
#include "Common/Logger.hpp"

namespace Processors {
namespace VisualServoPB {

VisualServoPB_Processor::VisualServoPB_Processor(const std::string & name) :
	Base::Component(name)
{
	LOG(LTRACE) << "Hello VisualServoPB_Processor\n";
}

VisualServoPB_Processor::~VisualServoPB_Processor()
{
	LOG(LTRACE) << "Good bye VisualServoPB_Processor\n";
}

bool VisualServoPB_Processor::onInit()
{
	LOG(LTRACE) << "VisualServoPB_Processor::initialize\n";

	// Register data streams, events and event handlers HERE!
	h_onObjectLocated.setup(this, &VisualServoPB_Processor::onObjectLocated);
	registerHandler("onObjectLocated", &h_onObjectLocated);

	h_onObjectNotFound.setup(this, &VisualServoPB_Processor::onObjectNotFound);
	registerHandler("onObjectNotFound", &h_onObjectNotFound);

	registerStream("in_position", &in_position);
	registerStream("out_reading", &out_reading);

	readingReady = registerEvent("readingReady");

	return true;
}

bool VisualServoPB_Processor::onFinish()
{
	LOG(LTRACE) << "VisualServoPB_Processor::finish\n";

	return true;
}

bool VisualServoPB_Processor::onStep()
{
	LOG(LTRACE) << "VisualServoPB_Processor::step\n";
	return true;
}

bool VisualServoPB_Processor::onStop()
{
	return true;
}

bool VisualServoPB_Processor::onStart()
{
	return true;
}

void VisualServoPB_Processor::onObjectLocated()
{
	LOG(LTRACE) << "VisualServoPB_Processor::onObjectLocated()\n";
	PBReading pbr;
	pbr.objectVisible = true;
	pbr.objectPosition = in_position.read();

	out_reading.write(pbr);
	readingReady->raise();
}

void VisualServoPB_Processor::onObjectNotFound()
{
	LOG(LTRACE) << "VisualServoPB_Processor::onObjectNotFound()\n";
	PBReading pbr;
	pbr.objectVisible = false;
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 4; ++j) {
			pbr.objectPosition.elements[i][j] = 0;
		}
	}
	out_reading.write(pbr);

	readingReady->raise();
}

}//: namespace VisualServoPB
}//: namespace Processors
