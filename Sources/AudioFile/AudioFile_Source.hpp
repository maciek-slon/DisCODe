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

//#include <conio.h>
#include <stdlib.h>
#include <AL/al.h>
#include <AL/alc.h>
//#include <AL/alu.h>
#include <AL/alut.h>

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


	ALboolean LoadALData();

	void SetListenerValues();

	void KillALData();

	/// Threshold properties
	Props props;

	// Buffers hold sound data.
	ALuint Buffer;

	// Sources are points emitting sound.
	ALuint Source;



	// Position of the source sound.
	ALfloat SourcePos[3];

	// Velocity of the source sound.
	ALfloat SourceVel[3];


	// Position of the listener.
	ALfloat ListenerPos[3];

	// Velocity of the listener.
	ALfloat ListenerVel[3];

	// Orientation of the listener. (first 3 elements are "at", second 3 are "up")
	ALfloat ListenerOri[6];
};

}//: namespace AudioFile_Source
}//: namespace Processors


/*
 * Register processor component.
 */
REGISTER_SOURCE_COMPONENT("AudioFile_Source", Sources::AudioFile_Source::AudioFile_Source, Common::Panel_Empty)


#endif /* AUDIOFILE_SOURCE_HPP_ */
