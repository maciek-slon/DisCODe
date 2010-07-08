/*!
 * \file RGBtoHSV_Processor.cpp
 * \brief
 * \author mstefanc
 * \date 2010-05-15
 */

#include <memory>
#include <string>
#include <iostream>

#include "OpenCVWnd_Processor.hpp"
#include "Logger.hpp"

namespace Processors {
namespace OpenCVWnd {

OpenCVWnd_Processor::OpenCVWnd_Processor(const std::string & name) : Base::Component(name) {
	cout<<"Hello OpenCVWnd_Processor\n";
}

OpenCVWnd_Processor::~OpenCVWnd_Processor() {
	cout<<"Good bye OpenCVWnd_Processor\n";
}

bool OpenCVWnd_Processor::onInit() {
	std::cout << "OpenCVWnd_Processor::initialize\n";

	h_onNewImage.setup(this, &OpenCVWnd_Processor::onNewImage);
	registerHandler("onNewImage", &h_onNewImage);

	registerStream("in_img", &in_img);

	return true;
}

bool OpenCVWnd_Processor::onFinish() {
	std::cout << "OpenCVWnd_Processor::finish\n";

	return true;
}

bool OpenCVWnd_Processor::onStep()
{
	cout<<"OpenCVWnd_Processor::step\n";
	return true;
}

bool OpenCVWnd_Processor::onStop()
{
	return true;
}

bool OpenCVWnd_Processor::onStart()
{
	return true;
}

void OpenCVWnd_Processor::onNewImage() {
	try {
		imshow( props.title, in_img.read() );
		waitKey( 2 );
	}
	catch(...) {
		LOG(ERROR) << "OpenCVWnd::onNewImage failed\n";
	}
}


}//: namespace OpenCVWnd
}//: namespace Processors
