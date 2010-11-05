/*
 * AudioFile_Source.cpp
 *
 *  Created on: Oct 29, 2010
 *      Author: aszymane
 */

#include <memory>
#include <string>

#include "AudioFile_Source.hpp"
#include "Common/Logger.hpp"

namespace Sources {
namespace AudioFile_Source {



AudioFile_Source::AudioFile_Source(const std::string & name) : Base::Component(name)
{
	LOG(TRACE) << "Hello AudioFile_Source\n";
}

AudioFile_Source::~AudioFile_Source()
{
	LOG(TRACE) << "Good bye AudioFile_Source\n";
}

bool AudioFile_Source::onInit()
{
	LOG(TRACE) << "AudioFile_Source::initialize\n";

	// Register data streams, events and event handlers HERE!

	// Position of the source sound.
	SourcePos[0] = 0.0;
	SourcePos[1] = 0.0;
	SourcePos[2] = 0.0;

	// Velocity of the source sound.
	SourceVel[0] = 0.0;
	SourceVel[1] = 0.0;
	SourceVel[2] = 0.0;


	// Position of the listener.
	ListenerPos[0] = 0.0;
	ListenerPos[1] = 0.0;
	ListenerPos[2] = 0.0;

	// Velocity of the listener.
	ListenerPos[0] = 0.0;
	ListenerPos[1] = 0.0;
	ListenerPos[2] = 0.0;

	// Orientation of the listener. (first 3 elements are "at", second 3 are "up")
	ListenerOri[0] = 0.0;
	ListenerOri[1] = 0.0;
	ListenerOri[2] = -1.0;
	ListenerOri[3] = 0.0;
	ListenerOri[4] = 1.0;
	ListenerOri[5] = 0.0;

	// Initialize OpenAL and clear the error bit.

	alutInitWithoutContext(NULL, NULL);
	alGetError();

	// Load the wav data.
	if (LoadALData() == AL_FALSE)
	    return -1;

    SetListenerValues();

	return true;
}

bool AudioFile_Source::onFinish()
{
	LOG(TRACE) << "AudioFile_Source::finish\n";

    alDeleteBuffers(1, &Buffer);
    alDeleteSources(1, &Source);
    alutExit();

	return true;
}

bool AudioFile_Source::onStep()
{
	LOG(TRACE) << "AudioFile_Source::step\n";

    ALubyte c = ' ';

    alSourcePlay(Source);
    /*
    while (c != 'q')
    {
        c = getchar();

        switch (c)
        {
            // Pressing 'p' will begin playing the sample.
            case 'p': alSourcePlay(Source); break;

            // Pressing 's' will stop the sample from playing.
            case 's': alSourceStop(Source); break;

            // Pressing 'h' will pause (hold) the sample.
            case 'h': alSourcePause(Source); break;
        };
    }
*/

	return true;
}

bool AudioFile_Source::onStop()
{
	return true;
}

bool AudioFile_Source::onStart()
{
	return true;
}


ALboolean AudioFile_Source::LoadALData()
{
    // Variables to load into.

    ALenum format;
    ALsizei size;
    ALvoid* data;
    ALsizei freq;
    ALboolean loop;

    // Load wav data into a buffer.
    alGenBuffers(1, &Buffer);
    if (alGetError() != AL_NO_ERROR)
        return AL_FALSE;

    Buffer=alutCreateBufferFromFile(props.filename.c_str());
//    alutLoadWAVFile(const_cast<ALbyte*>(reinterpret_cast<const ALbyte*>(props.filename.c_str())), &format, &data, &size, &freq, &loop);
//    alBufferData(Buffer, format, data, size, freq);
//    alutUnloadWAV(format, data, size, freq);

//    alutSaveWAVFile("wavdata/FancyPants2.wav", &format, &data, &size, &freq, &loop);

    // Bind buffer with a source.
    alGenSources(1, &Source);

    if (alGetError() != AL_NO_ERROR)
        return AL_FALSE;

    alSourcei (Source, AL_BUFFER,   Buffer   );
    alSourcef (Source, AL_PITCH,    1.0f     );
    alSourcef (Source, AL_GAIN,     1.0f     );
    alSourcefv(Source, AL_POSITION, SourcePos);
    alSourcefv(Source, AL_VELOCITY, SourceVel);
    alSourcei (Source, AL_LOOPING,  loop     );

    // Do another error check and return.
    if (alGetError() == AL_NO_ERROR)
        return AL_TRUE;

    return AL_FALSE;
}

void AudioFile_Source::SetListenerValues()
{
    alListenerfv(AL_POSITION,    ListenerPos);
    alListenerfv(AL_VELOCITY,    ListenerVel);
    alListenerfv(AL_ORIENTATION, ListenerOri);
}


void AudioFile_Source::KillALData()
{
    alDeleteBuffers(1, &Buffer);
    alDeleteSources(1, &Source);
    alutExit();
}


}//: namespace AudioFile_Source
}//: namespace Sources
