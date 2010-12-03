/*!
 * \file
 * \brief
 */

#include <memory>
#include <string>

#include "GrayImageSegmentation.hpp"
#include "Common/Logger.hpp"

namespace Processors {
namespace GrayImageSegmentation {

GrayImageSegmentation_Processor::GrayImageSegmentation_Processor(const std::string & name) :
	Base::Component(name)
{
	LOG(LTRACE) << "Hello GrayImageSegmentation_Processor\n";
}

GrayImageSegmentation_Processor::~GrayImageSegmentation_Processor()
{
	LOG(LTRACE) << "Good bye GrayImageSegmentation_Processor\n";
}

bool GrayImageSegmentation_Processor::onInit()
{
	LOG(LTRACE) << "GrayImageSegmentation_Processor::initialize\n";

	// Register data streams, events and event handlers HERE!
	h_onNewImage.setup(this, &GrayImageSegmentation_Processor::onNewImage);
	registerHandler("onNewImage", &h_onNewImage);

	registerStream("in_img", &in_img);

	return true;
}

bool GrayImageSegmentation_Processor::onFinish()
{
	LOG(LTRACE) << "GrayImageSegmentation_Processor::finish\n";

	return true;
}

bool GrayImageSegmentation_Processor::onStep()
{
	LOG(LTRACE) << "GrayImageSegmentation_Processor::step\n";
	return true;
}

bool GrayImageSegmentation_Processor::onStop()
{
	return true;
}

bool GrayImageSegmentation_Processor::onStart()
{
	return true;
}

void GrayImageSegmentation_Processor::onNewImage()
{

}

}//: namespace GrayImageSegmentation
}//: namespace Processors
