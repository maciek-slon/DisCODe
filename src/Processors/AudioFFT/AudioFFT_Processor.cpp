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
	registerStream("in_info", &in_info);

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
			printf("aaa\n");
			mat = in_data.read().clone();
			sfinfo = in_info.read();
			mat_dft = mat.clone();

			for (int s = 0; s < 20; s++) // sample in time window
			{
					printf("%f\n", mat.at<double> (0, s));
			}

			printf("bbb\n");
			mat_l = mat.row(0);
			mat_r = mat.row(1);

			sample_size = sfinfo.samplerate * 10 / 1000;

			for (int s = 0; s < 20; s++) // sample in time window
			{
					printf("%f\n", mat_l.at<double> (0, s));
			}
			printf("\n");

			printf("ccc\n");
			// If there are 2 channels
			if (mat.rows == 2) {
				mat_dft_l = WindowedFFT(mat_l, sample_size).clone();

				mat_dft_r = WindowedFFT(mat_r, sample_size).clone();

			for (int s = 0; s < 10; s++) // sample in time window
			{
				for (int t = 0; t < 6; t++) // time window
					printf("%f\t\t", mat_dft_r.at<double> (s, t));
				printf("\n");
			}
			printf("\n");

				licznik++;

				printf("ddd\n");
				out_data_r.write(mat_dft_r.t());
				out_data_l.write(mat_dft_l.t());

				newDataL->raise();
				newDataR->raise();

			} else
				printf("Bad channels number\n");

		}
	} catch (...) {
		LOG(LERROR) << "AudioFFT::onNewData failed\n";
	}

}

double HammingW(int t, int s, int sample_size)
{
	double w;
	w=0.54-0.46*cos((2*3.14*(t+s))/(sample_size-1));
	return w;
}

// Performing Windowed Fourier Transform
cv::Mat AudioFFT_Processor::WindowedFFT(cv::Mat data_mat, int sample_size) {
	cv::Mat mat_samp = cv::Mat(sample_size, (data_mat.cols - sample_size)/(sample_size/4), CV_64FC1);
	cv::Mat mat_dft = mat_samp.t();
	cv::Mat row = cv::Mat(1,sample_size,CV_64FC1);

	for (int t = 0; t < (data_mat.cols - sample_size); t+=(sample_size/4))// time window
	{
		for (int s = 0; s < sample_size; s++)
		{
			if ((t - sample_size / 2) < 0 && s < sample_size / 2)
				row.at<double> (0, s) = 0;
			else if ((t + sample_size / 2) > data_mat.cols && s > sample_size / 2)
				row.at<double> (0, s) = 0;
			else
				row.at<double> (0, s) = data_mat.at<double> (0, (t-sample_size + s));

			mat_samp.at<double> (s, t/(sample_size/4)) = row.at<double> (0, s)*HammingW(t,s,sample_size);
		}
	}

	cv::dft(mat_samp.t(), mat_dft, cv::DFT_ROWS);

	return mat_dft;
}


}//: namespace AudioFFT
}//: namespace Processors
