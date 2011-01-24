/*
 * Speakers_Sink.hpp
 *
 *  Created on: 29-11-2010
 *      Author: aszymane
 */

#ifndef SPEAKERS_SINK_HPP_
#define SPEAKERS_SINK_HPP_

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "Panel_Empty.hpp"
#include "DataStream.hpp"
#include "Props.hpp"

#include <cv.h>
#include <highgui.h>

#include <stdio.h>
#include <sndfile.h>

#include <portaudio.h>

#include <stdlib.h>

namespace Sinks {
namespace Speakers_Sink {

class Speakers_Sink: public Base::Component {
public:
	/*!
	 * Constructor.
	 */
	Speakers_Sink(const std::string & name = "");

	/*!
	 * Destructor
	 */
	virtual ~Speakers_Sink();

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
	 * Callback function for playing given sound data
	 */
	static int patestCallback(  const void *inputBuffer, void *outputBuffer,
            unsigned long framesPerBuffer,
            const PaStreamCallbackTimeInfo* timeInfo,
            PaStreamCallbackFlags statusFlags,
            void *userData );

	/// Event handler.
	Base::EventHandler<Speakers_Sink> h_onNewData;

	/// Input data stream
	Base::DataStreamIn<cv::Mat> in_data;

	/// Array with sound data
	cv::Mat data;

	double sleep_time;

	PaStream *stream;
	PaError err;

};

}//: namespace Speakers_Sink
}//: namespace Sinks


/*
 * Register processor component.
 */
REGISTER_SOURCE_COMPONENT("Speakers", Sinks::Speakers_Sink::Speakers_Sink, Common::Panel_Empty)

#endif /* SPEAKERS_SINK_HPP_ */
