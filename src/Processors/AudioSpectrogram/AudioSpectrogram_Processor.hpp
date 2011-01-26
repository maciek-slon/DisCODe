/*
 * AudioSpectrogram.h
 *
 *  Created on: Nov 10, 2010
 *      Author: aszymane
 */

#ifndef AUDIOSPECTROGRAM_H_
#define AUDIOSPECTROGRAM_H_

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "Panel_Empty.hpp"
#include "DataStream.hpp"
#include "Props.hpp"

#include <cv.h>
#include <highgui.h>

#include <sndfile.h>

namespace Processors {
namespace AudioSpectrogram {

/*!
 * \class AudioFileFFT_Processor
 * \brief Example processor class.
 */
class AudioSpectrogram_Processor: public Base::Component {
public:
	/*!
	 * Constructor.
	 */
	AudioSpectrogram_Processor(const std::string & name = "");

	/*!
	 * Destructor
	 */
	virtual ~AudioSpectrogram_Processor();

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
	 * Compute spectrogtam of given Fourier coefficients
	 */
	cv::Mat ComputeSpectrogram(cv::Mat mat);

	/*!
	 * Extends output image by n
	 */
	cv::Mat ExtendOutputMatrix(cv::Mat mat, int n);

	/*!
	 * Shortens output image by n
	 */
	cv::Mat ShortenOutputMatrix(cv::Mat mat, int n);

	/// Compute Hamming window coefficients
	double HammingW(int t, int s, int sample_size);

	/// Compute maximum value in matrix
	double Max(cv::Mat mat);
	/// Compute minimum value in matrix
	double Min(cv::Mat mat);

	/// Event handler.
	Base::EventHandler<AudioSpectrogram_Processor> h_onNewData;

	/// Input data stream
	Base::DataStreamIn<cv::Mat> in_data;

	/// Event raised, when image is processed
	Base::Event * newData;

	/// Output data stream - processed image
	Base::DataStreamOut<cv::Mat> out_data;

	int licznik;

	cv::Mat img_out_transp;
	cv::Mat mat_big;
	cv::Mat mat_small;
	cv::Mat mat_out;
	cv::Mat mat_in;
	double max;
	double min;
	cv::Mat img;
	cv::Mat mat_color;
	cv::Mat mat_rgb;
	cv::Mat jedynki;
	cv::Mat jedynki2;
};

}//: namespace AudioSpectrogram
}//: namespace Processors


/*
 * Register processor component.
 */
REGISTER_PROCESSOR_COMPONENT("AudioSpectrogram", Processors::AudioSpectrogram::AudioSpectrogram_Processor, Common::Panel_Empty)

#endif /* AUDIOSPECTROGRAM_H_ */
