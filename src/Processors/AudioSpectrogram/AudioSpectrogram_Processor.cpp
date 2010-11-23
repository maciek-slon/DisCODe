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

AudioSpectrogram_Processor::AudioSpectrogram_Processor(
		const std::string & name) :
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
			printf("aa\n");

			if (in_data.empty())
				return;

			mat_in = in_data.read();//.clone();

			printf("ab\n");

			for (int i = 0; i < 10; i++) {
				for (int j = 0; j < 6; j++)
					printf("%f\t\t", mat_in.at<double> (i, j));
				printf("\n");
			}

			printf("bb\n");

			mat_out=ComputeSpectrogram(mat_in);//.clone();

			printf("mat rows=%d\tcols=%d\n", mat_in.rows, mat_in.cols);
			printf("mat_out rows=%d\tcols=%d\n", mat_out.rows, mat_out.cols);

			printf("cc\n");

			for (int s = 0; s < 10; s++) // sample in time window
			{
				for (int t = 0; t < 6; t++) // time window
					printf("%f\t\t", mat_out.at<double> (s, t));
				printf("\n");
			}
			printf("\n");

			printf("dd\n");

			max=Max(mat_out);
			min=Min(mat_out);

			printf("de\n");
			printf("min=%f\tmax=%f\t\n",min,max);
			printf("wsp=%f\n", ((max - min) )/256);

			for (int s = 0; s < mat_out.rows; s++) // sample in time window
				for (int t = 0; t < mat_out.cols; t++) // time window
				{
					if (max == min)
						max = max + 1;

					mat_out.at<double> (s, t) = (mat_out.at<double> (s, t) / ((max - min ))/255) ;
					mat_out.at<double>(s,t)=( (255.0-mat_out.at<double>(s,t)));
				}

			printf("df\n");
			for (int s = 0; s < 10; s++) {
				for (int t = 0; t < 6; t++)
					printf("%f\t\t", mat_out.at<double> (s, t));
				printf("\n");
			}

			printf("dg\n");
			mat_out.convertTo(img_out_transp, CV_8SC1, 1, 0);

			printf("dh\n");
			printf("mat_out row=%d\tcol=%d\n", mat_out.rows, mat_out.cols);
			printf("img_out_transp row=%d\tcol=%d\n", img_out_transp.rows,
					img_out_transp.cols);


			for (int s = 0; s < 20; s++) {
				for (int t = 0; t < 10; t++)
					printf("%d\t\t", img_out_transp.at<int> (s, t));
				printf("\n");
			}

			printf("di\n");

			// bigger image
			mat_big=ExtendOutputMatrix(img_out_transp,2);//.clone();
			printf("mat_big row=%d\tcol=%d\n",mat_big.rows,
					mat_big.cols);

			printf("dj\n");
			licznik++;
			printf("dk\n");
			out_data.write(mat_big.clone());
			printf("ee\n");
			newData->raise();
			printf("ff\n");

		}
	} catch (...) {
		LOG(LERROR) << "AudioSpectrogram::onNewData failed\n";
	}

	printf("gg\n");
}

cv::Mat AudioSpectrogram_Processor::ComputeSpectrogram(cv::Mat mat)
{
	cv::Mat mat_out = cv::Mat(mat.rows, mat.cols / 2 + 1, CV_64FC1);
	// kwadrat amplitudy
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


double AudioSpectrogram_Processor::Max(cv::Mat mat)
{
	double max = -10;
	for (int s = 0; s < mat.rows; s++)// sample in time window
		for (int t = 0; t < mat.cols; t++) // time window
			if (mat.at<double> (s, t) > max)
				max = mat.at<double> (s, t);
	return max;
}

double AudioSpectrogram_Processor::Min(cv::Mat mat)
{
	double min = 100000;
	for (int s = 0; s < mat.rows; s++)// sample in time window
		for (int t = 0; t < mat.cols; t++) // time window
			if (mat.at<double> (s, t) < min)
				min = mat.at<double> (s, t);
	return min;
}

// Extends matrix n times
cv::Mat AudioSpectrogram_Processor::ExtendOutputMatrix(cv::Mat mat, int n)
{

	printf("before1\n");
	cv::Mat mat_big_ = cv::Mat(mat.rows * 2, mat.cols * 2, CV_8UC1);
	printf("before2\n");
	for (int s = 0; s < mat.rows; s++)// sample in time window
		for (int t = 0; t < mat.cols; t++) // time window
		{
			for(int i=0;i<n;i++)
				for(int j=0;j<n;j++)
				{
					mat_big_.at<uint8_t> (s * 2 + i, t * 2 + j) = mat.at<uint8_t> (s,t);
//					printf("s=%8d,\tt=%8d,\trows=%8d,\tcols=%8d,\ti=%8d,\tj=%8d,\tmat_big i=%8d,\tmat_big j=%8d\n"
//							,s,t,mat.rows,mat.cols,i,j,s * 2 + i,t * 2 + j);
				}
//			mat_big.at<int> (s * 2 + 1, t * 2) = mat.at<int> (s, t);
//			mat_big.at<int> (s * 2, t * 2 + 1) = mat.at<int> (s, t);
//			mat_big.at<int> (s * 2 + 1, t * 2 + 1) = mat.at<int> (s, t);
		}
	printf("after\n");
	return mat_big_;
}

}//: namespace AudioSpectrogram
}//: namespace Processors

