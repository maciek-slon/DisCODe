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

using namespace cv;
using namespace std;
using Types::Segmentation::SegmentedImage;

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

	registerStream("out_segmented", &out_segmented);
	segmented = registerEvent("segmented");

	segmentExtractor.setMinSegmentArea(props.minSegmentArea);
	segmentExtractor.setMinVariance(props.minVariance);
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
	Mat image = in_img.read().clone();
	if (image.type() != CV_8U) {
		LOG(LERROR) << "GrayImageSegmentation_Processor::onNewImage(): image.type() != CV_8U\n";
		return;
	}
	SegmentedImage si = segmentExtractor.segmentImage(image);

	LOG(LDEBUG) << "GrayImageSegmentation_Processor::onNewImage(): si.segments.size()=" << si.segments.size();

	out_segmented.write(si);
	segmented->raise();
}

}//: namespace GrayImageSegmentation
}//: namespace Processors
