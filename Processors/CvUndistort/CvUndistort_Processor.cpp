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

CvUndistort_Processor::CvUndistort_Processor(const string& n) :
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


	LOG(TRACE) << "1\n";
	originalImage = in_img.read().clone();
	LOG(TRACE) << "2\n";

	cv::Mat undistortedImage = originalImage.clone();

	// TODO: replace with initUndistortRectifyMap + remap
	//initUndistortRectifyMap(props.cameraMatrix, props.distCoeffs, Mat(), );
	//remap(originalImage, undistortedImage, map1, map2, interpolation);
	undistort(originalImage, undistortedImage, props.cameraMatrix, props.distCoeffs);

	LOG(TRACE) << "3\n";
	out_img.write(undistortedImage.clone());
	LOG(TRACE) << "4\n";
	newUndistortedImage->raise();
	LOG(TRACE) << "5\n";
}

} // namespace CvUndistort

} // namespace Processors
