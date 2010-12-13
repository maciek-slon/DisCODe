/*
 * CameraCalib.cpp
 *
 *  Created on: 2010-06-21
 *      Author: konradb3
 */

#include <boost/filesystem.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <fstream>

#include <highgui.h>

#include "CameraCalib.h"

namespace Processors {
namespace CameraCalib {

using namespace cv;
using namespace std;
using namespace boost::posix_time;
using namespace boost::interprocess;

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
	registerStream("out_chessboard", &out_chessboard);

	chessboardFound = registerEvent("chessboardFound");
	chessboardNotFound = registerEvent("chessboardNotFound");

	chessboardModelPoints.clear();
	objectPoints.clear();
	imagePoints.clear();
	lastImagePoints.clear();
	imageSize = Size(0, 0);

	for (int i = 0; i < props.patternSize.height; ++i) {
		for (int j = 0; j < props.patternSize.width; ++j) {
			Point3f p(j * props.squareSize, i * props.squareSize, 0);
			chessboardModelPoints.push_back(p);
			LOG(LDEBUG) << "\t" << p.x << "\t" << p.y;
		}
	}

	lastImageAlreadySaved = true;

	try {
		if (props.saveImages) {
			if (boost::filesystem::create_directories(props.imagesBaseDirectory)) {
				LOG(LNOTICE) << "Directory \"" << props.imagesBaseDirectory << "\" has been created.\n";
			}
		}
	} catch (exception &e) {
		LOG(LERROR) << "Couldn't create directory \"" << props.imagesBaseDirectory << "\": " << e.what() << "\n";
	}

