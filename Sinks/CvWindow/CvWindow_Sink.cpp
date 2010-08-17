/*!
 * \file CvWindow_Sink.cpp
 * \brief
 * \author mstefanc
 * \date 2010-05-15
 */

#include <memory>
#include <string>
#include <iostream>

#include "CvWindow_Sink.hpp"
#include "Logger.hpp"

namespace Sinks {
namespace CvWindow {

CvWindow_Sink::CvWindow_Sink(const std::string & name) : Base::Component(name) {
	LOG(TRACE)<<"Hello CvWindow_Sink\n";
}

CvWindow_Sink::~CvWindow_Sink() {
	LOG(TRACE)<<"Good bye CvWindow_Sink\n";
}

bool CvWindow_Sink::onInit() {
	LOG(TRACE) << "CvWindow_Sink::initialize\n";

	h_onNewImage.setup(this, &CvWindow_Sink::onNewImage);
	registerHandler("onNewImage", &h_onNewImage);

	registerStream("in_img", &in_img);

	return true;
}

bool CvWindow_Sink::onFinish() {
	LOG(TRACE) << "CvWindow_Sink::finish\n";

	return true;
}

bool CvWindow_Sink::onStep()
{
	LOG(TRACE)<<"CvWindow_Sink::step\n";
	return true;
}

bool CvWindow_Sink::onStop()
{
	return true;
}

bool CvWindow_Sink::onStart()
{
	return true;
}

void CvWindow_Sink::onNewImage() {
	try {
		imshow( props.title, in_img.read() );
		waitKey( 2 );
	}
	catch(...) {
		LOG(ERROR) << "CvWindow::onNewImage failed\n";
	}
}


}//: namespace CvWindow
}//: namespace Sinks
