/*!
 * \file CvFilter2D_Processor.cpp
 * \brief
 * \author mstefanc
 * \date 2010-07-05
 */

#include <memory>
#include <string>

#include "CvFilter2D_Processor.hpp"
#include "Logger.hpp"

namespace Processors {
namespace CvFilter2D {

CvFilter2D_Processor::CvFilter2D_Processor(const std::string & name) : Base::Component(name)
{
	LOG(LTRACE) << "Hello CvFilter2D_Processor\n";
}

CvFilter2D_Processor::~CvFilter2D_Processor()
{
	LOG(LTRACE) << "Good bye CvFilter2D_Processor\n";
}

bool CvFilter2D_Processor::onInit()
{
	LOG(LTRACE) << "CvFilter2D_Processor::initialize\n";

	h_onNewImage.setup(this, &CvFilter2D_Processor::onNewImage);
	registerHandler("onNewImage", &h_onNewImage);

	registerStream("in_img", &in_img);

	newImage = registerEvent("newImage");

	registerStream("out_img", &out_img);

	return true;
}

bool CvFilter2D_Processor::onFinish()
{
	LOG(LTRACE) << "CvFilter2D_Processor::finish\n";

	return true;
}

bool CvFilter2D_Processor::onStep()
{
	LOG(LTRACE) << "CvFilter2D_Processor::step\n";
	return true;
}

bool CvFilter2D_Processor::onStop()
{
	return true;
}

bool CvFilter2D_Processor::onStart()
{
	return true;
}

void CvFilter2D_Processor::onNewImage()
{
	LOG(LTRACE) << "CvFilter2D_Processor::onNewImage\n";
	try {
		cv::Mat img = in_img.read();

		//img.convertTo(tmp, CV_32F, 1./255);
		cv::filter2D(img, tmp, -1, props.kernel, cv::Size(-1, -1), props.delta, cv::BORDER_REPLICATE);

		out_img.write(tmp);

		newImage->raise();
	} catch (...) {
		LOG(LERROR) << "CvFilter2D_Processor::onNewImage failed\n";
	}
}

}//: namespace CvFilter2D
}//: namespace Processors
