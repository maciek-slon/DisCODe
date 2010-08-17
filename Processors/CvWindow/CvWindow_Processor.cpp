/*!
 * \file CvWindow_Processor.cpp
 * \brief
 * \author mstefanc
 * \date 2010-05-15
 */

#include <memory>
#include <string>
#include <iostream>

#include "CvWindow_Processor.hpp"
#include "Logger.hpp"

namespace Processors {
namespace CvWindow {

CvWindow_Processor::CvWindow_Processor(const std::string & name) : Base::Component(name) {
	LOG(TRACE)<<"Hello CvWindow_Processor\n";
}

CvWindow_Processor::~CvWindow_Processor() {
	LOG(TRACE)<<"Good bye CvWindow_Processor\n";
}

bool CvWindow_Processor::onInit() {
	LOG(TRACE) << "CvWindow_Processor::initialize\n";

	h_onNewImage.setup(this, &CvWindow_Processor::onNewImage);
	registerHandler("onNewImage", &h_onNewImage);

	registerStream("in_img", &in_img);

	return true;
}

bool CvWindow_Processor::onFinish() {
	LOG(TRACE) << "CvWindow_Processor::finish\n";

	return true;
}

bool CvWindow_Processor::onStep()
{
	LOG(TRACE)<<"CvWindow_Processor::step\n";
	return true;
}

bool CvWindow_Processor::onStop()
{
	return true;
}

bool CvWindow_Processor::onStart()
{
	return true;
}

void CvWindow_Processor::onNewImage() {
	try {
		imshow( props.title, in_img.read() );
		waitKey( 2 );
	}
	catch(...) {
		LOG(ERROR) << "CvWindow::onNewImage failed\n";
	}
}


}//: namespace CvWindow
}//: namespace Processors
