/*!
 * \file MS_Sign_LUT.cpp
 * \brief
 * \author mstefanc
 * \date 2010-07-05
 */

#include <memory>
#include <string>

#include "MS_Sign_LUT.hpp"
#include "Logger.hpp"

namespace Processors {
namespace MS_Sign {

// OpenCV writes hue in range 0..180 instead of 0..360
#define H(x) (x>>1)

MS_Sign_LUT::MS_Sign_LUT(const std::string & name) : Base::Component(name)
{
	LOG(LTRACE) << "Hello MS_Sign_LUT\n";
}

MS_Sign_LUT::~MS_Sign_LUT()
{
	LOG(LTRACE) << "Good bye MS_Sign_LUT\n";
}

bool MS_Sign_LUT::onInit()
{
	LOG(LTRACE) << "MS_Sign_LUT::initialize\n";

	h_onNewImage.setup(this, &MS_Sign_LUT::onNewImage);
	registerHandler("onNewImage", &h_onNewImage);

	registerStream("in_img", &in_img);

	newImage = registerEvent("newImage");

	registerStream("out_hue", &out_hue);
	registerStream("out_segments", &out_segments);

	return true;
}

bool MS_Sign_LUT::onFinish()
{
	LOG(LTRACE) << "MS_Sign_LUT::finish\n";

	return true;
}

bool MS_Sign_LUT::onStep()
{
	LOG(LTRACE) << "MS_Sign_LUT::step\n";
	return true;
}

bool MS_Sign_LUT::onStop()
{
	return true;
}

bool MS_Sign_LUT::onStart()
{
	return true;
}

void MS_Sign_LUT::onNewImage()
{
	LOG(LTRACE) << "MS_Sign_LUT::onNewImage\n";
	try {
		cv::Mat hsv_img = in_img.read();

		cv::Size size = hsv_img.size();

		hue_img.create(size, CV_8UC1);
		segments.create(size, CV_8UC1);

		// Check the arrays for continuity and, if this is the case,
		// treat the arrays as 1D vectors
		if (hsv_img.isContinuous() && segments.isContinuous() && hue_img.isContinuous()) {
			size.width *= size.height;
			size.height = 1;
		}
		size.width *= 3;

		for (int i = 0; i < size.height; i++) {
			// when the arrays are continuous,
			// the outer loop is executed only once
			// if not - it's executed for each row
			const uchar* hsv_p = hsv_img.ptr <uchar> (i);
			uchar* seg_p = segments.ptr <uchar> (i);
			uchar* hue_p = hue_img.ptr <uchar> (i);

			int j, k = 0;
			for (j = 0; j < size.width; j += 3) {
				uchar col = hsv_p[j];
				uchar sat = hsv_p[j + 1];
				uchar val = hsv_p[j + 2];

				// make red part in hue continous
				if (col < H(60))
					hue_p[k] = col + H(360);
				else
					hue_p[k] = col;

				// label colors
				if (col < H(20))
					col = 255; // red
				else if (col < H(180))
					col = 0;
				else if (col < H(220))
					if (sat < 180)
						col = 0;
					else
						col = 128;
				else if (col < H(300))
					col = 128; // blue
				else if (col < H(320))
					col = 0;
				else
					col = 255; //red

				// exclude undersaturated areas (gray levels)
				if (sat < 30)
					col = 0;

				// exclude too dark areas
				if ((val < 100) && (col != 255))
					col = 0;

				// exclude too light areas with low saturation (white)
				if ((val > 240) && (sat < 50))
					col = 0;

				// set pixel color in result image
				seg_p[k] = col;

				++k;
			}
		}

		out_hue.write(hue_img);
		out_segments.write(segments);

		newImage->raise();
	}
	catch (Common::DisCODeException& ex) {
		LOG(LERROR) << ex.what() << "\n";
		ex.printStackTrace();
		exit(EXIT_FAILURE);
	}
	catch (const char * ex) {
		LOG(LERROR) << ex;
	}
	catch (...) {
		LOG(LERROR) << "MS_Sign_LUT::onNewImage failed\n";
	}
}

}//: namespace MS_Sign
}//: namespace Processors
