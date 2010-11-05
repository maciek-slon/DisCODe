/*!
 * \file Stereo_Processor.hpp
 * \brief
 * \author mstefanc
 * \date 2010-07-05
 */

#ifndef STEREO_PROCESSOR_HPP_
#define STEREO_PROCESSOR_HPP_

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "Panel_Empty.hpp"
#include "DataStream.hpp"
#include "Props.hpp"

#include <Types/stream_OpenCV.hpp>

#include <cv.h>
#include <highgui.h>

#include <sstream>

namespace Processors {
namespace Stereo {

using namespace cv;

/*!
 * \brief Properties
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

protected:

};

/*!
 * \class Stero_Processor
 * \brief Convolves an image with the kernel
 */
class Stero_Processor: public Base::Component
{
public:
	/*!
	 * Constructor.
	 */
	Stero_Processor(const std::string & name = "");

	/*!
	 * Destructor
	 */
	virtual ~Stero_Processor();

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
	void onNewImage_l();
	void onNewImage_r();

	/// Event handler.
	Base::EventHandler <Stero_Processor> h_onNewImage_l;
	Base::EventHandler <Stero_Processor> h_onNewImage_r;

	/// Input data stream
	Base::DataStreamIn <Mat> in_img_r;
	Base::DataStreamIn <Mat> in_img_l;

	/// Event raised, when image is processed
	Base::Event * newImage;

	/// Output data stream - processed image
	Base::DataStreamOut <Mat> out_img;

	/// Threshold properties
	Props props;

private:
	cv::Mat left;
	cv::Mat right;
	cv::Mat depth;

	cv::Mat left_g;
	cv::Mat right_g;

	cv::Mat depth8;

	cv::Mat map11, map12, map21, map22;

	cv::Mat xyz;

	cv::Mat Q;

	cv::StereoSGBM sgbm;
	cv::StereoBM bm;

	bool l,r;
};

}//: namespace Stereo
}//: namespace Processors


/*
 * Register processor component.
 */
REGISTER_PROCESSOR_COMPONENT("Stereo", Processors::Stereo::Stero_Processor, Common::Panel_Empty)

#endif /* STEREO_PROCESSOR_HPP_ */

