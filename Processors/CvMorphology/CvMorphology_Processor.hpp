/*!
 * \file CvMorphology_Processor.hpp
 * \brief
 * \author mstefanc
 * \date 2010-07-05
 */

#ifndef CVMORPHOLOGY_PROCESSOR_HPP_
#define CVMORPHOLOGY_PROCESSOR_HPP_

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "Panel_Empty.hpp"
#include "DataStream.hpp"
#include "Props.hpp"

#include <cv.h>
#include <highgui.h>

/**
 * \defgroup CvMorphology CvMorphology
 * \ingroup Processors
 *
 * Performs advanced morphological transformations
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
 * \prop{type,string,"MORPH_OPEN"}
 * Type of morphological operation, one of the following:
 * - \c MORPH_OPEN opening
 * - \c MORPH_CLOSE closing
 * - \c MORPH_GRADIENT morphological gradient
 * - \c MORPH_TOPHAT “top hat”
 * - \c MORPH_BLACKHAT “black hat”
 * \prop{iterations,int,1}
 * Number of times erosion and dilation are applied
 *
 * \see http://opencv.willowgarage.com/documentation/cpp/image_filtering.html#morphologyEx
 * @{
 *
 * @}
 */

namespace Processors {
namespace CvMorphology {

using namespace cv;

/*!
 * \brief CvThreshold properties
 */
struct Props: public Base::Props
{
	int type;
	int iterations;

	/*!
	 * \copydoc Base::Props::load
	 */
	void load(const ptree & pt)
	{
		type = str2type(pt.get("type", "MORPH_OPEN"));
		iterations = pt.get("iterations", 1);
	}

	/*!
	 * \copydoc Base::Props::save
	 */
	void save(ptree & pt)
	{
		pt.put("type", type2str(type));
		pt.put("iterations", iterations);
	}

protected:
	int str2type(const std::string & s) {
		if (s == "MORPH_BLACKHAT") return MORPH_BLACKHAT;
		if (s == "MORPH_CLOSE")	   return MORPH_CLOSE;
		if (s == "MORPH_GRADIENT") return MORPH_GRADIENT;
		if (s == "MORPH_OPEN")     return MORPH_OPEN;
		if (s == "MORPH_TOPHAT")   return MORPH_TOPHAT;
		                           return MORPH_OPEN;
	}

	std::string type2str(int t) {
		switch(t) {
			case MORPH_OPEN:     return "MORPH_OPEN";
			case MORPH_CLOSE:    return "MORPH_CLOSE";
			case MORPH_GRADIENT: return "MORPH_GRADIENT";
			case MORPH_TOPHAT:   return "MORPH_TOPHAT";
			case MORPH_BLACKHAT: return "MORPH_BLACKHAT";
			default:             return "MORPH_OPEN";
		}
	}



};

/*!
 * \class CvMorphology_Processor
 * \brief Example processor class.
 */
class CvMorphology_Processor: public Base::Component
{
public:
	/*!
	 * Constructor.
	 */
	CvMorphology_Processor(const std::string & name = "");

	/*!
	 * Destructor
	 */
	virtual ~CvMorphology_Processor();

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
	Base::EventHandler <CvMorphology_Processor> h_onNewImage;

	/// Input data stream
	Base::DataStreamIn <Mat> in_img;

	/// Event raised, when image is processed
	Base::Event * newImage;

	/// Output data stream - processed image
	Base::DataStreamOut <Mat> out_img;

	/// Threshold properties
	Props props;
};

}//: namespace CvMorphology
}//: namespace Processors


/*
 * Register processor component.
 */
REGISTER_PROCESSOR_COMPONENT("CvMorphology", Processors::CvMorphology::CvMorphology_Processor, Common::Panel_Empty)

#endif /* CVMORPHOLOGY_PROCESSOR_HPP_ */

