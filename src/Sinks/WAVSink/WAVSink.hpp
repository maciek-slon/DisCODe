/*
 * WAVFile_Sink.h
 *
 *  Created on: Nov 8, 2010
 *      Author: aszymane
 */

#ifndef WAVFILE_SINK_H_
#define WAVFILE_SINK_H_

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "Panel_Empty.hpp"
#include "DataStream.hpp"
#include "Props.hpp"

#include <cv.h>
#include <highgui.h>

#include <stdio.h>
#include <sndfile.h>

#include <stdlib.h>


namespace Sinks {
namespace WAVFile_Sink {

/*!
 * \brief WAVFile_Sink properties
 */
struct Props: public Base::Props
{

	std::string filename;

	/*!
	 * \copydoc Base::Props::load
	 */
	void load(const ptree & pt)
	{
		filename = pt.get("filename", "");
	}

	/*!
	 * \copydoc Base::Props::save
	 */
	void save(ptree & pt)
	{
		pt.put("filename", filename);
	}

};

class WAVFile_Sink: public Base::Component
{
public:
	/*!
	 * Constructor.
	 */
	WAVFile_Sink(const std::string & name = "");

	/*!
	 * Destructor
	 */
	virtual ~WAVFile_Sink();

	/*!
	 * Return file properties
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

	/// Event handler.
	Base::EventHandler<WAVFile_Sink> h_onNewData;

	/// Input data stream
	Base::DataStreamIn<cv::Mat> in_data;

	/// File properties
	Props props;

	///  Structure used to store sound file properties
	SF_INFO sfinfo;
	/// Pointer to .wav file
	SNDFILE* outfile;

	/// Array with sound data
	cv::Mat data;

	/// Pointer to sound data to be written to file
	double *dataWrite;
};

}//: namespace WAVFile_Sink
}//: namespace Sinks


/*
 * Register processor component.
 */
REGISTER_SOURCE_COMPONENT("WAVSink", Sinks::WAVFile_Sink::WAVFile_Sink, Common::Panel_Empty)


#endif /* WAVFILE_SINK_H_ */
