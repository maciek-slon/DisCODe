/*
 * AudioFFT.cpp
 *
 *  Created on: Nov 9, 2010
 *      Author: aszymane
 */

#include <memory>
#include <string>

#include "AudioFFT_Processor.hpp"
#include "Logger.hpp"

namespace Processors {
namespace AudioFFT {

AudioFFT_Processor::AudioFFT_Processor(const std::string & name) :
	Base::Component(name) {
	LOG(LTRACE) << "Hello AudioFFT_Processor\n";
}

AudioFFT_Processor::~AudioFFT_Processor() {
	LOG(LTRACE) << "Good bye AudioFFT_Processor\n";
}

bool AudioFFT_Processor::onInit() {
	LOG(LTRACE) << "AudioFFT_Processor::initialize\n";

	h_onNewData.setup(this, &AudioFFT_Processor::onNewData);

	registerHandler("onNewData", &h_onNewData);

	registerStream("in_data", &in_data);

	newDataL = registerEvent("newDataL");
	newDataR = registerEvent("newDataR");

	registerStream("out_data_r", &out_data_r);
	registerStream("out_data_l", &out_data_l);

	licznik = 1;

	return true;
}

bool AudioFFT_Processor::onFinish() {
	LOG(LTRACE) << "AudioFFT_Processor::finish\n";

	return true;
}

bool AudioFFT_Processor::onStep() {
	LOG(LTRACE) << "AudioFFT_Processor::step\n";

	printf("step fft %d\n", licznik);
	return true;
}

bool AudioFFT_Processor::onStop() {
	return true;
}

bool AudioFFT_Processor::onStart() {
	return true;
}

void AudioFFT_Processor::onNewData() {
	LOG(LTRACE) << "AudioFFT_Processor::onNewData\n";

	try {
		if (licznik == 1) {
			mat = in_data.read().clone();
			mat_dft = mat.clone();

			mat_l = mat.row(0);
			mat_r = mat.row(1);

			sample_size = props.sample_size;

			// If there are 2 channels
			if (mat.rows == 2) {
				mat_dft_l=WindowedFFT(mat_l,sample_size).clone();

				mat_dft_r=WindowedFFT(mat_r,sample_size).clone();

//			for (int s = 0; s < 10; s++) // sample in time window
//			{
//				for (int t = 0; t < 6; t++) // time window
//					printf("%f\t\t", mat_dft_r.at<double> (s, t));
//				printf("\n");
//			}
//			printf("\n");

			licznik++;

			out_data_r.write(mat_dft_r.t());
			out_data_l.write(mat_dft_l.t());

			newDataL->raise();
			newDataR->raise();

			}
			else printf("Bad channels number\n");

		}
	} catch (...) {
		LOG(LERROR) << "AudioFFT::onNewData failed\n";
	}

}

// Performing Windowed Fourier Transform
cv::Mat AudioFFT_Processor::WindowedFFT(cv::Mat data_mat, int sample_size)
{
	cv::Mat mat_samp = cv::Mat(sample_size, ((data_mat.cols-sample_size) / 128), CV_64FC1);
	cv::Mat mat_dft = mat_samp.t();

	for(int t=0;t<(data_mat.cols-sample_size)/128;t++)// time window
	{
			for(int s=0;s<sample_size;s++)
				mat_samp.at<double>(s,t)=data_mat.at<double>(0,(t+s));
	}

	cv::dft(mat_samp.t(), mat_dft, cv::DFT_ROWS);

	return mat_dft;
}

}//: namespace AudioFFT
}//: namespace Processors
