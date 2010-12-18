/*
 * CvFindChessboardCorners_Processor.cpp
 *
 *  Created on: 16-10-2010
 *      Author: mateusz
 */

#include "CvFindChessboardCorners_Processor.hpp"

namespace Processors {

namespace CvFindChessboardCorners {

using namespace std;
using namespace boost;
using namespace cv;
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

	LOG(LINFO) << "CvFindChessboardCorners_Processor: width: " << props.patternSize.width << "\n";
	LOG(LINFO) << "CvFindChessboardCorners_Processor: height: " << props.patternSize.height << "\n";
	LOG(LINFO) << "CvFindChessboardCorners_Processor: squareSize: " << props.squareSize << "\n";

	chessboard = boost::shared_ptr <Chessboard>(new Chessboard(props.patternSize, props.squareSize));

	vector <Point3f> modelPoints;
	for (int i = 0; i < props.patternSize.height; ++i) {
		for (int j = 0; j < props.patternSize.width; ++j) {
			modelPoints.push_back(Point3f(-j * props.squareSize, -i * props.squareSize, 0));
		}
	}

	chessboard->setModelPoints(modelPoints);

	LOG(LTRACE) << "component initialized\n";
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
	LOG(LTRACE) << "void CvFindChessboardCorners_Processor::onNewImage() begin\n";
	try {
		Mat image = in_img.read();

		timer.restart();

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
	LOG(LTRACE) << "void CvFindChessboardCorners_Processor::onNewImage() end\n";
}

} // namespace CvFindChessboardCorners {
} // namespace Processors {
