/*!
 * \file KW_Palm_LUT.hpp
 * \brief
 * \author kwasak
 * \date 2010-11-05
 */

#ifndef KW_PALM_LUT_HPP_
#define KW_PALM_LUT_HPP_

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "Panel_Empty.hpp"
#include "DataStream.hpp"
#include "Props.hpp"

#include <cv.h>
#include <highgui.h>

namespace Processors {
namespace KW_Palm {

using namespace cv;

/*!
 * \brief KW_PALM_LUT properties
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
 * \class KW_Palm_LUT
 * \brief Example processor class.
 */
class KW_Palm_LUT: public Base::Component
{
public:
	/*!
	 * Constructor.
	 */
	KW_Palm_LUT(const std::string & name = "");

	/*!
	 * Destructor
	 */
	virtual ~KW_Palm_LUT();

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
	Base::EventHandler <KW_Palm_LUT> h_onNewImage;

	/// Input image
	Base::DataStreamIn <Mat> in_img;

	/// Event raised, when image is processed
	Base::Event * newImage;

	/// Output data stream - hue part with continous red
	Base::DataStreamOut <Mat> out_hue;

	/// Output data stream - saturation
	Base::DataStreamOut <Mat> out_saturation;

	/// Output data stream - value
	Base::DataStreamOut <Mat> out_value;	

	/// Output data stream - segments
	Base::DataStreamOut <Mat> out_segments;

	/// Properties
	Props props;

private:
	cv::Mat hue_img;
	cv::Mat saturation_img;
	cv::Mat value_img;
	cv::Mat segments_img;
};

}//: namespace KW_Palm
}//: namespace Processors


/*
 * Register processor component.
 */
REGISTER_PROCESSOR_COMPONENT("KW_Palm_LUT", Processors::KW_Palm::KW_Palm_LUT, Common::Panel_Empty)

#endif /* KW_PALM_LUT_HPP_ */

