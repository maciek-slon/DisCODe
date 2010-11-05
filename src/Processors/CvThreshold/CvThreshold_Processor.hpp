/*!
 * \file CvThreshold_Processor.hpp
 * \brief
 * \author mstefanc
 * \date 2010-07-05
 */

#ifndef CVTHRESHOLD_PROCESSOR_HPP_
#define CVTHRESHOLD_PROCESSOR_HPP_

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "Panel_Empty.hpp"
#include "DataStream.hpp"
#include "Props.hpp"

#include <cv.h>
#include <highgui.h>

/**
 * \defgroup CvThreshold CvThreshold
 * \ingroup Processors
 *
 * Applies a fixed-level threshold to each array element
 *
 *
 *
 * \par Data streams:
 *
 * \streamin{in_img,cv::Mat}
 * Input image
 * \streamout{out_img_N,cv::Mat}
 * Output images, where N is channel number, counting from 0
 *
 *
 * \par Events:
 *
 * \event{newImage}
 * Image split
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
 * \prop{type,string,"binary"}
 * Thresholding type:
 * - THRESH_BINARY
 * - THRESH_BINARY_INV
 * - THRESH_TRUNC
 * - THRESH_TOZERO
 * - THRESH_TOZERO_INV
 * \prop{maxval,double,1.0}
 * Maximum value to use with THRESH_BINARY and THRESH_BINARY_INV thresholding types
 * \prop{thresh,double,0.5}
 * Threshold value
 *
 * \see http://opencv.willowgarage.com/documentation/cpp/miscellaneous_image_transformations.html#threshold
 * @{
 *
 * @}
 */

namespace Processors {
namespace CvThreshold {

using namespace cv;

/*!
 * \brief CvThreshold properties
 */
struct ThresholdProps: public Base::Props
{

	int type;
	double maxval;
	double thresh;

	/*!
	 * \copydoc Base::Props::load
	 */
	void load(const ptree & pt)
	{
		type = str2type(pt.get("type", "binary"));
		maxval = pt.get("maxval", 1.0);
		thresh = pt.get("thresh", 0.5);
	}

	/*!
	 * \copydoc Base::Props::save
	 */
	void save(ptree & pt)
	{
		pt.put("type", type2str(type));
		pt.put("maxval", maxval);
		pt.put("thresh", thresh);
	}

protected:
	int str2type(const std::string & s)
	{
		if (s == "binary")
			return THRESH_BINARY;
		else if (s == "inv_binary")
			return THRESH_BINARY_INV;
		else if (s == "trunc")
			return THRESH_TRUNC;
		else if (s == "zero")
			return THRESH_TOZERO;
		else if (s == "inv_zero")
			return THRESH_TOZERO_INV;
		else
			return THRESH_BINARY;
	}

	std::string type2str(int t)
	{
		switch (t)
		{
			case THRESH_BINARY:
				return "binary";
			case THRESH_BINARY_INV:
				return "inv_binary";
			case THRESH_TRUNC:
				return "trunc";
			case THRESH_TOZERO:
				return "zero";
			case THRESH_TOZERO_INV:
				return "inv_zero";
			default:
				return "binary";
		}
	}
};

/*!
 * \class CvThreshold_Processor
 * \brief Example processor class.
 */
class CvThreshold_Processor: public Base::Component
{
public:
	/*!
	 * Constructor.
	 */
	CvThreshold_Processor(const std::string & name = "");

	/*!
	 * Destructor
	 */
	virtual ~CvThreshold_Processor();

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
	Base::EventHandler <CvThreshold_Processor> h_onNewImage;

	/// Input data stream
	Base::DataStreamIn <Mat> in_img;

	/// Event raised, when image is processed
	Base::Event * newImage;

	/// Output data stream - processed image
	Base::DataStreamOut <Mat> out_img;

	/// Threshold properties
	ThresholdProps props;
};

}//: namespace CvThreshold
}//: namespace Processors


/*
 * Register processor component.
 */
REGISTER_PROCESSOR_COMPONENT("CvThreshold", Processors::CvThreshold::CvThreshold_Processor, Common::Panel_Empty)

#endif /* CVTHRESHOLD_PROCESSOR_HPP_ */

