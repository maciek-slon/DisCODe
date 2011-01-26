/*
 * Speakers_Sink.cpp
 *
 *  Created on: 29-11-2010
 *      Author: aszymane
 */

#include <memory>
#include <string>

#include "Speakers_Sink.hpp"
#include "Common/Logger.hpp"

#define NUM_SECONDS   (4)
#define SAMPLE_RATE   (44100)

namespace Sinks {
namespace Speakers_Sink {

Speakers_Sink::Speakers_Sink(const std::string & name) :
	Base::Component(name) {
	LOG(LTRACE) << "Hello Speakers_Sink\n";
}

Speakers_Sink::~Speakers_Sink() {
	LOG(LTRACE) << "Good bye Speakers_Sink\n";
}

bool Speakers_Sink::onInit() {
	LOG(LTRACE) << "Speakers_Sink::initialize\n";

	h_onNewData.setup(this, &Speakers_Sink::onNewData);
	registerHandler("onNewData", &h_onNewData);

	registerStream("in_data", &in_data);

	return true;
}

bool Speakers_Sink::onFinish() {
	LOG(LTRACE) << "Speakers_Sink::finish\n";

	return true;
}

bool Speakers_Sink::onStep() {
	LOG(LTRACE) << "Speakers_Sink::step\n";

	return true;
}

bool Speakers_Sink::onStop() {
	return true;
}

bool Speakers_Sink::onStart() {
	return true;
}

void Speakers_Sink::onNewData() {
	LOG(LTRACE) << "Speakers_Sink::onNewData\n";

		data = in_data.read().clone();

		sfinfo.frames=(sf_count_t)data.at<double>(0,0);
		sfinfo.samplerate=(int)data.at<double>(0,1);
		sfinfo.channels=(int)data.at<double>(0,2);
		sfinfo.format=(int)data.at<double>(0,3);
		sfinfo.sections=(int)data.at<double>(0,4);
		sfinfo.seekable=(int)data.at<double>(0,5);

		sleep_time = (double)sfinfo.frames / (double)sfinfo.samplerate;
		printf("prepare.....\n");

		/* Initialize library before making any other calls. */
		err = Pa_Initialize();
		if (err != paNoError)
			goto error;
		/* Open an audio I/O stream. */
		err = Pa_OpenDefaultStream(&stream, 0, 2,paFloat32, sfinfo.samplerate, 256, &Speakers_Sink::patestCallback, &data);
		if (err != paNoError)
			goto error;

		err = Pa_StartStream(stream);
		if (err != paNoError)
			goto error;
		printf("Now playing\n");
		/* Sleep for several seconds. */
		Pa_Sleep(sleep_time * 1000);

		err = Pa_StopStream(stream);
		if (err != paNoError)
			goto error;
		err = Pa_CloseStream(stream);
		if (err != paNoError)
			goto error;
		Pa_Terminate();
		printf("Played.\n");
		return;
		error: Pa_Terminate();
		fprintf(stderr, "An error occured while using the portaudio stream\n");
		fprintf(stderr, "Error number: %d\n", err);
		fprintf(stderr, "Error message: %s\n", Pa_GetErrorText(err));
		return ;

	onStep();
}


int frame = 0;
int Speakers_Sink::patestCallback(  const void *inputBuffer, void *outputBuffer,
        unsigned long framesPerBuffer,
        const PaStreamCallbackTimeInfo* timeInfo,
        PaStreamCallbackFlags statusFlags,
        void *userData )
{
	cv::Mat data2 = *((cv::Mat*) userData);
	float *out = (float*) outputBuffer;
	unsigned int i;
	(void) inputBuffer; /* Prevent unused variable warning. */

	for (i = 0; i < framesPerBuffer; i++) {
		if (i + frame * framesPerBuffer<data2.cols)
		{
			*out++ = data2.at<double>(1, i + frame * framesPerBuffer);
			*out++ = data2.at<double>(2, i + frame * framesPerBuffer);
		}
		else
		{
			*out++ = 0;
			*out++ = 0;
		}
	}
	++frame;
	return 0;
}

}//: namespace Speakers_Sink
}//: namespace Sinks
