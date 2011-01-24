/*
 * AudioSpectrogram.cpp
 *
 *  Created on: Nov 10, 2010
 *      Author: aszymane
 */

#include <memory>
#include <string>

#include "AudioSpectrogram_Processor.hpp"
#include "Logger.hpp"

namespace Processors {
namespace AudioSpectrogram {

AudioSpectrogram_Processor::AudioSpectrogram_Processor(const std::string & name) :
	Base::Component(name) {
	LOG(LTRACE) << "Hello AudioSpectrogram_Processor\n";
}

AudioSpectrogram_Processor::~AudioSpectrogram_Processor() {
	LOG(LTRACE) << "Good bye AudioSpectrogram_Processor\n";
}

bool AudioSpectrogram_Processor::onInit() {
	LOG(LTRACE) << "AudioSpectrogram_Processor::initialize\n";

	h_onNewData.setup(this, &AudioSpectrogram_Processor::onNewData);

	registerHandler("onNewData", &h_onNewData);

	registerStream("in_data", &in_data);

	newData = registerEvent("newData");

	registerStream("out_data", &out_data);

	licznik = 1;

	return true;
}

bool AudioSpectrogram_Processor::onFinish() {
	LOG(LTRACE) << "AudioSpectrogram_Processor::finish\n";

	return true;
}

bool AudioSpectrogram_Processor::onStep() {
	LOG(LTRACE) << "AudioSpectrogram_Processor::step\n";

	return true;
}

bool AudioSpectrogram_Processor::onStop() {
	return true;
}

bool AudioSpectrogram_Processor::onStart() {
	return true;
}

void AudioSpectrogram_Processor::onNewData() {
	LOG(LTRACE) << "AudioSpectrogram_Processor::onNewData\n";
	try {
		if (licznik == 1) {

			if (in_data.empty())
				return;

			mat_in = in_data.read();

			cv::Mat mat = mat_in.clone();

			// invert image
			for (int t = 0; t < mat_in.cols; t++)
				for (int s = 0; s < mat_in.rows; s++)
					mat.at<double> ((mat_in.rows - 1) - s, t) = mat_in.at<
							double> (s, t);

			// compute square of amplitude
			mat_out = ComputeSpectrogram(mat);

			max = Max(mat_out);
			min = Min(mat_out);

			for (int s = 0; s < mat_out.rows; s++) // sample in time window
				for (int t = 0; t < mat_out.cols; t++) // time window
					mat_out.at<double> (s, t) *= 1 + HammingW(mat_out.at<double> (s, t),0,max);


			mat_out.convertTo(img_out_transp, CV_8UC1, 0.1, 0);

			for (int s = 0; s < img_out_transp.rows; s++) // sample in time window
				for (int t = 0; t < img_out_transp.cols; t++) // time window
				{
					img_out_transp.at<uint8_t> (s, t) = (255-(img_out_transp.at<uint8_t> (s, t)));
				}

			max = Max(mat_out);
			min = Min(mat_out);

			// shorten image to have worse resolution
			mat_small = ShortenOutputMatrix(img_out_transp, 2);
			// extend image
			mat_big = ExtendOutputMatrix(mat_small, 4);

			// Proby kolorowania spektrogramu
//			cv::Mat maaat;
//			printf("aaaa\n");
//			mat_out.convertTo(maaat, CV_64FC1, 1, 0);
//
//			max=Max(maaat);
//			min=Min(maaat);
//			printf("min=%f\tmax=%f\n", min, max);
//			for(int s=0;s<mat_big.rows;s++)
//				for(int t=0;t<mat_big.cols;t++)
//				{
//					maaat.at<double>(s,t)=(maaat.at<double>(s,t));
//				}
//
//			for (int s = 0; s < 20; s++) {
//				for (int t = 0; t < 10; t++)
//					printf("%f\t\t", maaat.at<double> (s, t));
//				printf("\n");
//			}
//			printf("aaaa2\n");
//			img=cv::Mat(cv::Size(mat_out.cols,mat_out.rows),CV_64FC3);
//			printf("aaaa3\n");
//			jedynki=cv::Mat::ones(mat_out.rows,mat_out.cols,CV_64FC1);
//			jedynki2=cv::Mat::ones(mat_out.rows,mat_out.cols,CV_64FC1);
//			printf("aaaa4\n");
//			std::vector<cv::Mat> macierze=std::vector<cv::Mat>();
//			printf("aaaa5\n");
//			macierze.push_back(maaat);
//			macierze.push_back(jedynki);
//			macierze.push_back(jedynki2);
//
//			printf("maaat=(%d,%d),%d\t",maaat.cols,maaat.rows,maaat.channels());
//			printf("jedynki=(%d,%d).%d\t",jedynki.cols,jedynki.rows,jedynki.channels());
//			printf("img=(%d,%d),%d\t",img.cols,img.rows,img.channels());
//
//			printf("aaaa6\n");
//			cv::merge(macierze,img);
//			printf("aaaa7\n");
//

//			cv::cvtColor(img,mat_color,CV_RGB2HSV);
//			for(int t=0;t<mat_color.cols;t++)
//				for(int s=0;s<mat_color.rows;s++)
//				{
////					mat_color.at<cv::Scalar>(s,t)=cv::Scalar(mat_big.at<int>(s,t),1,1);
////					mat_color.at<double>(s,t*3)=mat_big.at<int>(s,t);
////					mat_color.at<double>(s,t*3+1)=1;
////					mat_color.at<double>(s,t*3+2)=1;
//				}
//			cv::cvtColor(img,mat_rgb,CV_HSV2RGB);

			licznik++;
			out_data.write(mat_big.clone());
			newData->raise();

		}
	} catch (...) {
		LOG(LERROR) << "AudioSpectrogram::onNewData failed\n";
	}

}

// Compute square of amplitude
cv::Mat AudioSpectrogram_Processor::ComputeSpectrogram(cv::Mat mat) {
	cv::Mat mat_out = cv::Mat(mat.rows, mat.cols / 2 + 1, CV_64FC1);
	// square of amplitude
	for (int s = 0; s < mat.rows; s++) // sample in time window
		for (int t = 0; t < mat.cols; t++) // time window
		{
			if (t == 0) // first is real value
				mat_out.at<double> (s, 0) = ((mat.at<double> (s, 0) * mat.at<
						double> (s, 0)) * 1e6);
			else {
				if (t == mat.cols - 1) // last is real value
					mat_out.at<double> (s, mat.cols - 1) = ((mat.at<double> (s,
							mat.cols - 1) * mat.at<double> (s, mat.cols - 1))
							* 1e6);
				else // complex values
				{
					mat_out.at<double> (s, t / 2 + 1) = ((mat.at<double> (s, t)
							* mat.at<double> (s, t) + mat.at<double> (s, t + 1)
							* mat.at<double> (s, t + 1)) * 1e6);
					t++;
				}
			}
		}

	return mat_out;
}

// Compute maximum value in matrix
double AudioSpectrogram_Processor::Max(cv::Mat mat) {
	double max = -10;
	for (int s = 0; s < mat.rows; s++)// sample in time window
		for (int t = 0; t < mat.cols; t++) // time window
			if (mat.at<double> (s, t) > max)
				max = mat.at<double> (s, t);
	return max;
}

// Compute minimum value in matrix
double AudioSpectrogram_Processor::Min(cv::Mat mat) {
	double min = 100000;
	for (int s = 0; s < mat.rows; s++)// sample in time window
		for (int t = 0; t < mat.cols; t++) // time window
			if (mat.at<double> (s, t) < min)
				min = mat.at<double> (s, t);
	return min;
}

// Extends matrix n times
cv::Mat AudioSpectrogram_Processor::ExtendOutputMatrix(cv::Mat mat, int n) {
	cv::Mat mat_big_ = cv::Mat(mat.rows * n, mat.cols * n, CV_8UC1);

	for (int s = 0; s < mat.rows; s++)// sample in time window
		for (int t = 0; t < mat.cols; t++) // time window
		{
			for (int i = 0; i < n; i++)
				for (int j = 0; j < n; j++)
					// extend value from mat to nxn square
					mat_big_.at<uint8_t> (s * n + i, t * n + j) = mat.at<
							uint8_t> (s, t);
		}

	return mat_big_;
}


// Shortens matrix n times
cv::Mat AudioSpectrogram_Processor::ShortenOutputMatrix(cv::Mat mat, int n) {
	cv::Mat mat_small_ = cv::Mat(mat.rows / n, mat.cols / n, CV_8UC1);
	double mean;

	for (int s = 0; s < mat_small_.rows; s++)// sample in time window
		for (int t = 0; t < mat_small_.cols; t++) // time window
		{
			mean=0;
			for (int i = 0; i < n; i++)
				for (int j = 0; j < n; j++)
					// extend value from mat to nxn square
					mean+=mat.at<uint8_t> (s * n + i, t * n + j);
			mat_small_.at<uint8_t> (s, t)=mean/(n*n);
		}

	return mat_small_;
}

// Compute Hamming window coefficients
double AudioSpectrogram_Processor::HammingW(int t, int s, int sample_size)
{
	double w;
	w=0.54-0.46*cos((2*3.14*(t+s))/(sample_size-1));
	return w;
}

}//: namespace AudioSpectrogram
}//: namespace Processors

