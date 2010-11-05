/*!
 * \file CvColorConv_Processor.hpp
 * \brief
 * \author mstefanc
 * \date 2010-07-05
 */

#ifndef CVCOLORCONV_PROCESSOR_HPP_
#define CVCOLORCONV_PROCESSOR_HPP_

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "Panel_Empty.hpp"
#include "DataStream.hpp"
#include "Props.hpp"

#include <cv.h>
#include <highgui.h>

/**
 * \defgroup CvColorConv CvColorConv
 * \ingroup Processors
 *
 * Converts image from one color space to another
 *
 *
 *
 * \par Data streams:
 *
 * \streamin{in_img,cv::Mat}
 * Input image
 * \streamout{out_img,cv::Mat}
 * Output image
 *
 *
 * \par Events:
 *
 * \event{newImage}
 * New image is ready
 *
 *
 * \par Event handlers:
 *
 * \handler{onNewImage}
 * New image arrived
 *
 *
 * \par Properties:
 *
 * \prop{type,string,"BGR2GRAY"}
 * The color space conversion code, some available values are:
 * - BGR2BGRA, RGB2RGBA, BGRA2BGR, RGBA2RGB, BGR2RGBA, RGB2BGRA, RGBA2BGR, BGRA2RGB, BGR2RGB, RGB2BGR, BGRA2RGBA, RGBA2BGRA
 * - BGR2GRAY, RGB2GRAY, GRAY2BGR, GRAY2RGB, GRAY2BGRA, GRAY2RGBA, BGRA2GRAY, RGBA2GRAY
 * - BGR2BGR565, RGB2BGR565, BGR5652BGR, BGR5652RGB, BGRA2BGR565, RGBA2BGR565, BGR5652BGRA, BGR5652RGBA, GRAY2BGR565, BGR5652GRAY
 * - BGR2BGR555, RGB2BGR555, BGR5552BGR, BGR5552RGB, BGRA2BGR555,RGBA2BGR555, BGR5552BGRA, BGR5552RGBA, GRAY2BGR555, BGR5552RGBA
 * - BGR2XYZ, RGB2XYZ, XYZ2BGR, XYZ2RGB
 * - BGR2YCrCb, RGB2YCrCb, YCrCb2BGR, YCrCb2RGB
 * - BGR2HSV, RGB2HSV, HSV2BGR, HSV2RGB
 * - BGR2Lab, RGB2Lab, Lab2BGR, Lab2RGB
 * - BayerBG2BGR, BayerGB2BGR, BayerRG2BGR, BayerGR2BGR
 * - BayerBG2RGB, BayerGB2RGB, BayerRG2RGB, BayerGR2RGB
 * - BGR2Luv, RGB2Luv, Luv2BGR, Luv2RGB
 * - BGR2HLS, RGB2HLS, HLS2BGR, HLS2RGB
 *
 *
 * \see http://opencv.willowgarage.com/documentation/cpp/miscellaneous_image_transformations.html#cvtColor
 * @{
 *
 * @}
 */

namespace Processors {
namespace CvColorConv {

using namespace cv;

/*!
 * \brief CvColorConv properties
 */
struct ColorConvProps: public Base::Props
{

	int type;

	/*!
	 * \copydoc Base::Props::load
	 */
	void load(const ptree & pt)
	{
		type = str2type(pt.get("type", "BGR2GRAY"));
	}