	return true;
}

bool CameraCalib_Processor::onStart()
{
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
	scoped_lock<interprocess_mutex> lock(eventsMutex);
	try {
		image = in_img.read().clone();
		if (objectPoints.size() == 0) {
			imageSize = image.size();
		}

		if (imageSize != image.size()) {
			LOG(LERROR) << "CameraCalib_Processor::onNewImage() image size is wrong. Ignoring frame.\n";
			return;
		}

		lastImagePoints.clear();
		bool found = findChessboardCorners(image, props.patternSize, lastImagePoints, findChessboardCornersFlags);
		LOG(LDEBUG) << "lastImagePoints.size()=" << lastImagePoints.size();

		if (found) {
			LOG(LINFO) << "chessboard found\n";

			if (props.findSubpix) {
				cornerSubPix(image, lastImagePoints, Size(5, 5), Size(1, 1), TermCriteria(CV_TERMCRIT_EPS
						| CV_TERMCRIT_ITER, 50, 1e-3));
				LOG(LDEBUG) << "lastImagePoints.size()=" << lastImagePoints.size();
			}

			if (props.storeOnNewImage) {
				addImageToSet();
				lastImageAlreadySaved = true;
			} else {
				lastImageAlreadySaved = false;
			}

			Types::Objects3D::Chessboard chessboard(props.patternSize, props.squareSize);

			chessboard.setImagePoints(lastImagePoints);
			chessboard.setModelPoints(chessboardModelPoints);
			out_chessboard.write(chessboard);

			chessboardFound->raise();
		} else {
			LOG(LINFO) << "chessboard not found\n";

			chessboardNotFound->raise();
		}
	} catch (const Exception& e) {
		LOG(LERROR) << e.what() << "\n";
	}
}

void CameraCalib_Processor::onStoreLastImage()
{
	scoped_lock<interprocess_mutex> lock(eventsMutex);
	if (lastImageAlreadySaved) {
		LOG(LWARNING) << "CameraCalib_Processor::onStoreLastImage(): image already saved.\n";
		return;
	}

	LOG(LTRACE) << "CameraCalib_Processor::onStoreLastImage(): saving image.\n";
	addImageToSet();
	lastImageAlreadySaved = true;
}

void CameraCalib_Processor::addImageToSet()
{
	LOG(LTRACE) << "CameraCalib_Processor::addImageToSet()\n";
	if (lastImagePoints.size() != chessboardModelPoints.size() || lastImagePoints.size() == 0) {
		LOG(LERROR) << "CameraCalib_Processor::addImageToSet() nothing to add to set.\n";
		LOG(LDEBUG) << "lastImagePoints.size(): " << lastImagePoints.size();
		LOG(LDEBUG) << "chessboardModelPoints.size(): " << chessboardModelPoints.size();
		return;
	}
	imagePoints.push_back(lastImagePoints);
	objectPoints.push_back(chessboardModelPoints);

	if (props.saveImages) {
		if (imagePoints.size() == 1) { // we're saving first image. Create directory
			currentDirectory = props.imagesBaseDirectory + "/CameraCalib_" + getTimeAsString();
			LOG(LNOTICE) << "CameraCalib_Processor::addImageToSet(): creating directory: \"" << currentDirectory
					<< "\"\n";
			try {
				if (!boost::filesystem::create_directory(currentDirectory)) {
					LOG(LWARNING) << "Directory \"" << currentDirectory
							<< "\" already exists. Data will be overwritten.\n";
				}
			} catch (const exception &ex) {
				LOG(LERROR) << "Couldn't create directory \"" << currentDirectory << "\" for storing images.\n";
				LOG(LNOTICE) << "Check imagesBaseDirectory property.\n";
			}
		}

		stringstream ss;

		ss << currentDirectory << "/CalibrationImage_";
		ss.fill('0');
		ss.width(4);
		ss << imagePoints.size() << ".png";

		LOG(LNOTICE) << "Saving image \"" << ss.str() << "\"\n";

		imwrite(ss.str().c_str(), image);
	}
}

void CameraCalib_Processor::onSequenceEnd()
{
	scoped_lock<interprocess_mutex> lock(eventsMutex);
	if (imagePoints.size() < 3) {
		LOG(LERROR) << "CameraCalib_Processor::onSequenceEnd(): imagePoints.size() < 3\n";
		return;
	}
	Mat cameraMatrix = Mat::eye(Size(3, 3), CV_64F);
	Mat distCoeffs = Mat::zeros(Size(8, 1), CV_64F);
	vector <Mat> rvecs;
	vector <Mat> tvecs;

	LOG(LNOTICE) << "CameraCalib_Processor::onSequenceEnd(): running calibrateCamera() with " << imagePoints.size()
			<< " data items\n";
	double reprojectionError =
			calibrateCamera(objectPoints, imagePoints, imageSize, cameraMatrix, distCoeffs, rvecs, tvecs);
	LOG(LNOTICE) << "CameraCalib_Processor::onSequenceEnd(): calibrateCamera() finished\n";
	saveResults(cameraMatrix, distCoeffs, reprojectionError);

	imagePoints.clear();
	objectPoints.clear();
}

std::string CameraCalib_Processor::getTimeAsString()
{
	stringstream ss;
	ptime now(second_clock::local_time());
	time_facet* facet(new time_facet("%Y%m%d%H%M%S"));
	ss.imbue(std::locale(std::cout.getloc(), facet));
	ss << now;
	//delete facet;
	return ss.str();
}

void CameraCalib_Processor::saveResults(cv::Mat cameraMatrix, cv::Mat distCoeffs, double reprojectionError)
{
	stringstream ss;
	ss << "calculationFinishedAt = " << getTimeAsString() << endl;
	ss << "reprojectionError = " << reprojectionError << endl;
	ss << "cameraMatrix = [\n\t";
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			ss << cameraMatrix.at <double> (i, j) << "\t";
		}
		if (i < 2) {
			ss << ";\n\t";
		}
	}
	ss << "\n]\n";

	ss << "distCoeffs = [";
	for (int i = 0; i < distCoeffs.cols; ++i) {
		ss << distCoeffs.at <double> (i, 0);
		if (i < distCoeffs.cols - 1) {
			ss << "\t";
		}
	}
	ss << "]\n";
	LOG(LNOTICE) << ss.str();

	try {
		ofstream of(props.resultsFilename);
		of << ss.str();
		of.close();
	} catch (const exception &ex) {
		LOG(LERROR) << "Error writing to file resultsFilename \"" << props.resultsFilename << "\"\n";
	}
}

} // namespace CameraCalib

} // namespace Processors
