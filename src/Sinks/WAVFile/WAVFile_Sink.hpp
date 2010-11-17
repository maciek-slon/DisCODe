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

//#define BUFFER_LEN 308952

/*!
 * \brief AudioFile_Source data
 */
struct audioFileData
{

	SF_INFO sfinfo;
	cv::Mat dataMat;

};

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

	/// Event signaling that new image was retrieved.
//	Base::Event * newData;

	/// Event handler.
	Base::EventHandler<WAVFile_Sink> h_onNewData;

	/// Input data stream
	Base::DataStreamIn<cv::Mat> in_data;

	/// Input data stream
	Base::DataStreamIn<SF_INFO> in_info;

	/// Output data stream
//	Base::DataStreamIn<audioFileData> in_data;

	void SetListenerValues();

	void KillALData();

	/// Threshold properties
	Props props;

	SF_INFO sfinfo;
	SNDFILE* outfile;

	int licznik;

	cv::Mat data;

	double *dataWrite;
//	double data[BUFFER_LEN];

//	audioFileData dataToFile;
};

}//: namespace WAVFile_Sink
}//: namespace Sinks


/*
 * Register processor component.
 */
REGISTER_SOURCE_COMPONENT("WAVFile", Sinks::WAVFile_Sink::WAVFile_Sink, Common::Panel_Empty)


#endif /* WAVFILE_SINK_H_ */
