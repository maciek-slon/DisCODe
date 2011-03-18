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

namespace Processors {
namespace AudioFFT {

/*!
 * \brief AudioFFT properties
 */
struct Props: public Base::Props
{

	int sample_time;
	int sample_delay;
	char window_type;

	/*!
	 * \copydoc Base::Props::load
	 */
	void load(const ptree & pt)
	{
		sample_time = pt.get("sample_time", 16);
		sample_delay = pt.get("sample_delay", 10);
		window_type=pt.get("window_type",'h');
	}

	/*!
	 * \copydoc Base::Props::save
	 */
	void save(ptree & pt)
	{
		pt.put("sample_time", sample_time);
		pt.put("sample_delay", sample_delay);
		pt.put("window_type", window_type);
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
	 * Return FFT properties
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

	/*!
	 * Returns Hamming window coefficients for given sample with given window size
	 */
	double HammingW(int t, int s, int sample_size);
	/*!
	 * Returns rectangle window coefficients for given sample with given window size
	 */
	double RectangleW(int t, int s, int sample_size);

	/*!
	 * Returns Fourier coefficients for row of data using given
	 * window size and shift between windows
	 */
	cv::Mat WindowedFFT(cv::Mat data_mat, int sample_size=128, int sample_delay=64);

	/// Event handler.
	Base::EventHandler <AudioFFT_Processor> h_onNewData;

	/// Input data stream
	Base::DataStreamIn <cv::Mat> in_data;

	/// Event raised, when data is processed - left canal
	Base::Event * newDataL;
	/// Event raised, when data is processed - right canal
	Base::Event * newDataR;

	/// Output data stream - processed data - right canal
	Base::DataStreamOut <cv::Mat> out_data_r;
	/// Output data stream - processed data - left canal
	Base::DataStreamOut <cv::Mat> out_data_l;

	/// FFT properties
	Props props;

	/// size of window
	int sample_size;
	/// shift between windows
	int sample_delay;

	/// Array with sound data
	cv::Mat mat;

	/// Array with sound data from left canal
	cv::Mat mat_l;
	/// Array with sound data from right canal
	cv::Mat mat_r;

	/// Array with Fourier coefficients from sound data from left canal
	cv::Mat mat_dft_l;
	/// Array with Fourier coefficients from sound data from right canal
	cv::Mat mat_dft_r;

};

}//: namespace AudioFFT
}//: namespace Processors


/*
 * Register processor component.
 */
REGISTER_PROCESSOR_COMPONENT("AudioFFT", Processors::AudioFFT::AudioFFT_Processor, Common::Panel_Empty)

#endif /* AUDIOFFT_HPP_ */
