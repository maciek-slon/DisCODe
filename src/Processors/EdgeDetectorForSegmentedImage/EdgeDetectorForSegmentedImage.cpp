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

using namespace cv;
using namespace std;
using namespace Types;

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
	registerStream("out_contours", &out_contours);

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

	DrawableContainer dc;

	for (size_t i = 0; i < si.segments.size(); ++i) {
		si.segments[i].setSegmentImageFromSegmentedImage(si.image);
		Contour *c = new Contour;
		c->setContours(*si.segments[i].getContours());

//		if (si.segments[i].getContours()->size() > 1) {
//			LOG (LFATAL) << "si.segments[" << i << "].getContours()->size() = "
//					<< si.segments[i].getContours()->size() << endl;
//		}
		dc.add(c);
	}

	out_edgesDetected.write(si);
	out_contours.write(dc);
	edgesDetected->raise();
}

}//: namespace EdgeDetectorForSegmentedImage
}//: namespace Processors
