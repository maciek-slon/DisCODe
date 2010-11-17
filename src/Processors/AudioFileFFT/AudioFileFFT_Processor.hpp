/*
 * AudioFileFFT.hpp
 *
 *  Created on: Nov 9, 2010
 *      Author: aszymane
 */

#ifndef AUDIOFILEFFT_HPP_
#define AUDIOFILEFFT_HPP_

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "Panel_Empty.hpp"
#include "DataStream.hpp"
#include "Props.hpp"

#include <cv.h>
#include <highgui.h>

#include <sndfile.h>

namespace Processors {
namespace AudioFileFFT {

using namespace cv;

/*!
 * \brief AudioFileFFT properties
 */
struct AudioFileFFT: public Base::Props
{


};

/*!
 * \class AudioFileFFT_Processor
 * \brief Example processor class.
 */
class AudioFileFFT_Processor: public Base::Component
{
public:
	/*!
	 * Constructor.
	 */
	AudioFileFFT_Processor(const std::string & name = "");

	/*!
	 * Destructor
	 */
	virtual ~AudioFileFFT_Processor();

	/*!
	 * Return window properties
	 */
//	Base::Props * getProperties()
//	{
//		return &props;
//	}

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
	void onNewData();

	/// Event handler.
	Base::EventHandler <AudioFileFFT_Processor> h_onNewData;

	/// Input data stream
	Base::DataStreamIn <cv::Mat> in_data;

	/// Event raised, when image is processed
	Base::Event * newData;


	/// Output data stream - processed image
	Base::DataStreamOut <cv::Mat> out_data_r;

	/// Output data stream - processed image
	Base::DataStreamOut <cv::Mat> out_data_l;


	int licznik;


	cv::Mat mat;
	cv::Mat mat_dft;

	cv::Mat mat_l;
	cv::Mat mat_r;

	int data_sample;

	cv::Mat mat_samp_l;
	cv::Mat mat_dft_l;

	cv::Mat mat_samp_r;
	cv::Mat mat_dft_r;

};

}//: namespace AudioFileFFT
}//: namespace Processors


/*
 * Register processor component.
 */
REGISTER_PROCESSOR_COMPONENT("AudioFileFFT", Processors::AudioFileFFT::AudioFileFFT_Processor, Common::Panel_Empty)

#endif /* AUDIOFILEFFT_HPP_ */
