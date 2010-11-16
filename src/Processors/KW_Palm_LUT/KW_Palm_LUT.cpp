/*!
 * \file KW_Palm_LUT.cpp
 * \brief
 * \author kwasak
 * \date 2010-11-05
 */

#include <memory>
#include <string>

#include "KW_Palm_LUT.hpp"
#include "Logger.hpp"

namespace Processors {
namespace KW_Palm {

// OpenCV writes hue in range 0..180 instead of 0..360
#define H(x) (x>>1)

KW_Palm_LUT::KW_Palm_LUT(const std::string & name) : Base::Component(name)
{
	LOG(LTRACE) << "Hello KW_Palm_LUT\n";
}

KW_Palm_LUT::~KW_Palm_LUT()
{
	LOG(LTRACE) << "Good bye KW_Palm_LUT\n";
}

bool KW_Palm_LUT::onInit()
{
	LOG(LTRACE) << "KW_Palm_LUT::initialize\n";

	h_onNewImage.setup(this, &KW_Palm_LUT::onNewImage);
	registerHandler("onNewImage", &h_onNewImage);

	registerStream("in_img", &in_img);

	newImage = registerEvent("newImage");

	registerStream("out_hue", &out_hue);
	registerStream("out_saturation", &out_saturation);
	registerStream("out_value", &out_value);
	registerStream("out_segments", &out_segments);

	return true;
}

bool KW_Palm_LUT::onFinish()
{
	LOG(LTRACE) << "KW_Palm_LUT::finish\n";

	return true;
}

bool KW_Palm_LUT::onStep()
{
	LOG(LTRACE) << "KW_Palm_LUT::step\n";
	return true;
}

bool KW_Palm_LUT::onStop()
{
	return true;
}

bool KW_Palm_LUT::onStart()
{
	return true;
}

void KW_Palm_LUT::onNewImage()
{
	LOG(LTRACE) << "KW_Palm_LUT::onNewImage\n";
	try {
		cv::Mat hsv_img = in_img.read();	//czytam obrazu w wejścia

		cv::Size size = hsv_img.size();		//rozmiar obrazka
;
		hue_img.create(size, CV_8UC1);	//8bitów, 0-255, 1 kanał
		saturation_img.create(size, CV_8UC1);
		value_img.create(size, CV_8UC1);
		segments_img.create(size, CV_8UC1);

		

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
			// get pointer to beggining of i-th row of output value image
			uchar* val_p = value_img.ptr <uchar> (i);
			// get pointer to beggining of i-th row of output vsegment image
			uchar* seg_p = segments_img.ptr <uchar> (i);

			

			int j, k = 0;
			for (j = 0; j < size.width; j += 3)
			{
				hue_p[k] = hsv_p[j];
				sat_p[k] = hsv_p[j + 1];
				val_p[k] = hsv_p[j + 2];
				//seg_p[k] = hsv_p[j + 2];

				seg_p[k] = 0;
				if ((hue_p[k] > H(0)) && (hue_p[k] < H(50)))
				{
					if((sat_p[k] > 23) && (sat_p[k] < 68))
					{
						seg_p[k] = 255;
					}
				}

				++k;
			}
		}

		out_hue.write(hue_img);
		out_saturation.write(saturation_img);
		out_value.write(value_img);
		out_segments.write(segments_img);

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
		LOG(LERROR) << "KW_Palm_LUT::onNewImage failed\n";
	}
}

}//: namespace KW_Palm
}//: namespace Processors
