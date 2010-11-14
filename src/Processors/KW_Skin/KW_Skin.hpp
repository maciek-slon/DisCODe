/*!
 * \file KW_Skin.hpp
 * \brief
 * \author kwasak
 * \date 2010-11-11
 */

#ifndef KW_SKIN_HPP_
#define KW_SKIN_HPP_

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "Panel_Empty.hpp"
#include "DataStream.hpp"
#include "Props.hpp"

#include <cv.h>
#include <highgui.h>

namespace Processors {
namespace KW_Skin {

using namespace cv;

/*!
 * \brief KW_mean_skin properties
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
 * \class KW_Skin
 * \brief Example processor class.
 */
class KW_Skin: public Base::Component
{
public:
	/*!
	 * Constructor.
	 */
	KW_Skin(const std::string & name = "");

	/*!
	 * Destructor
	 */
	virtual ~KW_Skin();

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
	Base::EventHandler <KW_Skin> h_onNewImage;

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
	int k;
};

}//: namespace KW_Skin
}//: namespace Processors


/*
 * Register processor component.
 */
REGISTER_PROCESSOR_COMPONENT("KW_Skin", Processors::KW_Skin::KW_Skin, Common::Panel_Empty)

#endif /* KW_SKIN_HPP_ */

