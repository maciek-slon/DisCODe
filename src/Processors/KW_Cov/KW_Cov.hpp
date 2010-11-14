/*!
 * \file KW_Palm_LUT.hpp
 * \brief
 * \author kwasak
 * \date 2010-11-05
 */

#ifndef KW_COV_HPP_
#define KW_COV_HPP_

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "Panel_Empty.hpp"
#include "DataStream.hpp"
#include "Props.hpp"

#include <cv.h>
#include <highgui.h>

namespace Processors {
namespace KW_Cov_ {

using namespace cv;

/*!
 * \brief KW_Cov properties
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
 * \class KW_Cov
 * \brief Example processor class.
 */
class KW_Cov: public Base::Component
{
public:
	/*!
	 * Constructor.
	 */
	KW_Cov(const std::string & name = "");

	/*!
	 * Destructor
	 */
	virtual ~KW_Cov();

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
	void onNewImage1();
	void onNewImage2();

	/// Event handler.
	/// New image is waiting
	Base::EventHandler <KW_Cov> h_onNewImage1;
	Base::EventHandler <KW_Cov> h_onNewImage2;

	/// Input image
	//	Base::DataStreamIn <Mat> in_img;

	/// Input images
	Base::DataStreamIn < cv::Mat, Base::DataStreamBuffer::Newest > in_img1;
	Base::DataStreamIn < cv::Mat, Base::DataStreamBuffer::Newest > in_img2;


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

}//: namespace KW_Cov_
}//: namespace Processors


/*
 * Register processor component.
 */
REGISTER_PROCESSOR_COMPONENT("KW_Cov", Processors::KW_Cov_::KW_Cov, Common::Panel_Empty)

#endif /* KW_COV_HPP_ */

