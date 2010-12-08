/*!
 * \file
 * \brief
 */

#include <memory>
#include <string>

#include "EdgeDetectorForSegmentedImage.hpp"
#include "Common/Logger.hpp"

namespace Processors {
namespace EdgeDetectorForSegmentedImage {

EdgeDetectorForSegmentedImage_Processor::EdgeDetectorForSegmentedImage_Processor(const std::string & name) :
	Base::Component(name)
{
	LOG(LTRACE) << "Hello EdgeDetectorForSegmentedImage_Processor\n";
}

EdgeDetectorForSegmentedImage_Processor::~EdgeDetectorForSegmentedImage_Processor()
{
	LOG(LTRACE) << "Good bye EdgeDetectorForSegmentedImage_Processor\n";
}

bool EdgeDetectorForSegmentedImage_Processor::onInit()
{
	LOG(LTRACE) << "EdgeDetectorForSegmentedImage_Processor::initialize\n";

	// Register data streams, events and event handlers HERE!
	registerStream("in_segmented", &in_segmented);
	registerStream("out_edgesDetected", &out_edgesDetected);

	h_onSegmented.setup(this, &EdgeDetectorForSegmentedImage_Processor::onSegmented);
	registerHandler("onSegmented", &h_onSegmented);

	edgesDetected = registerEvent("edgesDetected");

	return true;
}

bool EdgeDetectorForSegmentedImage_Processor::onFinish()
{
	LOG(LTRACE) << "EdgeDetectorForSegmentedImage_Processor::finish\n";

	return true;
}

bool EdgeDetectorForSegmentedImage_Processor::onStep()
{
	LOG(LTRACE) << "EdgeDetectorForSegmentedImage_Processor::step\n";
	return true;
}

bool EdgeDetectorForSegmentedImage_Processor::onStop()
{
	return true;
}

bool EdgeDetectorForSegmentedImage_Processor::onStart()
{
	return true;
}

void EdgeDetectorForSegmentedImage_Processor::onSegmented()
{
	Types::Segmentation::SegmentedImage si = in_segmented.read();

	si.detectEdges();

	out_edgesDetected.write(si);
	edgesDetected->raise();
}

}//: namespace EdgeDetectorForSegmentedImage
}//: namespace Processors
