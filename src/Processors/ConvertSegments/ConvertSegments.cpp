/*!
 * \file
 * \brief
 */

#include <memory>
#include <string>

#include "ConvertSegments.hpp"
#include "Common/Logger.hpp"

namespace Processors {
namespace ConvertSegments {

using namespace cv;
using namespace std;
using Types::Segmentation::SegmentedImage;

ConvertSegments_Processor::ConvertSegments_Processor(const std::string & name) :
	Base::Component(name)
{
	LOG(LTRACE) << "Hello ConvertSegments_Processor\n";

	for (int i = 0; i < colorsSize; ++i) {
		colors[i] = ((i + 1) * 255) / colorsSize;
	}
}

ConvertSegments_Processor::~ConvertSegments_Processor()
{
	LOG(LTRACE) << "Good bye ConvertSegments_Processor\n";
}

bool ConvertSegments_Processor::onInit()
{
	LOG(LTRACE) << "ConvertSegments_Processor::initialize\n";

	// Register data streams, events and event handlers HERE!
	h_onSegmented.setup(this, &ConvertSegments_Processor::onSegmented);
	registerHandler("onSegmented", &h_onSegmented);

	registerStream("in_segmented", &in_segmented);

	registerStream("out_img", &out_img);
	onNewImage = registerEvent("onNewImage");

	return true;
}

bool ConvertSegments_Processor::onFinish()
{
	LOG(LTRACE) << "ConvertSegments_Processor::finish\n";

	return true;
}

bool ConvertSegments_Processor::onStep()
{
	LOG(LTRACE) << "ConvertSegments_Processor::step\n";
	return true;
}

bool ConvertSegments_Processor::onStop()
{
	return true;
}

bool ConvertSegments_Processor::onStart()
{
	return true;
}

void ConvertSegments_Processor::onSegmented()
{
	SegmentedImage si = in_segmented.read();

	Mat image = Mat::zeros(si.image.size(), CV_8U);

	Size size = si.image.size();
	for (int y = 0; y < size.height; ++y) {
		for (int x = 0; x < size.width; ++x) {
image		.at<u_int8_t>(y, x) =
	}
}

out_img.write(image);
}

}//: namespace ConvertSegments
}//: namespace Processors
