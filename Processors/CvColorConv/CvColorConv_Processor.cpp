/*!
 * \file CvColorConv_Processor.cpp
 * \brief
 * \author mstefanc
 * \date 2010-07-05
 */

#include <memory>
#include <string>

#include "CvColorConv_Processor.hpp"
#include "Logger.hpp"

namespace Processors {
namespace CvColorConv {

CvColorConv_Processor::CvColorConv_Processor()
{
	LOG(TRACE) << "Hello CvThreshold_Processor\n";
}

CvColorConv_Processor::~CvColorConv_Processor()
{
	LOG(TRACE) << "Good bye CvThreshold_Processor\n";
}

bool CvColorConv_Processor::initialize()
{
	LOG(TRACE) << "CvThreshold_Processor::initialize\n";

	h_onNewImage.setup(this, &CvColorConv_Processor::onNewImage);
	registerHandler("onNewImage", &h_onNewImage);

	registerStream("in_img", &in_img);

	newImage = registerEvent("newImage");

	registerStream("out_img", &out_img);

	return true;
}

bool CvColorConv_Processor::finish()
{
	LOG(TRACE) << "CvThreshold_Processor::finish\n";

	return true;
}

int CvColorConv_Processor::step()
{
	LOG(TRACE) << "CvThreshold_Processor::step\n";
	return 0;
}

void CvColorConv_Processor::onNewImage()
{
	LOG(TRACE) << "CvThreshold_Processor::onNewImage\n";
	try {
		cv::Mat img = in_img.read();
		cv::Mat out = img.clone();
		cvtColor(img, out, props.type);
		out_img.write(out);
		newImage->raise();
	} catch (...) {
		LOG(ERROR) << "CvThreshold::onNewImage failed\n";
	}
}

}//: namespace CvColorConv
}//: namespace Processors
