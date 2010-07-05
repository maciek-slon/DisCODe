/*!
 * \file CvThreshold_Processor.hpp
 * \brief
 * \author mstefanc
 * \date 2010-07-05
 */

#ifndef CVTHRESHOLD_PROCESSOR_HPP_
#define CVTHRESHOLD_PROCESSOR_HPP_

#include "Kernel_Aux.hpp"
#include "Kernel.hpp"
#include "Panel_Empty.hpp"
#include "DataStream.hpp"
#include "Props.hpp"

#include <cv.h>
#include <highgui.h>

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
	 * \copydoc Common::Props::load
	 */
	void load(const ptree & pt)
	{
		type = str2type(pt.get("type", "binary"));
		maxval = pt.get("maxval", 1.0);
		thresh = pt.get("thresh", 0.5);
	}

	/*!
	 * \copydoc Common::Props::save
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
class CvThreshold_Processor: public Base::Kernel
{
public:
	/*!
	 * Constructor.
	 */
	CvThreshold_Processor();

	/*!
	 * Destructor
	 */
	virtual ~CvThreshold_Processor();

	/*!
	 * Processor initialization
	 */
	bool initialize();

	/*!
	 * Release all resources
	 */
	bool finish();

	/*!
	 * Processes given frame.
	 */
	int step();

	/*!
	 * Return window properties
	 */
	Base::Props * getProperties()
	{
		return &props;
	}

protected:
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
 * Register processor kernel.
 */
REGISTER_PROCESSOR_KERNEL("CvThreshold", Processors::CvThreshold::CvThreshold_Processor, Common::Panel_Empty)

#endif /* CVTHRESHOLD_PROCESSOR_HPP_ */

