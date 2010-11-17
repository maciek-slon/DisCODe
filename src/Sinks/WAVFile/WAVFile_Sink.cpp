/*
 * WAVFile_Sink.cpp
 *
 *  Created on: Nov 8, 2010
 *      Author: aszymane
 */

#include <memory>
#include <string>

#include "WAVFile_Sink.hpp"
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


	registerStream("in_info", &in_info);
	registerStream("in_data", &in_data);

	licznik = 1;

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

//	if (licznik == 1) {
		data = in_data.read().clone();
		sfinfo = in_info.read();

//		data = dataToFile.dataMat.clone();
		//		for(int i=0;i<BUFFER_LEN;i++)
		//			data[i] = dataToFile.data[i];
/*
		printf("dane we:\n");
		for (int i = 0; i < 20; i++)
			printf("%f\n", dataToFile.dataMat.at<double> (0, i));

		printf("out%d frames\n", (int) sfinfo.frames);
		printf("out%d samplerate\n", sfinfo.samplerate);
		printf("out%d channels\n", sfinfo.channels);
		printf("out%d format\n", sfinfo.format);
		printf("out%d sections\n", sfinfo.sections);
		printf("out%d seekable\n", sfinfo.seekable);
//		sf_count_t frames = sfinfo.frames;
*/
		//		int writecount = sf_write_double (outfile, data, sfinfo.frames*sfinfo.channels);

		licznik++;
		if (!(outfile = sf_open(props.filename.c_str(), SFM_WRITE, &sfinfo))) { /* Open failed so print an error message. */
			printf("Not able to open output file %s.\n", props.filename.c_str());
			/* Print the error message from libsndfile. */
			sf_perror(outfile);
			//return  1 ;
		};

		dataWrite = NULL;
		dataWrite = new double[sfinfo.frames * sfinfo.channels];
		//		dataWrite=(double*)malloc(sizeof(sizeof(double)*sfinfo.frames*sfinfo.channels));

		for (int i = 0; i < sfinfo.frames; i++)
			for (int c = 0; c < sfinfo.channels; c++)
				dataWrite[c * i + c] = data.at<double> (c, i);

		int writecount = sf_write_double(outfile, dataWrite, sfinfo.frames * sfinfo.channels);

		delete [] dataWrite;
		dataWrite = NULL;

		sf_close(outfile);
		sf_open(props.filename.c_str(), SFM_READ, &sfinfo);

		sf_close(outfile);
		/*
		 printf("dane wy:\n");
		 for(int i=0;i<20;i++)
		 printf("%f\n", data[i]);

		 printf("writecount %d <----------------\n",writecount);
		 */
		licznik++;

//	}

	//
	onStep();
}

}//: namespace WAVFile_Sink
}//: namespace Sinks
