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

VisualServoPB_Processor::VisualServoPB_Processor(const std::string & name) : Base::Component(name)
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

}//: namespace VisualServoPB
}//: namespace Processors
