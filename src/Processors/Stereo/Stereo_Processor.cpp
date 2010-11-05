/*!
 * \file Stereo_Processor.cpp
 * \brief
 * \author mstefanc
 * \date 2010-07-05
 */

#include <memory>
#include <string>

#include "Stereo_Processor.hpp"
#include "Logger.hpp"

namespace Processors {
namespace Stereo {

Stero_Processor::Stero_Processor(const std::string & name) :
	Base::Component(name)
{
	LOG(LTRACE) << "Hello CvFilter2D_Processor\n";
}

Stero_Processor::~Stero_Processor()
{
	LOG(LTRACE) << "Good bye CvFilter2D_Processor\n";
}

bool Stero_Processor::onInit()
{
	LOG(LTRACE) << "CvFilter2D_Processor::initialize\n";

	l = r = false;

	h_onNewImage_l.setup(this, &Stero_Processor::onNewImage_l);
	h_onNewImage_r.setup(this, &Stero_Processor::onNewImage_r);

	registerHandler("onNewImage_l", &h_onNewImage_l);
	registerHandler("onNewImage_r", &h_onNewImage_r);

	registerStream("in_img_l", &in_img_r);
	registerStream("in_img_r", &in_img_l);

	newImage = registerEvent("newImage");


	registerStream("out_img", &out_img);

	cv::Rect roi1, roi2;

	cv::Size img_size(640, 480);

	// reading intrinsic parameters
	cv::FileStorage fs("intrinsics.yml", CV_STORAGE_READ);
	if (!fs.isOpened()) {
		printf("Failed to open file %s\n", "intrinsics.yml");
		return -1;
	}

	cv::Mat M1, D1, M2, D2;
	fs["M1"] >> M1;
	fs["D1"] >> D1;
	fs["M2"] >> M2;
	fs["D2"] >> D2;

	fs.open("extrinsics.yml", CV_STORAGE_READ);
	if (!fs.isOpened()) {
		printf("Failed to open file %s\n", "extrinsics.yml");
		return -1;
	}

	cv::Mat R, T, R1, P1, R2, P2;
	fs["R"] >> R;
	fs["T"] >> T;

	cv::stereoRectify(M1, D1, M2, D2, img_size, R, T, R1, R2, P1, P2, Q, -1, img_size, &roi1, &roi2);

	cv::initUndistortRectifyMap(M1, D1, R1, P1, img_size, CV_16SC2, map11, map12);
	cv::initUndistortRectifyMap(M2, D2, R2, P2, img_size, CV_16SC2, map21, map22);

	bm.state->roi1 = roi1;
	bm.state->roi2 = roi2;
	bm.state->preFilterCap = 31;
	bm.state->SADWindowSize = 9;
	bm.state->minDisparity = 0;
	bm.state->numberOfDisparities = 64;
	bm.state->textureThreshold = 10;
	bm.state->uniquenessRatio = 15;
	bm.state->speckleWindowSize = 100;
	bm.state->speckleRange = 32;
	bm.state->disp12MaxDiff = 1;

	sgbm.preFilterCap = 63;
	sgbm.SADWindowSize = 3;
	sgbm.P1 = 8 * 1 * sgbm.SADWindowSize * sgbm.SADWindowSize;
	sgbm.P2 = 32 * 1 * sgbm.SADWindowSize * sgbm.SADWindowSize;
	sgbm.minDisparity = 0;
	sgbm.numberOfDisparities = 64;
	sgbm.uniquenessRatio = 10;
	sgbm.speckleWindowSize = 100;
	sgbm.speckleRange = 32;
	sgbm.disp12MaxDiff = 1;
	sgbm.fullDP = true;

	return true;
}

bool Stero_Processor::onFinish()
{
	LOG(LTRACE) << "CvFilter2D_Processor::finish\n";

	return true;
}

bool Stero_Processor::onStep()
{
	LOG(LTRACE) << "CvFilter2D_Processor::step\n";
	return true;
}

bool Stero_Processor::onStop()
{
	return true;
}

bool Stero_Processor::onStart()
{
	return true;
}

void Stero_Processor::onNewImage_l()
{
	LOG(LTRACE) << "CvFilter2D_Processor::onNewImage_l\n";
	try {
		cv::Mat leftr = in_img_l.read();
		cv::remap(leftr, left, map11, map12, cv::INTER_LINEAR);
		l = true;
		//if(r)
		//{
		//	cv::cvtColor(left, left_g, CV_RGB2GRAY);
		//	cv::cvtColor(right, right_g, CV_RGB2GRAY);

		//	sgbm(left_g, right_g, depth);

		//	depth.convertTo(depth8, CV_8U, 255 / (64 * 16.));

		//	out_img.write(depth8);
		//	newImage->raise();
		//l = r = false;
		//}
	} catch (...) {
		LOG(LERROR) << "CvFilter2D_Processor::onNewImage_l failed\n";
	}
}

void Stero_Processor::onNewImage_r()
{
	LOG(LTRACE) << "CvFilter2D_Processor::onNewImage_r\n";
	try {
		cv::Mat rightr = in_img_r.read();
		cv::remap(rightr, right, map21, map22, cv::INTER_LINEAR);
		r = true;

		if(l)
		{
			cv::cvtColor(left, left_g, CV_RGB2GRAY);
			cv::cvtColor(right, right_g, CV_RGB2GRAY);

			sgbm(left_g, right_g, depth);

			depth.convertTo(depth8, CV_8U, 255 / (64 * 16.));

			out_img.write(depth8);
			newImage->raise();
			l = r = false;
		}
	} catch (...) {
		LOG(LERROR) << "CvFilter2D_Processor::onNewImage_r failed\n";
	}
}

}//: namespace CvFilter2D
}//: namespace Processors
