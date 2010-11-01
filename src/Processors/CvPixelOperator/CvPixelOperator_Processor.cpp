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
	LOG(LTRACE) << "Hello CvPixelOperator_Processor\n";
}

CvPixelOperator_Processor::~CvPixelOperator_Processor()
{
	LOG(LTRACE) << "Good bye CvPixelOperator_Processor\n";
}

bool CvPixelOperator_Processor::onInit()
{
	LOG(LTRACE) << "CvPixelOperator_Processor::initialize\n";

	h_onNewImage.setup(this, &CvPixelOperator_Processor::onNewImage);
	registerHandler("onNewImage", &h_onNewImage);

	registerStream("in_img", &in_img);

	newImage = registerEvent("newImage");

	registerStream("out_img", &out_img);

	return true;
}

bool CvPixelOperator_Processor::onFinish()
{
	LOG(LTRACE) << "CvPixelOperator_Processor::finish\n";

	return true;
}

bool CvPixelOperator_Processor::onStep()
{
	LOG(LTRACE) << "CvPixelOperator_Processor::step\n";
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
	LOG(LTRACE) << "CvPixelOperator_Processor::onNewImage\n";
	try {
		img = in_img.read();

		cv::Size size = img.size();

		std::cout << size.width << "x" << size.height << "\n";
		// Check the arrays for continuity and, if this is the case,
		// treat the arrays as 1D vectors
		if (img.isContinuous()) {
			size.width *= size.height * img.channels();
			size.height = 1;
		}

		for (int i = 0; i < size.height; i++) {
			// when the arrays are continuous,
			// the outer loop is executed only once
			// if not - it's executed for each row
			uchar* img_p = img.ptr <uchar> (i);

			int j;
			for (j = 0; j < size.width; ++j) {
				img_p[j] = (*props.op)(img_p[j]);
			}
		}

		out_img.write(img);

		newImage->raise();
	} catch (...) {
		LOG(LERROR) << "CvPixelOperator_Processor::onNewImage failed\n";
	}
}

}//: namespace CvPixelOperator
}//: namespace Processors
