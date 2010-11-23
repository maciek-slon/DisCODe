/*!
 * \file KW_mean_skin.cpp
 * \brief
 * \author kwasak
 * \date 2010-11-11
 */

#include <memory>
#include <string>

#include "KW_Skin.hpp"
#include "Logger.hpp"

namespace Processors {
namespace KW_Skin {

// OpenCV writes hue in range 0..180 instead of 0..360
#define H(x) (x>>1)

KW_Skin::KW_Skin(const std::string & name) : Base::Component(name)
{
	LOG(LTRACE) << "Hello KW_Skin\n";
	k = 0;
}

KW_Skin::~KW_Skin()
{
	LOG(LTRACE) << "Good bye KW_Skin\n";
}

bool KW_Skin::onInit()
{
	LOG(LTRACE) << "KW_Skin::initialize\n";

	h_onNewImage.setup(this, &KW_Skin::onNewImage);
	registerHandler("onNewImage", &h_onNewImage);

	registerStream("in_img", &in_img);

	newImage = registerEvent("newImage");

	registerStream("out_hue", &out_hue);
	registerStream("out_saturation", &out_saturation);


	return true;
}

bool KW_Skin::onFinish()
{
	LOG(LTRACE) << "KW_Skin::finish\n";

	return true;
}

bool KW_Skin::onStep()
{
	LOG(LTRACE) << "KW_Skin::step\n";
	return true;
}

bool KW_Skin::onStop()
{
	return true;
}

bool KW_Skin::onStart()
{
	return true;
}

void KW_Skin::onNewImage()
{
	LOG(LTRACE) << "KW_Skin::onNewImage\n";
	try {
		cv::Mat hsv_img = in_img.read();	//czytam obrazem w zejścia

		cv::Size size = hsv_img.size();		//rozmiar obrazka

		hue_img.create(size, CV_8UC1);	//8bitów, 0-255, 1 kanał
		saturation_img.create(size, CV_8UC1);
		

		// Check the arrays for continuity and, if this is the case,
		// treat the arrays as 1D vectors
		if (hsv_img.isContinuous() && segments_img.isContinuous() && value_img.isContinuous() && hue_img.isContinuous() && saturation_img.isContinuous()) {
			size.width *= size.height;
			size.height = 1;
		}
		size.width *= 3;

		for (int i = 0; i < size.height; i++) {
			// when the arrays are continuous,
			// the outer loop is executed only once
			// if not - it's executed for each row

			// get pointer to beggining of i-th row of input hsv image
			const uchar* hsv_p = hsv_img.ptr <uchar> (i);
			// get pointer to beggining of i-th row of output hue image
			uchar* hue_p = hue_img.ptr <uchar> (i);
			// get pointer to beggining of i-th row of output saturation image
			uchar* sat_p = saturation_img.ptr <uchar> (i);

			

			int j;
			for (j = 0; j < size.width; j += 3)
			{
				hue_p[k] = hsv_p[j];
				sat_p[k] = hsv_p[j + 1];
				++k;
			}
		}

		out_hue.write(hue_img);
		out_saturation.write(saturation_img);


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
		LOG(LERROR) << "KW_Skin::onNewImage failed\n";
	}
}

}//: namespace KW_Skin
}//: namespace Processors
