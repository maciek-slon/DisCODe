/*!
 * \file
 * \brief
 */

#include <memory>
#include <string>

#include "CspObjectRecognize.hpp"
#include "Common/Logger.hpp"

namespace Processors {
namespace CspObjectRecognize {

CspObjectRecognize_Processor::CspObjectRecognize_Processor(const std::string & name) :
	Base::Component(name)
{
	LOG(LTRACE) << "Hello CspObjectRecognize_Processor\n";
}

CspObjectRecognize_Processor::~CspObjectRecognize_Processor()
{
	LOG(LTRACE) << "Good bye CspObjectRecognize_Processor\n";
}

bool CspObjectRecognize_Processor::onInit()
{
	LOG(LTRACE) << "CspObjectRecognize_Processor::initialize\n";

	// Register data streams, events and event handlers HERE!
	registerStream("in_segmentedImage", &in_segmentedImage);

	h_onSegmentedImage.setup(this, &CspObjectRecognize_Processor::onSegmentedImage);
	registerHandler("onSegmentedImage", &h_onSegmentedImage);
	return true;
}

bool CspObjectRecognize_Processor::onFinish()
{
	LOG(LTRACE) << "CspObjectRecognize_Processor::finish\n";

	return true;
}

bool CspObjectRecognize_Processor::onStep()
{
	LOG(LTRACE) << "CspObjectRecognize_Processor::step\n";
	return true;
}

bool CspObjectRecognize_Processor::onStop()
{
	return true;
}

bool CspObjectRecognize_Processor::onStart()
{
	return true;
}

void CspObjectRecognize_Processor::onSegmentedImage()
{
	LOG(LFATAL) << "CspObjectRecognize_Processor::onSegmentedImage()\n";
}

}//: namespace CspObjectRecognize
}//: namespace Processors
