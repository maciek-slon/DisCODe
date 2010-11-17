/*
 * AudioFile_Source.hpp
 *
 *  Created on: Oct 29, 2010
 *      Author: aszymane
 */

#ifndef AUDIOFILE_SOURCE_HPP_
#define AUDIOFILE_SOURCE_HPP_

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


namespace Sources {
namespace AudioFile_Source {

//#define BUFFER_LEN 308952

/*!
 * \brief AudioFile_Source properties
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

/*!
 * \brief AudioFile_Source data
 */
struct audioFileData
{

	SF_INFO sfinfo;
	cv::Mat dataMat;

};
/*!
 * \class AudioFile_Source_Processor
 * \brief AudioFile_Source processor class.
 */
class AudioFile_Source: public Base::Component
{
public:
	/*!
	 * Constructor.
	 */
	AudioFile_Source(const std::string & name = "");

	/*!
	 * Destructor
	 */
	virtual ~AudioFile_Source();

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


	/// Event signaling that new image was retrieved.
	Base::Event * newData;

	/// Output data stream
//	Base::DataStreamOut<double*> out_data;

	/// Output data stream
	Base::DataStreamOut<SF_INFO> out_info;

	/// Output data stream
	Base::DataStreamOut<cv::Mat> out_data;

//	void SetListenerValues();

	void KillALData();

	/// Threshold properties
	Props props;

	SF_INFO sfinfo;
	SNDFILE* infile;

	int licznik;

	cv::Mat data;

	double *dataRead;
//	double data[BUFFER_LEN];

//	audioFileData dataFromFile;
};

}//: namespace AudioFile_Source
}//: namespace Sinks


/*
 * Register processor component.
 */
REGISTER_SOURCE_COMPONENT("AudioFile", Sources::AudioFile_Source::AudioFile_Source, Common::Panel_Empty)


#endif /* AUDIOFILE_SOURCE_HPP_ */
