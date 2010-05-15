/*!
 * \file RGBtoHSV_Processor.cpp
 * \brief
 * \author mstefanc
 * \date 2010-05-15
 */

#include <memory>
#include <string>
#include <iostream>

#include "RGBtoHSV_Processor.hpp"

namespace Processors {
namespace RGBtoHSV {

RGBtoHSV_Processor::RGBtoHSV_Processor() {
	cout<<"Hello RGBtoHSV_Processor\n";

	initialize();
}

RGBtoHSV_Processor::~RGBtoHSV_Processor() {
	finish();

	cout<<"Good bye RGBtoHSV_Processor\n";
}

void RGBtoHSV_Processor::initialize() {
	std::cout << "RGBtoHSV_Processor::initialize\n";

	h_onNewImage.setup(this, &RGBtoHSV_Processor::onNewImage);
	registerHandler("onNewImage", &h_onNewImage);

	registerStream("in_img", &in_img);
}

void RGBtoHSV_Processor::finish() {
	std::cout << "RGBtoHSV_Processor::finish\n";
}

int RGBtoHSV_Processor::step()
{
	cout<<"RGBtoHSV_Processor::step\n";
	return 0;
}

void RGBtoHSV_Processor::onNewImage() {
	cvShowImage( "video", in_img.read() );
	cvWaitKey( 1 );
}


}//: namespace RGBtoHSV
}//: namespace Processors
