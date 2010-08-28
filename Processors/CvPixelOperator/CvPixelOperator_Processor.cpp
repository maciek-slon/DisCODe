/*!
 * \file CvPixelOperator_Processor.cpp
 * \brief
 * \author mstefanc
 * \date 2010-07-05
 */

#include <memory>
#include <string>

#include "CvPixelOperator_Processor.hpp"
#include "Logger.hpp"

namespace Processors {
namespace CvPixelOperator {

CvPixelOperator_Processor::CvPixelOperator_Processor(const std::string & name) : Base::Component(name)
{
	LOG(TRACE) << "Hello CvPixelOperator_Processor\n";
}

CvPixelOperator_Processor::~CvPixelOperator_Processor()
{
	LOG(TRACE) << "Good bye CvPixelOperator_Processor\n";
}

bool CvPixelOperator_Processor::onInit()
{
	LOG(TRACE) << "CvPixelOperator_Processor::initialize\n";

	h_onNewImage.setup(this, &CvPixelOperator_Processor::onNewImage);
	registerHandler("onNewImage", &h_onNewImage);

	registerStream("in_img", &in_img);

	newImage = registerEvent("newImage");

	registerStream("out_img", &out_img);

	return true;
}

bool CvPixelOperator_Processor::onFinish()
{
	LOG(TRACE) << "CvPixelOperator_Processor::finish\n";

	return true;
}

bool CvPixelOperator_Processor::onStep()
{
	LOG(TRACE) << "CvPixelOperator_Processor::step\n";
	return true;
}

bool CvPixelOperator_Processor::onStop()
{
	return true;
}

bool CvPixelOperator_Processor::onStart()
{
	return true;
}

void CvPixelOperator_Processor::onNewImage()
{
	LOG(TRACE) << "CvPixelOperator_Processor::onNewImage\n";
	try {
		cv::Mat img = in_img.read();

		//img.convertTo(tmp, CV_32F, 1./255);
		cv::filter2D(img, tmp, -1, props.kernel, cv::Size(-1, -1), props.delta, cv::BORDER_REPLICATE);

		out_img.write(tmp);

		newImage->raise();
	} catch (...) {
		LOG(ERROR) << "CvPixelOperator_Processor::onNewImage failed\n";
	}
}

}//: namespace CvPixelOperator
}//: namespace Processors
