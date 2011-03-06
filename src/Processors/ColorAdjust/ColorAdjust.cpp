/*!
 * \file ColorAdjust.cpp
 * \brief
 * \author mstefanc
 * \date 2010-07-05
 */

#include <memory>
#include <string>

#include "ColorAdjust.hpp"
#include "Logger.hpp"

namespace Processors {
namespace ColorAdjust {

ColorAdjust::ColorAdjust(const std::string & name) : Base::Component(name)
{
	LOG(LTRACE) << "Hello ColorAdjust\n";


	/*hand = new Base::EventHandler2;
	hand->setup(boost::bind(&CvWindow_Sink::onNewImageN, this, i));
	handlers.push_back(hand);
	registerHandler(std::string("onNewImage")+id, hand);*/
}

ColorAdjust::~ColorAdjust()
{
	LOG(LTRACE) << "Good bye ColorAdjust\n";
}

bool ColorAdjust::onInit()
{
	LOG(LTRACE) << "ColorAdjust::initialize\n";

	h_onNewImage.setup(this, &ColorAdjust::onNewImage);
	registerHandler("onNewImage", &h_onNewImage);

	registerStream("in_img", &in_img);

	newImage = registerEvent("newImage");

	registerStream("out_img", &out_img);



	return true;
}

bool ColorAdjust::onFinish()
{
	LOG(LTRACE) << "ColorAdjust::finish\n";

	return true;
}

bool ColorAdjust::onStep()
{
	LOG(LTRACE) << "ColorAdjust::step\n";
	return true;
}

bool ColorAdjust::onStop()
{
	return true;
}

bool ColorAdjust::onStart()
{
	return true;
}

void ColorAdjust::onNewImage()
{
	LOG(LTRACE) << "ColorAdjust::onNewImage\n";
	try {
		cv::Mat img = in_img.read();
		cv::Mat out = img.clone();
		out_img.write(out);
		newImage->raise();
	} catch (...) {
		LOG(LERROR) << "CvThreshold::onNewImage failed\n";
	}
}

}//: namespace ColorAdjust
}//: namespace Processors
