/*!
 * \file MS_Sign_LUT.hpp
 * \brief
 * \author mstefanc
 * \date 2010-07-05
 */

#ifndef MS_SIGN_LUT_HPP_
#define MS_SIGN_LUT_HPP_

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "Panel_Empty.hpp"
#include "DataStream.hpp"
#include "Props.hpp"

#include <cv.h>
#include <highgui.h>

namespace Processors {
namespace MS_Sign {

using namespace cv;

/*!
 * \brief MS_Sign_LUT properties
 */
struct Props: public Base::Props
{
	/*!
	 * \copydoc Base::Props::load
	 */
	void load(const ptree & pt)
	{
	}

	/*!
	 * \copydoc Base::Props::save
	 */
	void save(ptree & pt)
	{
	}
};

/*!
 * \class MS_Sign_LUT
 * \brief Example processor class.
 */
class MS_Sign_LUT: public Base::Component
{
public:
	/*!
	 * Constructor.
	 */
	MS_Sign_LUT(const std::string & name = "");

	/*!
	 * Destructor
	 */
	virtual ~MS_Sign_LUT();

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
	Base::EventHandler <MS_Sign_LUT> h_onNewImage;

	/// Input image
	Base::DataStreamIn <Mat> in_img;

	/// Event raised, when image is processed
	Base::Event * newImage;

	/// Output data stream - hue part with continous red
	Base::DataStreamOut <Mat> out_hue;

	/// Output data stream - segments
	Base::DataStreamOut <Mat> out_segments;

	/// Properties
	Props props;

private:
	cv::Mat hue_img;
	cv::Mat segments;
};

}//: namespace MS_Sign
}//: namespace Processors


/*
 * Register processor component.
 */
REGISTER_PROCESSOR_COMPONENT("MS_Sign_LUT", Processors::MS_Sign::MS_Sign_LUT, Common::Panel_Empty)

#endif /* MS_SIGN_LUT_HPP_ */

