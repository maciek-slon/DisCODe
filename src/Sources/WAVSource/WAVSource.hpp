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
 * \class AudioFile_Source
 * \brief AudioFile source class.
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

	/// Event signaling that new data was retrieved.
	Base::Event * newData;

	/// Output data stream
	Base::DataStreamOut<cv::Mat> out_data;

	/// File properties
	Props props;

	///  Structure used to store sound file properties
	SF_INFO sfinfo;
	/// Pointer to .wav file
	SNDFILE* infile;

	int licznik;

	/// Array with sound data
	cv::Mat data;

	/// Pointer to sound data extracted from file
	double *dataRead;
};

}//: namespace AudioFile_Source
}//: namespace Sources


/*
 * Register processor component.
 */
REGISTER_SOURCE_COMPONENT("WAVSource", Sources::AudioFile_Source::AudioFile_Source, Common::Panel_Empty)


#endif /* AUDIOFILE_SOURCE_HPP_ */
