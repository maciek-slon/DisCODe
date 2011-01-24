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

	return true;
}

bool AudioFFT_Processor::onFinish() {
	LOG(LTRACE) << "AudioFFT_Processor::finish\n";

	return true;
}

bool AudioFFT_Processor::onStep() {
	LOG(LTRACE) << "AudioFFT_Processor::step\n";

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

			// read data
			mat = in_data.read().clone();

			// If there are 2 channels
			if (mat.rows == 3) {
				// extract canal data into two arrays
				mat_l = mat.row(1);
				mat_r = mat.row(2);

				// compute size of window and shift between windows
				sample_size = mat.at<double>(0,1) * props.sample_time / 1000;
				sample_delay = mat.at<double>(0,1) * props.sample_delay / 1000;

				// Perform WFFT for both canals
				mat_dft_l = WindowedFFT(mat_l, sample_size,sample_delay).clone();
				mat_dft_r = WindowedFFT(mat_r, sample_size,sample_delay).clone();

				out_data_r.write(mat_dft_r.t());
				out_data_l.write(mat_dft_l.t());

				newDataL->raise();
				newDataR->raise();

			} else
			{
				printf("Bad channels number\n");

				mat_l.create(1, mat.at<double>(0,0), CV_64FC1);
				mat_r.create(1, mat.at<double>(0,0), CV_64FC1);
			}

//		}
	} catch (...) {
		LOG(LERROR) << "AudioFFT::onNewData failed\n";
	}

}

double AudioFFT_Processor::HammingW(int t, int s, int sample_size)
{
	double w;
	w=0.54-0.46*cos((2*3.14*(t+s))/(sample_size-1));
	return w;
}
double AudioFFT_Processor::RectangleW(int t, int s, int sample_size)
{
	return 1;
}

// Performing Windowed Fourier Transform
cv::Mat AudioFFT_Processor::WindowedFFT(cv::Mat data_mat, int sample_size, int sample_delay) {
	cv::Mat mat_samp = cv::Mat(sample_size, (data_mat.cols - sample_size)/sample_delay, CV_64FC1);
	cv::Mat mat_dft = mat_samp.t();
	cv::Mat row = cv::Mat(1,sample_size,CV_64FC1);

	// rewrite data to use Windowed FFT
	for (int t = 0; t < (data_mat.cols - sample_size); t+=sample_delay)// time window
	{
		for (int s = 0; s < sample_size; s++)
		{
			if ((t - sample_size / 2) < 0 && s < sample_size / 2)
				row.at<double> (0, s) = 0;
			else if ((t + sample_size / 2) > data_mat.cols && s > sample_size / 2)
				row.at<double> (0, s) = 0;
			else
				row.at<double> (0, s) = data_mat.at<double> (0, (t-sample_size + s));

			if(props.window_type=='h')
				mat_samp.at<double> (s, t/sample_delay) = row.at<double> (0, s)*HammingW(t,s,sample_size);
			else if(props.window_type=='r')
				mat_samp.at<double> (s, t/sample_delay) = row.at<double> (0, s)*RectangleW(t,s,sample_size);
			else
				mat_samp.at<double> (s, t/sample_delay) = row.at<double> (0, s)*RectangleW(t,s,sample_size);
		}
	}

	// perform FFT for rows
	cv::dft(mat_samp.t(), mat_dft, cv::DFT_ROWS);

	return mat_dft;
}


}//: namespace AudioFFT
}//: namespace Processors
