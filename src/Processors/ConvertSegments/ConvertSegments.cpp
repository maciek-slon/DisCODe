/*!
 * \file
 * \brief
 */

#include <memory>
#include <string>
#include <map>

#include "ConvertSegments.hpp"
#include "Common/Logger.hpp"

namespace Processors {
namespace ConvertSegments {

using namespace cv;
using namespace std;
using Types::Segmentation::SegmentedImage;
using Types::Segmentation::MaskType;

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
	newImage = registerEvent("newImage");

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

	Mat classImage;
	if (props.showEdgeImage) {
		classImage = si.edgeImage;
	} else {
		classImage = si.image;
	}

	Mat image = Mat::zeros(si.image.size(), CV_8U);

	map <MaskType, u_int8_t> conversionMap;
	conversionMap[0] = 0;

	u_int8_t color = 0;
	Size size = si.image.size();
	for (int y = 0; y < size.height; ++y) {
		for (int x = 0; x < size.width; ++x) {
			MaskType originalValue = classImage.at <MaskType> (y, x);
			if (conversionMap.find(originalValue) == conversionMap.end()) {
				color = (color + 32) % 256;
				if (color == 0) {
					color = 32;
				}
				conversionMap[originalValue] = color;
			}
			image.at <u_int8_t> (y, x) = conversionMap[originalValue];
		}
	}

	out_img.write(image);
	newImage->raise();
}

}//: namespace ConvertSegments
}//: namespace Processors