	/*!
	 * \copydoc Base::Props::save
	 */
	void save(ptree & pt)
	{
		pt.put("type", type2str(type));
	}

protected:
	int str2type(const std::string & s)
	{
		if (s=="BGR2BGRA") return CV_BGR2BGRA;
		if (s=="RGB2RGBA") return CV_RGB2RGBA;

		if (s=="BGRA2BGR") return CV_BGRA2BGR;
		if (s=="RGBA2RGB") return CV_RGBA2RGB;

		if (s=="BGR2RGBA") return CV_BGR2RGBA;
		if (s=="RGB2BGRA") return CV_RGB2BGRA;

		if (s=="RGBA2BGR") return CV_RGBA2BGR;
		if (s=="BGRA2RGB") return CV_BGRA2RGB;

		if (s=="BGR2RGB") return CV_BGR2RGB;
		if (s=="RGB2BGR") return CV_RGB2BGR;

		if (s=="BGRA2RGBA") return CV_BGRA2RGBA;
		if (s=="RGBA2BGRA") return CV_RGBA2BGRA;

		if (s=="BGR2GRAY") return CV_BGR2GRAY;
		if (s=="RGB2GRAY") return CV_RGB2GRAY;
		if (s=="GRAY2BGR") return CV_GRAY2BGR;
		if (s=="GRAY2RGB") return CV_GRAY2RGB;
		if (s=="GRAY2BGRA") return CV_GRAY2BGRA;
		if (s=="GRAY2RGBA") return CV_GRAY2RGBA;
		if (s=="BGRA2GRAY") return CV_BGRA2GRAY;
		if (s=="RGBA2GRAY") return CV_RGBA2GRAY;

		if (s=="BGR2BGR565") return CV_BGR2BGR565;
		if (s=="RGB2BGR565") return CV_RGB2BGR565;
		if (s=="BGR5652BGR") return CV_BGR5652BGR;
		if (s=="BGR5652RGB") return CV_BGR5652RGB;
		if (s=="BGRA2BGR565") return CV_BGRA2BGR565;
		if (s=="RGBA2BGR565") return CV_RGBA2BGR565;
		if (s=="BGR5652BGRA") return CV_BGR5652BGRA;
		if (s=="BGR5652RGBA") return CV_BGR5652RGBA;

		if (s=="GRAY2BGR565") return CV_GRAY2BGR565;
		if (s=="BGR5652GRAY") return CV_BGR5652GRAY;

		if (s=="BGR2BGR555") return CV_BGR2BGR555;
		if (s=="RGB2BGR555") return CV_RGB2BGR555;
		if (s=="BGR5552BGR") return CV_BGR5552BGR;
		if (s=="BGR5552RGB") return CV_BGR5552RGB;
		if (s=="BGRA2BGR555") return CV_BGRA2BGR555;
		if (s=="RGBA2BGR555") return CV_RGBA2BGR555;
		if (s=="BGR5552BGRA") return CV_BGR5552BGRA;
		if (s=="BGR5552RGBA") return CV_BGR5552RGBA;

		if (s=="GRAY2BGR555") return CV_GRAY2BGR555;
		if (s=="BGR5552GRAY") return CV_BGR5552GRAY;

		if (s=="BGR2XYZ") return CV_BGR2XYZ;
		if (s=="RGB2XYZ") return CV_RGB2XYZ;
		if (s=="XYZ2BGR") return CV_XYZ2BGR;
		if (s=="XYZ2RGB") return CV_XYZ2RGB;

		if (s=="BGR2YCrCb") return CV_BGR2YCrCb;
		if (s=="RGB2YCrCb") return CV_RGB2YCrCb;
		if (s=="YCrCb2BGR") return CV_YCrCb2BGR;
		if (s=="YCrCb2RGB") return CV_YCrCb2RGB;

		if (s=="BGR2HSV") return CV_BGR2HSV;
		if (s=="RGB2HSV") return CV_RGB2HSV;

		if (s=="BGR2Lab") return CV_BGR2Lab;
		if (s=="RGB2Lab") return CV_RGB2Lab;

		if (s=="BayerBG2BGR") return CV_BayerBG2BGR;
		if (s=="BayerGB2BGR") return CV_BayerGB2BGR;
		if (s=="BayerRG2BGR") return CV_BayerRG2BGR;
		if (s=="BayerGR2BGR") return CV_BayerGR2BGR;

		if (s=="BayerBG2RGB") return CV_BayerBG2RGB;
		if (s=="BayerGB2RGB") return CV_BayerGB2RGB;
		if (s=="BayerRG2RGB") return CV_BayerRG2RGB;
		if (s=="BayerGR2RGB") return CV_BayerGR2RGB;

		if (s=="BGR2Luv") return CV_BGR2Luv;
		if (s=="RGB2Luv") return CV_RGB2Luv;
		if (s=="BGR2HLS") return CV_BGR2HLS;
		if (s=="RGB2HLS") return CV_RGB2HLS;

		if (s=="HSV2BGR") return CV_HSV2BGR;
		if (s=="HSV2RGB") return CV_HSV2RGB;

		if (s=="Lab2BGR") return CV_Lab2BGR;
		if (s=="Lab2RGB") return CV_Lab2RGB;
		if (s=="Luv2BGR") return CV_Luv2BGR;
		if (s=="Luv2RGB") return CV_Luv2RGB;
		if (s=="HLS2BGR") return CV_HLS2BGR;
		if (s=="HLS2RGB") return CV_HLS2RGB;

		return CV_BGR2GRAY;
	}

