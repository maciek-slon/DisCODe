/*
 * CvFindChessboardCorners_Processor.cpp
 *
 *  Created on: 16-10-2010
 *      Author: mateusz
 */

#include "CvFindChessboardCorners_Processor.hpp"

namespace Processors {

namespace CvFindChessboardCorners {

CvFindChessboardCorners_Processor::CvFindChessboardCorners_Processor(const std::string & name) :
	Component(name)
{
	findChessboardCornersFlags = CV_CALIB_CB_ADAPTIVE_THRESH + CV_CALIB_CB_NORMALIZE_IMAGE + CV_CALIB_CB_FAST_CHECK;
	patternSize.width = 3;
	patternSize.height = 4;
}

CvFindChessboardCorners_Processor::~CvFindChessboardCorners_Processor()
{
}

bool CvFindChessboardCorners_Processor::onFinish()
{
	return true;
}

bool CvFindChessboardCorners_Processor::onStop()
{
	return true;
}

bool CvFindChessboardCorners_Processor::onInit()
{
	LOG(FATAL) << "Mrrocpp_Proxy::onInit\n";

	h_onNewImage.setup(this, &CvFindChessboardCorners_Processor::onNewImage);
	registerHandler("onNewImage", &h_onNewImage);

	registerStream("in_img", &in_img);
	return true;
}

bool CvFindChessboardCorners_Processor::onStart()
{
	return true;
}

bool CvFindChessboardCorners_Processor::onStep()
{
	return true;
}

void CvFindChessboardCorners_Processor::onNewImage()
{
	Mat image = in_img.read();
	vector<Point2f> corners;
	bool found = findChessboardCorners(image, patternSize, corners, findChessboardCornersFlags);

}

} // namespace CvFindChessboardCorners {

} // namespace Processors {
