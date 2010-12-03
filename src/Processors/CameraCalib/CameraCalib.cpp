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

using namespace cv;
using namespace std;

CameraCalib_Processor::CameraCalib_Processor(const std::string & name) :
	Base::Component(name)
{
	findChessboardCornersFlags = CV_CALIB_CB_ADAPTIVE_THRESH + CV_CALIB_CB_NORMALIZE_IMAGE + CV_CALIB_CB_FAST_CHECK;
}

CameraCalib_Processor::~CameraCalib_Processor()
{
}

bool CameraCalib_Processor::onInit()
{
	h_onNewImage.setup(this, &CameraCalib_Processor::onNewImage);
	registerHandler("onNewImage", &h_onNewImage);

	h_onStoreLastImage.setup(this, &CameraCalib_Processor::onStoreLastImage);
	registerHandler("onStoreLastImage", &h_onStoreLastImage);

	h_onSequenceEnd.setup(this, &CameraCalib_Processor::onSequenceEnd);
	registerHandler("onSequenceEnd", &h_onSequenceEnd);

	registerStream("in_img", &in_img);

	chessboardModelPoints.clear();
	objectPoints.clear();
	imagePoints.clear();
	lastImagePoints.clear();
	imageSize = Size(0, 0);

	for (int i = 0; i < props.patternSize.height; ++i) {
		for (int j = 0; j < props.patternSize.width; ++j) {
			chessboardModelPoints.push_back(Point3f(-j * props.squareSize, -i * props.squareSize, 0));
		}
	}

	return true;
}

bool CameraCalib_Processor::onStep()
{
	return true;
}

bool CameraCalib_Processor::onFinish()
{
	return true;
}

bool CameraCalib_Processor::onStop()
{
	return true;
}

void CameraCalib_Processor::onNewImage()
{
	try {
		Mat image = in_img.read();
		if (objectPoints.size() == 0) {
			imageSize = image.size();
		}

		if (imageSize != image.size()) {
			LOG(LERROR) << "CameraCalib_Processor::onNewImage() image size is wrong. Ignoring frame.\n";
			return;
		}

		bool found = findChessboardCorners(image, props.patternSize, lastImagePoints, findChessboardCornersFlags);

		if (found) {
			LOG(LTRACE) << "chessboard found\n";

			if (props.findSubpix) {
				cornerSubPix(image, lastImagePoints, Size(5, 5), Size(1, 1), TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 50, 1e-3));
			}

			if(props.storeOnNewImage){
				addImageToSet();
			}

			Types::Objects3D::Chessboard chessboard(props.patternSize, props.squareSize);

			chessboard.setImagePoints(lastImagePoints);
			chessboard.setModelPoints(chessboardModelPoints);
			out_chessboard.write(chessboard);

			chessboardFound->raise();
		} else {
			LOG(LTRACE) << "chessboard not found\n";

			chessboardNotFound->raise();
		}
	} catch (const Exception& e) {
		LOG(LERROR) << e.what() << "\n";
	}

	if (props.storeOnNewImage) {

	}
}

void CameraCalib_Processor::onStoreLastImage()
{
	addImageToSet();
}

void CameraCalib_Processor::addImageToSet()
{
	if (lastImagePoints.size() != chessboardModelPoints.size() || lastImagePoints.size() == 0) {
		LOG(LERROR) << "CameraCalib_Processor::addImageToSet() nothing to add to set.\n";
		return;
	}
	imagePoints.push_back(lastImagePoints);
	objectPoints.push_back(chessboardModelPoints);
}

void CameraCalib_Processor::onSequenceEnd()
{
	if(imagePoints.size() < 3){
		LOG(LERROR) << "CameraCalib_Processor::onSequenceEnd(): imagePoints.size() < 3\n";
	}
	Mat cameraMatrix;
	Mat distCoeffs;
	Mat rvecs;
	Mat tvecs;
	int flags;
//	double ret = calibrateCamera(objectPoints, imagePoints, imageSize, cameraMatrix, distCoeffs, rvecs, tvecs, flags);
}

} // namespace CameraCalib

} // namespace Processors
