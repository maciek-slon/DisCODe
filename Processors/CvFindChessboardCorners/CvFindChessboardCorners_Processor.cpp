/*
 * CvFindChessboardCorners_Processor.cpp
 *
 *  Created on: 16-10-2010
 *      Author: mateusz
 */

#include "CvFindChessboardCorners_Processor.hpp"

namespace Processors {

namespace CvFindChessboardCorners {

using namespace Types::Objects3D;

CvFindChessboardCorners_Processor::CvFindChessboardCorners_Processor(const std::string & name) :
	Component(name)
{
	findChessboardCornersFlags = CV_CALIB_CB_ADAPTIVE_THRESH + CV_CALIB_CB_NORMALIZE_IMAGE + CV_CALIB_CB_FAST_CHECK;
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
	h_onNewImage.setup(this, &CvFindChessboardCorners_Processor::onNewImage);
	registerHandler("onNewImage", &h_onNewImage);

	registerStream("in_img", &in_img);
	registerStream("out_chessboard", &out_chessboard);

	chessboardFound = registerEvent("chessboardFound");
	chessboardNotFound = registerEvent("chessboardNotFound");

	LOG(INFO) << "CvFindChessboardCorners_Processor: width: "<< props.patternSize.width << "\n";
	LOG(INFO) << "CvFindChessboardCorners_Processor: height: "<< props.patternSize.height << "\n";
	LOG(INFO) << "CvFindChessboardCorners_Processor: squareSize: "<< props.squareSize << "\n";

	LOG(TRACE) << "component initialized\n";
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
	LOG(TRACE) << "void CvFindChessboardCorners_Processor::onNewImage() begin\n";
	try{
		Mat image = in_img.read();

		timer.restart();

		bool found = findChessboardCorners(image, props.patternSize, corners, findChessboardCornersFlags);

		LOG(INFO) << "findChessboardCorners() execution time: "<< timer.elapsed() << " s\n";

		if(found){
			LOG(TRACE) << "chessboard found\n";

//			Chessboard *cb = new Chessboard(props.patternSize, props.squareSize);
//			cb->setImagePoints(corners);
//
//			Types::DrawableContainer dc;
//
//			dc.add(cb);
//			out_chessboard.write(dc);

			Chessboard cb(props.patternSize, props.squareSize);
			cb.setImagePoints(corners);
			out_chessboard.write(cb);

			chessboardFound->raise();
		} else {
			LOG(TRACE) << "chessboard not found\n";

			chessboardNotFound->raise();
		}
	} catch (const Exception& e) {
		LOG(ERROR) << e.what() << "\n";
	}
	LOG(TRACE) << "void CvFindChessboardCorners_Processor::onNewImage() end\n";
}

} // namespace CvFindChessboardCorners {

} // namespace Processors {
