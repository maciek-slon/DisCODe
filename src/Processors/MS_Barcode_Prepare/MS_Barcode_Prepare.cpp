/*!
 * \file MS_Barcode_Prepare.cpp
 * \brief
 * \author mstefanc
 * \date 2010-07-05
 */

#include <memory>
#include <string>

#include "MS_Barcode_Prepare.hpp"
#include "Logger.hpp"

#include "Types/Ellipse.hpp"

namespace Processors {
namespace MS_Barcode {

MS_Barcode_Prepare::MS_Barcode_Prepare(const std::string & name) : Base::Component(name)
{
	LOG(LTRACE) << "Hello MS_Barcode_Prepare\n";
	count = 0;
}

MS_Barcode_Prepare::~MS_Barcode_Prepare()
{
	LOG(LTRACE) << "Good bye MS_Barcode_Prepare\n";
}

bool MS_Barcode_Prepare::onInit()
{
	LOG(LTRACE) << "MS_Barcode_Prepare::initialize\n";

	h_onNewImage1.setup(this, &MS_Barcode_Prepare::onNewImage1);
	registerHandler("onNewImage1", &h_onNewImage1);

	h_onNewImage2.setup(this, &MS_Barcode_Prepare::onNewImage2);
	registerHandler("onNewImage2", &h_onNewImage2);

	h_onNewImage3.setup(this, &MS_Barcode_Prepare::onNewImage3);
	registerHandler("onNewImage3", &h_onNewImage3);

	h_onNewImage4.setup(this, &MS_Barcode_Prepare::onNewImage4);
	registerHandler("onNewImage4", &h_onNewImage4);

	registerStream("in_img1", &in_img1);
	registerStream("in_img2", &in_img2);
	registerStream("in_img3", &in_img3);
	registerStream("in_img4", &in_img4);

	newImage = registerEvent("newImage");

	registerStream("out_img", &out_img);

	return true;
}

bool MS_Barcode_Prepare::onFinish()
{
	LOG(LTRACE) << "MS_Barcode_Prepare::finish\n";

	return true;
}

bool MS_Barcode_Prepare::onStep()
{
	LOG(LTRACE) << "MS_Barcode_Prepare::step\n";

	count = 0;

	cv::threshold(sum, sum, props.thresh, 1.0, CV_THRESH_BINARY);
	//cv::normalize(sum, sum, 1.0, 0.0, CV_C);

	out_img.write(sum);
	newImage->raise();

	return true;
}

bool MS_Barcode_Prepare::onStop()
{
	return true;
}

bool MS_Barcode_Prepare::onStart()
{
	return true;
}

void MS_Barcode_Prepare::onNewImage1()
{
	LOG(LTRACE) << "MS_Barcode_Prepare::onNewImage1\n";

	cv::Mat img;
	try {
		img = in_img1.read();
	}
	catch(...) {
		return;
	}

	if (count == 0)
		sum = img.clone();
	else
		sum = sum + img;

	if (++count == props.inputs)
		onStep();
}

void MS_Barcode_Prepare::onNewImage2()
{
	LOG(LTRACE) << "MS_Barcode_Prepare::onNewImage2\n";

	cv::Mat img;
	try {
		img = img = in_img2.read();
	}
	catch(...) {
		return;
	}

	if (count == 0)
		sum = img.clone();
	else
		sum = sum + img;

	if (++count == props.inputs)
		onStep();
}

void MS_Barcode_Prepare::onNewImage3()
{
	LOG(LTRACE) << "MS_Barcode_Prepare::onNewImage3\n";

	cv::Mat img;
	try {
		img = img = in_img3.read();
	}
	catch(...) {
		return;
	}

	if (count == 0)
		sum = img.clone();
	else
		sum = sum + img;

	if (++count == props.inputs)
		onStep();
}

void MS_Barcode_Prepare::onNewImage4()
{
	LOG(LTRACE) << "MS_Barcode_Prepare::onNewImage4\n";

	cv::Mat img;
	try {
		img = img = in_img4.read();
	}
	catch(...) {
		return;
	}

	if (count == 0)
		sum = img.clone();
	else
		sum = sum + img;

	if (++count == props.inputs)
		onStep();
}


}//: namespace MS_Barcode
}//: namespace Processors
