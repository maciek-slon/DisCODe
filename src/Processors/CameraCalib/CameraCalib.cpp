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

	h_onCaptureNow.setup(this, &CameraCalib_Processor::onCaptureNow);
	registerHandler("onCaptureNow", &h_onCaptureNow);

	h_onSequenceEnd.setup(this, &CameraCalib_Processor::onSequenceEnd);
	registerHandler("onSequenceEnd", &h_onSequenceEnd);

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
	bool ret = findChessboardCorners(frame, Size(4, 3), points, CV_CALIB_CB_ADAPTIVE_THRESH
			+ CV_CALIB_CB_NORMALIZE_IMAGE + CV_CALIB_CB_FAST_CHECK);
	if (ret) {
		cornerSubPix(frame, points, Size(5, 5), Size(-1, -1), TermCriteria(TermCriteria::MAX_ITER + TermCriteria::EPS, 200, 10e-6));
		drawChessboardCorners(frame, Size(4, 3), Mat(points), ret);
	}
	imshow("test", frame);
	waitKey(2);

	try {
		Mat image = in_img.read();

		bool found = findChessboardCorners(image, props.patternSize, corners, findChessboardCornersFlags);

		LOG(LINFO) << "findChessboardCorners() execution time: " << timer.elapsed() << " s\n";

		if (found) {
			LOG(LTRACE) << "chessboard found\n";

			if (props.findSubpix) {
				cornerSubPix(image, corners, Size(5, 5), Size(1, 1), TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 50, 1e-3));
			}

			chessboard->setImagePoints(corners);
			out_chessboard.write(*chessboard);

			chessboardFound->raise();
		} else {
			LOG(LTRACE) << "chessboard not found\n";

			chessboardNotFound->raise();
		}
	} catch (const Exception& e) {
		LOG(LERROR) << e.what() << "\n";
	}
}

void CameraCalib_Processor::onCaptureNow()
{

}

void CameraCalib_Processor::onSequenceEnd()
{

}

} // namespace CameraCalib

} // namespace Processors