	std::string type2str(int t)
	{
		switch (t)
		{
			case CV_BGR2BGRA: return "BGR2BGRA";
			//case CV_RGB2RGBA: return "RGB2RGBA"; // <- same define

			case CV_BGRA2BGR: return "BGRA2BGR";
			//case CV_RGBA2RGB: return "RGBA2RGB";

			case CV_BGR2RGBA: return "BGR2RGBA";
			//case CV_RGB2BGRA: return "RGB2BGRA";

			case CV_RGBA2BGR: return "RGBA2BGR";
			//case CV_BGRA2RGB: return "BGRA2RGB";

			case CV_BGR2RGB: return "BGR2RGB";
			//case CV_RGB2BGR: return "RGB2BGR";

			case CV_BGRA2RGBA: return "BGRA2RGBA";
			//case CV_RGBA2BGRA: return "RGBA2BGRA";

			case CV_BGR2GRAY: return "BGR2GRAY";
			case CV_RGB2GRAY: return "RGB2GRAY";
			case CV_GRAY2BGR: return "GRAY2BGR";
			//case CV_GRAY2RGB: return "GRAY2RGB";
			case CV_GRAY2BGRA: return "GRAY2BGRA";
			//case CV_GRAY2RGBA: return "GRAY2RGBA";
			case CV_BGRA2GRAY: return "BGRA2GRAY";
			case CV_RGBA2GRAY: return "RGBA2GRAY";

			case CV_BGR2BGR565: return "BGR2BGR565";
			case CV_RGB2BGR565: return "RGB2BGR565";
			case CV_BGR5652BGR: return "BGR5652BGR";
			case CV_BGR5652RGB: return "BGR5652RGB";
			case CV_BGRA2BGR565: return "BGRA2BGR565";
			case CV_RGBA2BGR565: return "RGBA2BGR565";
			case CV_BGR5652BGRA: return "BGR5652BGRA";
			case CV_BGR5652RGBA: return "BGR5652RGBA";

			case CV_GRAY2BGR565: return "GRAY2BGR565";
			case CV_BGR5652GRAY: return "BGR5652GRAY";

			case CV_BGR2BGR555: return "BGR2BGR555";
			case CV_RGB2BGR555: return "RGB2BGR555";
			case CV_BGR5552BGR: return "BGR5552BGR";
			case CV_BGR5552RGB: return "BGR5552RGB";
			case CV_BGRA2BGR555: return "BGRA2BGR555";
			case CV_RGBA2BGR555: return "RGBA2BGR555";
			case CV_BGR5552BGRA: return "BGR5552BGRA";
			case CV_BGR5552RGBA: return "BGR5552RGBA";

			case CV_GRAY2BGR555: return "GRAY2BGR555";
			case CV_BGR5552GRAY: return "BGR5552GRAY";

			case CV_BGR2XYZ: return "BGR2XYZ";
			case CV_RGB2XYZ: return "RGB2XYZ";
			case CV_XYZ2BGR: return "XYZ2BGR";
			case CV_XYZ2RGB: return "XYZ2RGB";

			case CV_BGR2YCrCb: return "BGR2YCrCb";
			case CV_RGB2YCrCb: return "RGB2YCrCb";
			case CV_YCrCb2BGR: return "YCrCb2BGR";
			case CV_YCrCb2RGB: return "YCrCb2RGB";

			case CV_BGR2HSV: return "BGR2HSV";
			case CV_RGB2HSV: return "RGB2HSV";

			case CV_BGR2Lab: return "BGR2Lab";
			case CV_RGB2Lab: return "RGB2Lab";

			case CV_BayerBG2BGR: return "BayerBG2BGR";
			case CV_BayerGB2BGR: return "BayerGB2BGR";
			case CV_BayerRG2BGR: return "BayerRG2BGR";
			case CV_BayerGR2BGR: return "BayerGR2BGR";

			//case CV_BayerBG2RGB: return "BayerBG2RGB";
			//case CV_BayerGB2RGB: return "BayerGB2RGB";
			//case CV_BayerRG2RGB: return "BayerRG2RGB";
			//case CV_BayerGR2RGB: return "BayerGR2RGB";

			case CV_BGR2Luv: return "BGR2Luv";
			case CV_RGB2Luv: return "RGB2Luv";
			case CV_BGR2HLS: return "BGR2HLS";
			case CV_RGB2HLS: return "RGB2HLS";

			case CV_HSV2BGR: return "HSV2BGR";
			case CV_HSV2RGB: return "HSV2RGB";

			case CV_Lab2BGR: return "Lab2BGR";
			case CV_Lab2RGB: return "Lab2RGB";
			case CV_Luv2BGR: return "Luv2BGR";
			case CV_Luv2RGB: return "Luv2RGB";
			case CV_HLS2BGR: return "HLS2BGR";
			case CV_HLS2RGB: return "HLS2RGB";

			default: return "RGB2GRAY";
		}
	}
};

/*!
 * \class CvColorConv_Processor
 * \brief Example processor class.
 */
class CvColorConv_Processor: public Base::Component
{
public:
	/*!
	 * Constructor.
	 */
	CvColorConv_Processor(const std::string & name = "");

