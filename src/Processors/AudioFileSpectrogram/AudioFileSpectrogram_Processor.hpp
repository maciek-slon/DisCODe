/*
 * AudioFileSpectrogram.h
 *
 *  Created on: Nov 10, 2010
 *      Author: aszymane
 */

#ifndef AUDIOFILESpectrogram_H_
#define AUDIOFILESpectrogram_H_

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "Panel_Empty.hpp"
#include "DataStream.hpp"
#include "Props.hpp"

#include <cv.h>
#include <highgui.h>

#include <sndfile.h>

namespace Processors {
namespace AudioFileSpectrogram {

using namespace cv;

/*!
 * \brief AudioFileFFT properties
 */
struct AudioFileSpectrogram: public Base::Props {

};

/*!
 * \class AudioFileFFT_Processor
 * \brief Example processor class.
 */
class AudioFileSpectrogram_Processor: public Base::Component {
public:
	/*!
	 * Constructor.
	 */
	AudioFileSpectrogram_Processor(const std::string & name = "");

	/*!
	 * Destructor
	 */
	virtual ~AudioFileSpectrogram_Processor();

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
	Base::EventHandler<AudioFileSpectrogram_Processor> h_onNewData;

	/// Input data stream
	Base::DataStreamIn<cv::Mat> in_data;

	/// Event raised, when image is processed
	Base::Event * newData;

	/// Output data stream - processed image
	Base::DataStreamOut<cv::Mat> out_data;

	int licznik;

};

}//: namespace AudioFileSpectrogram
}//: namespace Processors


/*
 * Register processor component.
 */
REGISTER_PROCESSOR_COMPONENT("AudioFileSpectrogram", Processors::AudioFileSpectrogram::AudioFileSpectrogram_Processor, Common::Panel_Empty)

#endif /* AUDIOFILESpectrogram_H_ */
