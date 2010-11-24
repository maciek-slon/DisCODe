/*
 * AudioFileFFT.hpp
 *
 *  Created on: Nov 9, 2010
 *      Author: aszymane
 */

#ifndef AUDIOFFT_HPP_
#define AUDIOFFT_HPP_

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "Panel_Empty.hpp"
#include "DataStream.hpp"
#include "Props.hpp"

#include <cv.h>
#include <highgui.h>

#include <sndfile.h>

namespace Processors {
namespace AudioFFT {

/*!
 * \brief AudioFFT properties
 */
struct Props: public Base::Props
{

	int sample_size;

	/*!
	 * \copydoc Base::Props::load
	 */
	void load(const ptree & pt)
	{
		sample_size = pt.get("sample_size", 512);
	}

	/*!
	 * \copydoc Base::Props::save
	 */
	void save(ptree & pt)
	{
		pt.put("sample_size", sample_size);
	}

};

/*!
 * \class AudioFFT_Processor
 * \brief Example processor class.
 */
class AudioFFT_Processor: public Base::Component
{
public:
	/*!
	 * Constructor.
	 */
	AudioFFT_Processor(const std::string & name = "");

	/*!
	 * Destructor
	 */
	virtual ~AudioFFT_Processor();

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
	void onNewData();

	cv::Mat WindowedFFT(cv::Mat data_mat, int sample_size=128);
	cv::Mat WindowedFFT2(cv::Mat data_mat, int sample_size=128);

	/// Event handler.
	Base::EventHandler <AudioFFT_Processor> h_onNewData;

	/// Input data stream
	Base::DataStreamIn <cv::Mat> in_data;
	/// Input data stream
	Base::DataStreamIn <SF_INFO> in_info;

	/// Event raised, when image is processed
	Base::Event * newDataL;

	/// Event raised, when image is processed
	Base::Event * newDataR;

	/// Output data stream - processed image
	Base::DataStreamOut <cv::Mat> out_data_r;

	/// Output data stream - processed image
	Base::DataStreamOut <cv::Mat> out_data_l;

	/// Threshold properties
	Props props;

	int licznik;
	SF_INFO sfinfo;

	cv::Mat mat;
	cv::Mat mat_dft;

	cv::Mat mat_l;
	cv::Mat mat_r;

	int sample_size;

	cv::Mat mat_samp_l;
	cv::Mat mat_dft_l;

	cv::Mat mat_samp_r;
	cv::Mat mat_dft_r;

};

}//: namespace AudioFFT
}//: namespace Processors


/*
 * Register processor component.
 */
REGISTER_PROCESSOR_COMPONENT("AudioFFT", Processors::AudioFFT::AudioFFT_Processor, Common::Panel_Empty)

#endif /* AUDIOFFT_HPP_ */
