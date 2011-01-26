/*
 * WAVFile_Sink.cpp
 *
 *  Created on: Nov 8, 2010
 *      Author: aszymane
 */

#include <memory>
#include <string>

#include "WAVSink.hpp"
#include "Common/Logger.hpp"

namespace Sinks {
namespace WAVFile_Sink {

WAVFile_Sink::WAVFile_Sink(const std::string & name) :
	Base::Component(name) {
	LOG(LTRACE) << "Hello WAVFile_Sink\n";
}

WAVFile_Sink::~WAVFile_Sink() {
	LOG(LTRACE) << "Good bye WAVFile_Sink\n";
}

bool WAVFile_Sink::onInit() {
	LOG(LTRACE) << "WAVFile_Sink::initialize\n";

	h_onNewData.setup(this, &WAVFile_Sink::onNewData);
	registerHandler("onNewData", &h_onNewData);

	registerStream("in_data", &in_data);

	return true;
}

bool WAVFile_Sink::onFinish() {
	LOG(LTRACE) << "WAVFile_Sink::finish\n";

	return true;
}

bool WAVFile_Sink::onStep() {
	LOG(LTRACE) << "WAVFile_Sink::step\n";

	return true;
}

bool WAVFile_Sink::onStop() {
	return true;
}

bool WAVFile_Sink::onStart() {
	return true;
}

void WAVFile_Sink::onNewData() {
	LOG(LTRACE) << "WAVFile_Sink::onNewData\n";

	data = in_data.read().clone();

	sfinfo.frames=(sf_count_t)data.at<double>(0,0);
	sfinfo.samplerate=(int)data.at<double>(0,1);
	sfinfo.channels=(int)data.at<double>(0,2);
	sfinfo.format=(int)data.at<double>(0,3);
	sfinfo.sections=(int)data.at<double>(0,4);
	sfinfo.seekable=(int)data.at<double>(0,5);

	// save number of frames
	sf_count_t frames = sfinfo.frames;

	// open file for write
	if (!(outfile = sf_open(props.filename.c_str(), SFM_WRITE, &sfinfo))) { /* Open failed so print an error message. */
		printf("Not able to open output file %s.\n", props.filename.c_str());
		/* Print the error message from libsndfile. */
		sf_perror(outfile);
		//return  1 ;
	};

	// prepare array with data to write to file
	dataWrite = NULL;
	dataWrite = new double[frames * sfinfo.channels];

	for (int i = 0; i < frames; i++)
		for (int c = 0; c < sfinfo.channels; c++) {
			dataWrite[sfinfo.channels * i + c] = data.at<double> (c+1, i);
		}

	int writecount = sf_write_double(outfile, dataWrite, frames
			* sfinfo.channels);
	delete[] dataWrite;
	dataWrite = NULL;

	// close file
	sfinfo.frames = frames;
	sf_close(outfile);
	sf_open(props.filename.c_str(), SFM_READ, &sfinfo);

	printf("file %s written\n", props.filename.c_str());
	sf_close(outfile);

	onStep();
}

}//: namespace WAVFile_Sink
}//: namespace Sinks
