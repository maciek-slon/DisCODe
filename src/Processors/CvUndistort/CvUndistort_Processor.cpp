/*
 * CvUndistort_Processor.cpp
 *
 *  Created on: Oct 20, 2010
 *      Author: mboryn
 */

#include "CvUndistort_Processor.hpp"

namespace Processors {

namespace CvUndistort {

using namespace std;
using namespace cv;
using namespace boost;
using namespace Base;

CvUndistort_Processor::CvUndistort_Processor(const std::string& n) :
	Component(n)
{
}

CvUndistort_Processor::~CvUndistort_Processor()
{
}

Props * CvUndistort_Processor::getProperties()
{
	return &props;
}

bool CvUndistort_Processor::onStart()
{
	return true;
}

bool CvUndistort_Processor::onStop()
{
	return true;
}
bool CvUndistort_Processor::onInit()
{
	h_onNewImage.setup(this, &CvUndistort_Processor::onNewImage);
	registerHandler("onNewImage", &h_onNewImage);
	newUndistortedImage = registerEvent("newUndistortedImage");
	registerStream("in_img", &in_img);
	registerStream("out_img", &out_img);

	interpolation = INTER_LINEAR;

	return true;
}
bool CvUndistort_Processor::onFinish()
{
	return true;
}
bool CvUndistort_Processor::onStep()
{
	return true;
}

void CvUndistort_Processor::onNewImage()
{
	cv::Mat originalImage;

	originalImage = in_img.read();

	//cv::Mat undistortedImage = originalImage.clone();
	cv::Mat undistortedImage;

	// TODO: replace with initUndistortRectifyMap + remap
	//initUndistortRectifyMap(props.cameraMatrix, props.distCoeffs, Mat(), );
	//remap(originalImage, undistortedImage, map1, map2, interpolation);
	undistort(originalImage, undistortedImage, props.cameraMatrix, props.distCoeffs);

	out_img.write(undistortedImage.clone());
	newUndistortedImage->raise();
}

} // namespace CvUndistort

} // namespace Processors
