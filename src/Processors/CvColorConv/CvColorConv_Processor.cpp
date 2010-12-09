/*!
 * \file CvColorConv_Processor.cpp
 * \brief
 * \author mstefanc
 * \date 2010-07-05
 */

#include <memory>
#include <string>
#include <stdexcept>

#include "CvColorConv_Processor.hpp"
#include "Logger.hpp"

namespace Processors {
namespace CvColorConv {

CvColorConv_Processor::CvColorConv_Processor(const std::string & name) : Base::Component(name)
{
	LOG(LTRACE) << "Hello CvThreshold_Processor\n";
}

CvColorConv_Processor::~CvColorConv_Processor()
{
	LOG(LTRACE) << "Good bye CvThreshold_Processor\n";
}

bool CvColorConv_Processor::onInit()
{
	LOG(LTRACE) << "CvThreshold_Processor::initialize\n";

	h_onNewImage.setup(this, &CvColorConv_Processor::onNewImage);
	registerHandler("onNewImage", &h_onNewImage);

	registerStream("in_img", &in_img);

	newImage = registerEvent("newImage");

	registerStream("out_img", &out_img);

	return true;
}

bool CvColorConv_Processor::onFinish()
{
	LOG(LTRACE) << "CvThreshold_Processor::finish\n";

	return true;
}

bool CvColorConv_Processor::onStep()
{
	LOG(LTRACE) << "CvThreshold_Processor::step\n";
	return true;
}

bool CvColorConv_Processor::onStop()
{
	return true;
}

bool CvColorConv_Processor::onStart()
{
	return true;
}

void CvColorConv_Processor::onNewImage()
{
	LOG(LTRACE) << "CvThreshold_Processor::onNewImage\n";
	try {
		img = in_img.read();
		cvtColor(img, out, props.type);
		out_img.write(out);
		newImage->raise();
	} catch (const exception& ex) {
		LOG(LERROR) << "CvColorConv_Processor::onNewImage() failed. " << ex.what() << endl;
	}
}

}//: namespace CvColorConv
}//: namespace Processors
