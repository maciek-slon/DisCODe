/*
 * AudioFile_Source.cpp
 *
 *  Created on: Oct 29, 2010
 *      Author: aszymane
 *
 *      http://www.mega-nerd.com/libsndfile/
 */

#include <memory>
#include <string>

#include "WAVSource.hpp"
#include "Common/Logger.hpp"

#include <cstdlib>

namespace Sources {
namespace AudioFile_Source {

AudioFile_Source::AudioFile_Source(const std::string & name) :
	Base::Component(name) {
	LOG(LTRACE) << "Hello AudioFile_Source\n";
}

AudioFile_Source::~AudioFile_Source() {
	LOG(LTRACE) << "Good bye AudioFile_Source\n";
}

bool AudioFile_Source::onInit() {
	LOG(LTRACE) << "AudioFile_Source::initialize\n";
	newData = registerEvent("newData");

	registerStream("out_info", &out_info);
	registerStream("out_data", &out_data);

	sfinfo.format = 0;
	memset(&sfinfo, 0, sizeof(sfinfo));
	//open file and write info to sfinfo
	if (!(infile = sf_open(props.filename.c_str(), SFM_READ, &sfinfo))) { /* Open failed so print an error message. */
		printf("Not able to open input file %s.\n", props.filename.c_str());
		/* Print the error message from libsndfile. */
		sf_perror(infile);
		return 1;
	};

	// prepare array to read data from file
	dataRead = NULL;
	dataRead = new double[sfinfo.frames * sfinfo.channels];

	licznik = 1;

	return true;
}

bool AudioFile_Source::onFinish() {
	LOG(LTRACE) << "AudioFile_Source::finish\n";

	// close .wav file
	sf_close(infile);

	delete [] dataRead;
	dataRead = NULL;

	return true;
}

bool AudioFile_Source::onStep() {
	LOG(LTRACE) << "AudioFile_Source::step\n";

	if (licznik == 1) {

		// read data from file
		int readcount = sf_read_double(infile, dataRead, sfinfo.frames * sfinfo.channels);

		data.create(sfinfo.channels, sfinfo.frames, CV_64FC1);

		// rewrite data from double* to cv::Mat
		for (int i = 0; i < sfinfo.frames; i++)
			for (int c = 0; c < sfinfo.channels; c++)
				data.at<double> (c, i) = dataRead[sfinfo.channels * i + c];

		licznik++;

		// two outputs
		out_info.write(sfinfo);
		out_data.write(data.clone());

		newData->raise();
	}

	return true;
}

bool AudioFile_Source::onStop() {
	return true;
}

bool AudioFile_Source::onStart() {
	return true;
}

}//: namespace AudioFile_Source
}//: namespace Sources
