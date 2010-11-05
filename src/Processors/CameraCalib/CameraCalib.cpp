/*
 * CameraCalib.cpp
 *
 *  Created on: 2010-06-21
 *      Author: konradb3
 */

#include "CameraCalib.h"

#include <highgui.h>

namespace Processors {

namespace CameraCalib {

CameraCalib_Processor::CameraCalib_Processor()
{
	// TODO Auto-generated constructor stub

}

CameraCalib_Processor::~CameraCalib_Processor()
{
	// TODO Auto-generated destructor stub
}

bool CameraCalib_Processor::initialize()
{
	h_onNewImage.setup(this, &CameraCalib_Processor::onNewImage);
	registerHandler("onNewImage", &h_onNewImage);

	registerStream("in_img", &in_img);

	return true;
}

int CameraCalib_Processor::step()
{
	return true;
}

bool CameraCalib_Processor::finish()
{
	return true;
}

void CameraCalib_Processor::onNewImage()
{
	vector <Point2f> points;
	frame = in_img.read();
	bool ret = findChessboardCorners(frame, Size(4, 3), points, CV_CALIB_CB_ADAPTIVE_THRESH + CV_CALIB_CB_NORMALIZE_IMAGE + CV_CALIB_CB_FAST_CHECK);
	if(ret)
	{
		cornerSubPix(frame, points, Size(5, 5), Size(-1, -1), TermCriteria(TermCriteria::MAX_ITER + TermCriteria::EPS, 200, 10e-6));
		drawChessboardCorners(frame, Size(4, 3), Mat(points), ret);
	}
	imshow("test", frame);
	waitKey(2);

}

}

}
