/*!
 * \file CvMorphology_Processor.cpp
 * \brief
 * \author mstefanc
 * \date 2010-07-05
 */

#include <memory>
#include <string>

#include "CvMorphology_Processor.hpp"
#include "Logger.hpp"

namespace Processors {
namespace CvMorphology {

CvMorphology_Processor::CvMorphology_Processor(const std::string & name) : Base::Component(name)
{
	LOG(LTRACE) << "Hello CvMorphology_Processor\n";
}

CvMorphology_Processor::~CvMorphology_Processor()
{
	LOG(LTRACE) << "Good bye CvMorphology_Processor\n";
}

bool CvMorphology_Processor::onInit()
{
	LOG(LTRACE) << "CvMorphology_Processor::initialize\n";

	h_onNewImage.setup(this, &CvMorphology_Processor::onNewImage);
	registerHandler("onNewImage", &h_onNewImage);

	registerStream("in_img", &in_img);

	newImage = registerEvent("newImage");

	registerStream("out_img", &out_img);

	return true;
}

bool CvMorphology_Processor::onFinish()
{
	LOG(LTRACE) << "CvMorphology_Processor::finish\n";

	return true;
}

bool CvMorphology_Processor::onStep()
{
	LOG(LTRACE) << "CvMorphology_Processor::step\n";
	return true;
}

bool CvMorphology_Processor::onStop()
{
	return true;
}

bool CvMorphology_Processor::onStart()
{
	return true;
}

void CvMorphology_Processor::onNewImage()
{
	LOG(LTRACE) << "CvMorphology_Processor::onNewImage\n";
	try {
		cv::Mat img = in_img.read();
		//cv::Mat out = img.clone();
		cv::morphologyEx(img, img, props.type, cv::Mat(), Point(-1, -1), props.iterations);
		out_img.write(img);
		newImage->raise();
	} catch (...) {
		LOG(LERROR) << "CvMorphology_Processor::onNewImage failed\n";
	}
}

}//: namespace CvMorphology
}//: namespace Processors
