/*!
 * \file KW_Palm_LUT.cpp
 * \brief
 * \author kwasak
 * \date 2010-11-05
 */

#include <memory>
#include <string>

#include "KW_Cov.hpp"
#include "Logger.hpp"

namespace Processors {
namespace KW_Cov_ {

// OpenCV writes hue in range 0..180 instead of 0..360
#define H(x) (x>>1)

KW_Cov::KW_Cov(const std::string & name) : Base::Component(name)
{
	LOG(LTRACE) << "Hello KW_Cov\n";
}

KW_Cov::~KW_Cov()
{
	LOG(LTRACE) << "Good bye KW_Cov\n";
}

bool KW_Cov::onInit()
{
	LOG(LTRACE) << "KW_Cov::initialize\n";

	h_onNewImage.setup(this, &KW_Cov::onNewImage);
	registerHandler("onNewImage", &h_onNewImage);

	h_calculate.setup(this, &KW_Cov::calculate);
	registerHandler("calculate", &h_calculate);

	registerStream("in_img", &in_img);

	calculated = false;

	return true;
}

bool KW_Cov::onFinish()
{
	LOG(LTRACE) << "KW_Cov::finish\n";

	return true;
}

bool KW_Cov::onStep()
{
	LOG(LTRACE) << "KW_Cov::step\n";
	return true;
}

bool KW_Cov::onStop()
{
	return true;
}

bool KW_Cov::onStart()
{
	return true;
}

void KW_Cov::onNewImage()
{
	LOG(LTRACE) << "KW_Cov::onNewImage\n";

	try {
		cv::Mat img = in_img.read();
		cv::Size size = img.size();

		// Check the arrays for continuity and, if this is the case,
		// treat the arrays as 1D vectors
		if (img.isContinuous()) {
			size.width *= size.height;
			size.height = 1;
		}
		size.width *= img.channels();

		float val;

		for (int i = 0; i < size.height; i++) {
			// when the arrays are continuous,
			// the outer loop is executed only once
			// if not - it's executed for each row

			// get pointer to beggining of i-th row of input image
			const uchar* c_p = img.ptr <uchar> (i);

			for (int j = 0; j < size.width; j += 3)
			{
				for (int cc = 0; cc < props.channels; ++cc) {

					val = c_p[j+cc];

					if (props.normalize)
						val /= 255;

					c[cc].push_back(val);
				}
			}
		}
	}
	catch (...) {
		LOG(LERROR) << "Cos sie walło.";
	}
}

void KW_Cov::calculate()
{
	if (calculated)
		return;

	LOG(LTRACE) << "KW_Cov::calculate";
	for (int cc = 0; cc < props.channels; ++cc)
		LOG(LTRACE) << c[cc].size();

	cv::Mat samples(props.channels, c[0].size(), CV_32FC1);

	for (int cc = 0; cc < props.channels; ++cc) {
		float* c_p = samples.ptr <float> (cc);
		for (size_t j = 0; j < c[cc].size(); j++)
			c_p[j] = c[cc][j];
	}


	cv::Mat covar, mean;

	cv::calcCovarMatrix(samples, covar, mean, CV_COVAR_NORMAL | CV_COVAR_COLS | CV_COVAR_SCALE);

	for (int i = 0; i < mean.size().height; ++i)
		LOG(LTRACE) << "Mean[" << i << "] = " << mean.at<double>(i, 0);

	for (int i = 0; i < covar.size().height; ++i)
		for (int j = 0; j < covar.size().width; ++j)
			LOG(LTRACE) << "Covar[" << i << "," << j << "] = " << covar.at<double>(i, j);

	calculated = true;
}

}//: namespace KW_Cov_
}//: namespace Processors
