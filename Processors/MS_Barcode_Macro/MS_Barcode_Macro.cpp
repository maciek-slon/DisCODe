/*!
 * \file MS_Barcode_Macro.cpp
 * \brief
 * \author mstefanc
 * \date 2010-07-05
 */

#include <memory>
#include <string>

#include "MS_Barcode_Macro.hpp"
#include "Logger.hpp"

#include "Types/Ellipse.hpp"

namespace Processors {
namespace MS_Barcode {

MS_Barcode_Macro::MS_Barcode_Macro(const std::string & name) : Base::Component(name)
{
	LOG(TRACE) << "Hello MS_Barcode_Macro\n";
}

MS_Barcode_Macro::~MS_Barcode_Macro()
{
	LOG(TRACE) << "Good bye MS_Barcode_Macro\n";
}

bool MS_Barcode_Macro::onInit()
{
	LOG(TRACE) << "MS_Barcode_Macro::initialize\n";

	h_onNewImage.setup(this, &MS_Barcode_Macro::onNewImage);
	registerHandler("onNewImage", &h_onNewImage);

	registerStream("in_img", &in_img);

	newImage = registerEvent("newImage");

	registerStream("out_img", &out_img);

	registerStream("out_sf1", &out_sf1);
	registerStream("out_sfe1", &out_sfe1);

	registerStream("out_sf2", &out_sf2);
	registerStream("out_sfe2", &out_sfe2);

	return true;
}

bool MS_Barcode_Macro::onFinish()
{
	LOG(TRACE) << "MS_Barcode_Macro::finish\n";

	return true;
}

bool MS_Barcode_Macro::onStep()
{
	LOG(TRACE) << "MS_Barcode_Macro::step\n";

	return true;
}

bool MS_Barcode_Macro::onStop()
{
	return true;
}

bool MS_Barcode_Macro::onStart()
{
	return true;
}

void MS_Barcode_Macro::onNewImage()
{
	LOG(TRACE) << "MS_Barcode_Macro::onNewImage\n";

	cv::Mat img = in_img.read();

	img.convertTo(sf, CV_32FC1, 1./255.);

	cv::filter2D(sf, sf1, -1, props.kernel_1, cv::Size(-1, -1), 0, cv::BORDER_REPLICATE);
	cv::normalize(sf1, sf1, 1.0, 0.0, CV_C);
	cv::dilate(sf1, sfe1, cv::Mat(), cv::Point(-1,-1), 2);

	cv::filter2D(sf, sf2, -1, props.kernel_2, cv::Size(-1, -1), 0, cv::BORDER_REPLICATE);
	cv::normalize(sf2, sf2, 1.0, 0.0, CV_C);
	cv::dilate(sf2, sfe2, cv::Mat(), cv::Point(-1,-1), 2);

	sf1 = sf1 - sfe2;
	sf2 = sf2 - sfe1;

	cv::dilate(sf1, sf1, cv::Mat(), cv::Point(-1,-1), 3);
	cv::erode(sf1, sf1, cv::Mat(), cv::Point(-1,-1), 6);
	cv::dilate(sf1, sf1, cv::Mat(), cv::Point(-1,-1), 3);

	cv::dilate(sf2, sf2, cv::Mat(), cv::Point(-1,-1), 3);
	cv::erode(sf2, sf2, cv::Mat(), cv::Point(-1,-1), 6);
	cv::dilate(sf2, sf2, cv::Mat(), cv::Point(-1,-1), 3);

	cv::threshold(sf1, sf1, props.thresh, 1.0, CV_THRESH_BINARY);
	cv::threshold(sf2, sf2, props.thresh, 1.0, CV_THRESH_BINARY);

	sf = sf1 + sf2;

	out_sf1.write(sf1);
	out_sfe1.write(sfe1);
	out_sf2.write(sf2);
	out_sfe2.write(sfe2);
	out_img.write(sf);
	newImage->raise();
}


}//: namespace MS_Barcode
}//: namespace Processors
