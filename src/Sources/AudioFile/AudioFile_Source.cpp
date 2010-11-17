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

#include "AudioFile_Source.hpp"
#include "Common/Logger.hpp"

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
	if (!(infile = sf_open(props.filename.c_str(), SFM_READ, &sfinfo))) { /* Open failed so print an error message. */
		printf("Not able to open input file %s.\n", props.filename.c_str());
		/* Print the error message from libsndfile. */
		sf_perror(infile);
		return 1;
	};

	dataRead = NULL;
	dataRead = new double[sfinfo.frames * sfinfo.channels];

	licznik = 1;

	return true;
}

bool AudioFile_Source::onFinish() {
	LOG(LTRACE) << "AudioFile_Source::finish\n";

	sf_close(infile);

	delete [] dataRead;
	dataRead = NULL;

	return true;
}

bool AudioFile_Source::onStep() {
	LOG(LTRACE) << "AudioFile_Source::step\n";

//	printf("step source1 %d\n",licznik);

	if (licznik == 1) {
/*		printf("in%d frames\n", (int) sfinfo.frames);
		printf("in%d samplerate\n", sfinfo.samplerate);
		printf("in%d channels\n", sfinfo.channels);
		printf("in%d format\n", sfinfo.format);
		printf("in%d sections\n", sfinfo.sections);
		printf("in%d seekable\n", sfinfo.seekable);
*/

//		dataRead = NULL;
//		dataRead = new double[sfinfo.frames * sfinfo.channels];

		int readcount = sf_read_double(infile, dataRead, sfinfo.frames * sfinfo.channels);

		data.create(sfinfo.channels, sfinfo.frames, CV_64FC1);

		for (int c = 0; c < sfinfo.channels; c++)
			for (int i = 0; i < sfinfo.frames; i++)
				data.at<double> (c, i) = dataRead[c * i + c];

/*
		printf("dane we:\n");
		for (int i = 0; i < 20; i++)
			printf("%f\n", data.at<double> (0, i));

		printf("data size %d\n", sizeof(data) / sizeof(double));
		printf("readcount %d<-------------------\n", readcount);
*/
		licznik++;

//		delete [] dataRead;
//		dataRead = NULL;


//		printf("dane wy:\n");
//		for (int i = 0; i < 10; i++)
//			printf("%f\n", data.at<double> (0, i));

		out_info.write(sfinfo);
		out_data.write(data.clone());
//		printf("do wyslania s\n");

		newData->raise();
//		printf("wyslane s\n");
	}
//	printf("step source2 %d\n",licznik);

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
