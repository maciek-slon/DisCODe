/*!
 * \file
 * \brief
 */

#include <memory>
#include <string>

#include "GrayClassification.hpp"
#include "Common/Logger.hpp"

namespace Processors {
namespace GrayClassification {

using namespace std;
using namespace cv;

GrayClassification_Processor::GrayClassification_Processor(const std::string & name) :
	Base::Component(name)
{
	LOG(LTRACE) << "Hello GrayClassification_Processor\n";
}

GrayClassification_Processor::~GrayClassification_Processor()
{
	LOG(LTRACE) << "Good bye GrayClassification_Processor\n";
}

bool GrayClassification_Processor::onInit()
{
	LOG(LTRACE) << "GrayClassification_Processor::initialize\n";

	// Register data streams, events and event handlers HERE!
	h_onNewImage.setup(this, &GrayClassification_Processor::onNewImage);
	registerHandler("onNewImage", &h_onNewImage);

	registerStream("in_img", &in_img);

	newImage = registerEvent("newImage");

	registerStream("out_img", &out_img);

	return true;
}

bool GrayClassification_Processor::onFinish()
{
	LOG(LTRACE) << "GrayClassification_Processor::finish\n";

	return true;
}

bool GrayClassification_Processor::onStep()
{
	LOG(LTRACE) << "GrayClassification_Processor::step\n";
	return true;
}

bool GrayClassification_Processor::onStop()
{
	return true;
}

bool GrayClassification_Processor::onStart()
{
	return true;
}

void GrayClassification_Processor::onNewImage()
{
	LOG(LTRACE) << "GrayClassification_Processor::onNewImage\n";
	Mat img = in_img.read();

	line(img, Point(0, 0), Point(100, 100), CV_RGB(255, 0, 0), 4);

	out_img.write(img.clone());
	newImage->raise();
}

}//: namespace GrayClassification
}//: namespace Processors
