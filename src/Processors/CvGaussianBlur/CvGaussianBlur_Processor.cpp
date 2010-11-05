/*!
 * \file CvGaussianBlur_Processor.cpp
 * \brief
 * \author mstefanc
 * \date 2010-07-05
 */

#include <memory>
#include <string>

#include "CvGaussianBlur_Processor.hpp"
#include "Logger.hpp"

namespace Processors {
namespace CvGaussianBlur {

CvGaussianBlur_Processor::CvGaussianBlur_Processor(const std::string & name) : Base::Component(name)
{
	LOG(LTRACE) << "Hello CvGaussianBlur_Processor\n";
}

CvGaussianBlur_Processor::~CvGaussianBlur_Processor()
{
	LOG(LTRACE) << "Good bye CvGaussianBlur_Processor\n";
}

bool CvGaussianBlur_Processor::onInit()
{
	LOG(LTRACE) << "CvGaussianBlur_Processor::initialize\n";

	h_onNewImage.setup(this, &CvGaussianBlur_Processor::onNewImage);
	registerHandler("onNewImage", &h_onNewImage);

	registerStream("in_img", &in_img);

	newImage = registerEvent("newImage");

	registerStream("out_img", &out_img);

	return true;
}

bool CvGaussianBlur_Processor::onFinish()
{
	LOG(LTRACE) << "CvGaussianBlur_Processor::finish\n";

	return true;
}

bool CvGaussianBlur_Processor::onStep()
{
	LOG(LTRACE) << "CvGaussianBlur_Processor::step\n";
	return true;
}

bool CvGaussianBlur_Processor::onStop()
{
	return true;
}

bool CvGaussianBlur_Processor::onStart()
{
	return true;
}

void CvGaussianBlur_Processor::onNewImage()
{
	LOG(LTRACE) << "CvGaussianBlur_Processor::onNewImage\n";
	try {
		cv::Mat img = in_img.read();
		//cv::Mat out = img.clone();
		cv::GaussianBlur(img, img, props.kernel, props.sigmax, props.sigmay);
		out_img.write(img);
		newImage->raise();
	} catch (...) {
		LOG(LERROR) << "CvGaussianBlur_Processor::onNewImage failed\n";
	}
}

}//: namespace CvGaussianBlur
}//: namespace Processors
