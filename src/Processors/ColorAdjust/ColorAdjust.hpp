/*!
 * \file CvThreshold_Processor.hpp
 * \brief
 * \author mstefanc
 * \date 2010-07-05
 */

#ifndef COLORADJUST_HPP_
#define COLORADJUST_HPP_

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "Panel_Empty.hpp"
#include "DataStream.hpp"
#include "Property.hpp"

#include "EventHandler2.hpp"

#include <cv.h>
#include <highgui.h>

/**
 * \defgroup ColorAdjust ColorAdjust
 * \ingroup Processors
 *
 */

namespace Processors {
namespace ColorAdjust {

using namespace cv;


/*!
 * \class ColorAdjust
 * \brief
 */
class ColorAdjust: public Base::Component
{
public:
	/*!
	 * Constructor.
	 */
	ColorAdjust(const std::string & name = "");

	/*!
	 * Destructor
	 */
	virtual ~ColorAdjust();

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
	Base::EventHandler <ColorAdjust> h_onNewImage;

	/// Input data stream
	Base::DataStreamIn <Mat> in_img;

	/// Event raised, when image is processed
	Base::Event * newImage;

	/// Output data stream - processed image
	Base::DataStreamOut <Mat> out_img;

	Base::EventHandler2 * h_onSetBlackReference;
	Base::EventHandler2 * h_onSetWhiteReference;

private:

	void onSetBlackReference();

	void onSetWhiteReference();
};

}//: namespace ColorAdjust
}//: namespace Processors


/*
 * Register processor component.
 */
REGISTER_PROCESSOR_COMPONENT("ColorAdjust", Processors::ColorAdjust::ColorAdjust, Common::Panel_Empty)

#endif /* COLORADJUST_HPP_ */

