/*!
 * \file MS_Barcode_Decide.cpp
 * \brief
 * \author mstefanc
 * \date 2010-07-05
 */

#include <memory>
#include <string>

#include "MS_Barcode_Decide.hpp"
#include "Logger.hpp"

#include "Types/Ellipse.hpp"

namespace Processors {
namespace MS_Barcode {

MS_Barcode_Decide::MS_Barcode_Decide(const std::string & name) : Base::Component(name)
{
	LOG(TRACE) << "Hello MS_Barcode_Decide\n";
	count = 0;
}

MS_Barcode_Decide::~MS_Barcode_Decide()
{
	LOG(TRACE) << "Good bye MS_Barcode_Decide\n";
}

bool MS_Barcode_Decide::onInit()
{
	LOG(TRACE) << "MS_Barcode_Decide::initialize\n";

	h_onNewImage1.setup(this, &MS_Barcode_Decide::onNewImage1);
	registerHandler("onNewImage1", &h_onNewImage1);

	h_onNewImage2.setup(this, &MS_Barcode_Decide::onNewImage2);
	registerHandler("onNewImage2", &h_onNewImage2);

	h_onNewImage3.setup(this, &MS_Barcode_Decide::onNewImage3);
	registerHandler("onNewImage3", &h_onNewImage3);

	h_onNewImage4.setup(this, &MS_Barcode_Decide::onNewImage4);
	registerHandler("onNewImage4", &h_onNewImage4);

	registerStream("in_img1", &in_img1);
	registerStream("in_img2", &in_img2);
	registerStream("in_img3", &in_img3);
	registerStream("in_img4", &in_img4);

	newImage = registerEvent("newImage");

	registerStream("out_img", &out_img);

	return true;
}

bool MS_Barcode_Decide::onFinish()
{
	LOG(TRACE) << "MS_Barcode_Decide::finish\n";

	return true;
}

bool MS_Barcode_Decide::onStep()
{
	LOG(TRACE) << "MS_Barcode_Decide::step\n";

	count = 0;

	cv::threshold(sum, sum, props.thresh, 1.0, CV_THRESH_BINARY);
	//cv::normalize(sum, sum, 1.0, 0.0, CV_C);

	out_img.write(sum);
	newImage->raise();

	return true;
}

bool MS_Barcode_Decide::onStop()
{
	return true;
}

bool MS_Barcode_Decide::onStart()
{
	return true;
}

void MS_Barcode_Decide::onNewImage1()
{
	LOG(TRACE) << "MS_Barcode_Decide::onNewImage1\n";

	cv::Mat img = in_img1.read();

	if (count == 0)
		sum = img.clone();
	else
		sum = sum + img;

	if (++count == 4)
		onStep();
}

void MS_Barcode_Decide::onNewImage2()
{
	LOG(TRACE) << "MS_Barcode_Decide::onNewImage2\n";

	cv::Mat img = in_img2.read();

	if (count == 0)
		sum = img.clone();
	else
		sum = sum + img;

	if (++count == 4)
		onStep();
}

void MS_Barcode_Decide::onNewImage3()
{
	LOG(TRACE) << "MS_Barcode_Decide::onNewImage3\n";

	cv::Mat img = in_img3.read();

	if (count == 0)
		sum = img.clone();
	else
		sum = sum + img;

	if (++count == 4)
		onStep();
}

void MS_Barcode_Decide::onNewImage4()
{
	LOG(TRACE) << "MS_Barcode_Decide::onNewImage4\n";

	cv::Mat img = in_img4.read();

	if (count == 0)
		sum = img.clone();
	else
		sum = sum + img;

	if (++count == 4)
		onStep();
}


}//: namespace MS_Barcode
}//: namespace Processors
