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
	Component(name),
	prop_subpix("subpix", true),
	prop_subpix_window("subpix_window", 9, "range"),
	prop_scale("scale", true),
	prop_scale_factor("scale_factor", 2, "range"),

	prop_width("chessboard.width", 9),
	prop_height("chessboard.height", 6),
	prop_square_width("chessboard.square_width", 1),
	prop_square_height("chessboard.square_height", 1),

	prop_fastCheck("flags.fast_check", true),
	prop_filterQuads("flags.filter_quads", true),
	prop_adaptiveThreshold("flags.adaptive_treshold", true),
	prop_normalizeImage("flags.normalize_image", true)
{

	findChessboardCornersFlags = 0;

	registerProperty(prop_subpix);

	prop_subpix_window.addConstraint("3");
	prop_subpix_window.addConstraint("11");
	registerProperty(prop_subpix_window);

	registerProperty(prop_scale);

	prop_scale_factor.addConstraint("1");
	prop_scale_factor.addConstraint("8");
	registerProperty(prop_scale_factor);

	registerProperty(prop_width);
	registerProperty(prop_height);
	registerProperty(prop_square_width);
	registerProperty(prop_square_height);

	prop_fastCheck.setCallback(boost::bind(&CvFindChessboardCorners_Processor::flagsCallback, this, _1, _2));
	registerProperty(prop_fastCheck);

	prop_filterQuads.setCallback(boost::bind(&CvFindChessboardCorners_Processor::flagsCallback, this, _1, _2));
	registerProperty(prop_filterQuads);

	prop_adaptiveThreshold.setCallback(boost::bind(&CvFindChessboardCorners_Processor::flagsCallback, this, _1, _2));
	registerProperty(prop_adaptiveThreshold);

	prop_normalizeImage.setCallback(boost::bind(&CvFindChessboardCorners_Processor::flagsCallback, this, _1, _2));
	registerProperty(prop_normalizeImage);

	prop_width.setCallback(boost::bind(&CvFindChessboardCorners_Processor::sizeCallback, this, _1, _2));
	prop_height.setCallback(boost::bind(&CvFindChessboardCorners_Processor::sizeCallback, this, _1, _2));

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
	registerStream("out_imagePosition", &out_imagePosition);

	chessboardFound = registerEvent("chessboardFound");
	chessboardNotFound = registerEvent("chessboardNotFound");

	initChessboard();

	LOG(LTRACE) << "component initialized\n";
	return true;
}

void CvFindChessboardCorners_Processor::initChessboard() {
	LOG(LINFO) << "CvFindChessboardCorners_Processor: width: " << prop_width << "\n";
	LOG(LINFO) << "CvFindChessboardCorners_Processor: height: " << prop_height << "\n";
	LOG(LINFO) << "CvFindChessboardCorners_Processor: squareSize: " << prop_square_width << "x" << prop_square_height << "m\n";

	chessboard = boost::shared_ptr <Chessboard>(new Chessboard(cv::Size(prop_width, prop_height)));

	vector <Point3f> modelPoints;
	for (int i = 0; i < prop_height; ++i) {
		for (int j = 0; j < prop_width; ++j) {
			modelPoints.push_back(Point3f(-j * prop_square_height, -i * prop_square_width, 0));
		}
	}

	chessboard->setModelPoints(modelPoints);
}

void CvFindChessboardCorners_Processor::sizeCallback(int old_value, int new_value) {
	initChessboard();
}

void CvFindChessboardCorners_Processor::flagsCallback(bool old_value, bool new_value) {
	if(prop_fastCheck){
		findChessboardCornersFlags |= CV_CALIB_CB_FAST_CHECK;
	} else {
		findChessboardCornersFlags &= ~CV_CALIB_CB_FAST_CHECK;
	}

	if(prop_filterQuads){
		findChessboardCornersFlags |= CV_CALIB_CB_FILTER_QUADS;
	} else {
		findChessboardCornersFlags &= ~CV_CALIB_CB_FILTER_QUADS;
	}

	if(prop_adaptiveThreshold){
		findChessboardCornersFlags |= CV_CALIB_CB_ADAPTIVE_THRESH;
	} else {
		findChessboardCornersFlags &= ~CV_CALIB_CB_ADAPTIVE_THRESH;
	}

	if(prop_normalizeImage){
		findChessboardCornersFlags |= CV_CALIB_CB_NORMALIZE_IMAGE;
	} else {
		findChessboardCornersFlags &= ~CV_CALIB_CB_NORMALIZE_IMAGE;
	}
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
		if(in_img.empty()){
			return;
		}
		Mat image = in_img.read();

		timer.restart();

		cv::resize(image, sub_img, Size(), 1.0 / prop_scale_factor, 1.0 / prop_scale_factor, INTER_NEAREST);

		bool found;


		cv::Size chessboardSize(prop_width, prop_height);

		if (prop_scale) {
			found = findChessboardCorners(sub_img, chessboardSize, corners, findChessboardCornersFlags);
			for (size_t i = 0; i < corners.size(); ++i) {
				corners[i].x *= prop_scale_factor;
				corners[i].y *= prop_scale_factor;
			}
		} else {
			found = findChessboardCorners(image, chessboardSize, corners, findChessboardCornersFlags);
		}

		LOG(LTRACE) << "findChessboardCorners() execution time: " << timer.elapsed() << " s\n";

		if (found) {
			LOG(LTRACE) << "chessboard found\n";

			if (prop_subpix) {
				cornerSubPix(image, corners, Size(prop_subpix_window, prop_subpix_window), Size(1, 1), TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 50, 1e-3));
			}

			chessboard->setImagePoints(corners);
			out_chessboard.write(*chessboard);


			Types::ImagePosition imagePosition;
			double maxPixels = std::max(image.size().width, image.size().height);
			imagePosition.elements[0] = (corners[0].x - image.size().width / 2) / maxPixels;
			imagePosition.elements[1] = (corners[0].y - image.size().height / 2) / maxPixels;
			imagePosition.elements[2] = 0;
			imagePosition.elements[3] = - atan2(corners[1].y - corners[0].y, corners[1].x - corners[0].x);
			out_imagePosition.write(imagePosition);

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