	/*!
	 * Destructor
	 */
	virtual ~CvColorConv_Processor();

	/*!
	 * Return window properties
	 */
	Base::Props * getProperties()
	{
		return &props;
	}

protected:

	/*!
	 * Connects source to given device.
	 */
	bool onInit();

	/*!
	 * Disconnect source from device, closes streams, etc.
	 */
	bool onFinish();

	/*!
	 * Retrieves data from device.
	 */
	bool onStep();

	/*!
	 * Start component
	 */
	bool onStart();

	/*!
	 * Stop component
	 */
	bool onStop();

	/*!
	 * Event handler function.
	 */
	void onNewImage();

	/// Event handler.
	Base::EventHandler <CvColorConv_Processor> h_onNewImage;

	/// Input data stream
	Base::DataStreamIn <Mat> in_img;

	/// Event raised, when image is processed
	Base::Event * newImage;

	/// Output data stream - processed image
	Base::DataStreamOut <Mat> out_img;

	/// Threshold properties
	ColorConvProps props;

private:
	cv::Mat img;
	cv::Mat out;
};

}//: namespace CvColorConv
}//: namespace Processors


/*
 * Register processor component.
 */
REGISTER_PROCESSOR_COMPONENT("CvColorConv", Processors::CvColorConv::CvColorConv_Processor, Common::Panel_Empty)

#endif /* CVCOLORCONV_PROCESSOR_HPP_ */

