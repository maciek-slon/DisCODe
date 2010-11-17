/*
 * AudioFileFFT.cpp
 *
 *  Created on: Nov 9, 2010
 *      Author: aszymane
 */

#include <memory>
#include <string>

#include "AudioFileFFT_Processor.hpp"
#include "Logger.hpp"

namespace Processors {
namespace AudioFileFFT {

AudioFileFFT_Processor::AudioFileFFT_Processor(const std::string & name) :
	Base::Component(name) {
	LOG(LTRACE) << "Hello AudioFileFFT_Processor\n";
}

AudioFileFFT_Processor::~AudioFileFFT_Processor() {
	LOG(LTRACE) << "Good bye AudioFileFFT_Processor\n";
}

bool AudioFileFFT_Processor::onInit() {
	LOG(LTRACE) << "AudioFileFFT_Processor::initialize\n";

	h_onNewData.setup(this, &AudioFileFFT_Processor::onNewData);

	registerHandler("onNewData", &h_onNewData);

	registerStream("in_data", &in_data);

	newData = registerEvent("newData");

	registerStream("out_data_r", &out_data_r);
	registerStream("out_data_l", &out_data_l);

	licznik = 1;

	return true;
}

bool AudioFileFFT_Processor::onFinish() {
	LOG(LTRACE) << "AudioFileFFT_Processor::finish\n";

	return true;
}

bool AudioFileFFT_Processor::onStep() {
	LOG(LTRACE) << "AudioFileFFT_Processor::step\n";

	printf("step fft %d\n", licznik);
	return true;
}

bool AudioFileFFT_Processor::onStop() {
	return true;
}

bool AudioFileFFT_Processor::onStart() {
	return true;
}

void AudioFileFFT_Processor::onNewData() {
	LOG(LTRACE) << "AudioFileFFT_Processor::onNewData\n";

	printf("step onNewData1 %d\n", licznik);

	try {
		if (licznik == 1) {
			mat = in_data.read().clone();
			mat_dft = mat.clone();

			mat_l = mat.row(0);
			mat_r = mat.row(1);

			data_sample = 1024;

			mat_samp_l = cv::Mat(data_sample, (mat_l.cols / data_sample),
					CV_64FC1);
			mat_dft_l = mat_samp_l.clone();

			mat_samp_r = cv::Mat(data_sample, (mat_r.cols / data_sample),
					CV_64FC1);
			mat_dft_r = mat_samp_r.clone();
			// If there are 2 channels
			if (mat.rows == 2) {

				for (int t = 0; t < (mat_l.cols / data_sample); t++)
					for (int s = 0; s < data_sample; s++) {
						//						printf("%d < %d \t\t%d < %d\t\t%d < %d\n", t, mat_l.cols / data_sample, s, data_sample, t * s + s, mat_l.cols);
						mat_samp_l.at<double> (s, t) = mat_l.at<double> (0, t
								* s + s);
					}

				/*				printf("lewy kanal\n");
				 for (int t = 0; t < (10); t++) {
				 for (int s = 0; s < 9; s++)
				 printf("%f\t\t", mat_samp_l.at<double> (t, s));
				 printf("\n");
				 }
				 */
				cv::dft(mat_samp_l, mat_dft_l, DFT_ROWS);

				/*				printf("lewy kanal po dft\n");
				 for (int t = 0; t < (10); t++) {
				 for (int s = 0; s < 4; s++)
				 printf("%f\t\t", mat_dft_l.at<double> (t, s));
				 printf("\n");
				 }
				 */
				for (int t = 0; t < (mat_r.cols / data_sample); t++)
					for (int s = 0; s < data_sample; s++)
						mat_samp_r.at<double> (s, t) = mat_r.at<double> (0, t
								* s + s);

				/*				printf("prawy kanal\n");
				 for (int t = 0; t < (10); t++) {
				 for (int s = 0; s < 9; s++)
				 printf("%f\t\t", mat_samp_r.at<double> (t, s));
				 printf("\n");
				 }
				 */

				cv::dft(mat_samp_r, mat_dft_r, DFT_ROWS);
				/*
				 printf("prawy kanal po dft\n");
				 for (int t = 0; t < (10); t++) {
				 for (int s = 0; s < 4; s++)
				 printf("%f\t\t", mat_dft_r.at<double> (t, s));
				 printf("\n");
				 }
				 */

			licznik++;

			printf("do zapisane\n");
			out_data_r.write(mat_dft_r.clone());
			out_data_l.write(mat_dft_l.clone());

			newData->raise();

			}
			else printf("Bad channels number\n");

			//			for (int c = 0; c < mat.rows; c++) {
			//				cv::Mat row_before = mat.row(c);
			//				cv::Mat row_after = mat_dft.row(c);
			/*
			 printf("row %d before\n", c);
			 for (int j = 0; j < 10; j++)
			 printf("%f\t\t", row_before.at<float> (0, j));
			 */
			//				cv::dft(row_before, row_after); //FFT
			/*
			 printf("row %d after\n", c);

			 for (int j = 0; j < 10; j++)
			 printf("%f\t\t", row_after.at<float> (0, j));

			 float suma = 0;
			 for (int j = 0; j < data.sfinfo.frames * data.sfinfo.channels; j++)
			 suma += row_after.at<float> (0, j);

			 printf("suma=%d\n", suma);
			 for (int j = 0; j < mat_dft.cols; j++)
			 mat_dft.at<float> (c, j) = row_after.at<float> (0, j);
			 */
			//			}

			//			printf("do wypisania\n");
			//			printf("%d channels\n", mat.rows);
			//			printf("%d cols, %d rows\n", mat.cols, mat.rows);
			/*		for(int j=0;j<20;j++)
			 {
			 for(int c=0;c<data.sfinfo.channels;c++)
			 //				printf("%f\t\t", data.data[j*c+c]);
			 printf("%f\t\t", mat_dft.at<float>(c,j));
			 printf("\n next\n");
			 }
			 printf("po\n");
			 */
		}
	} catch (...) {
		LOG(LERROR) << "AudioFileFFT::onNewData failed\n";
	}

	//	printf("step onNewData2 %d\n", licznik);
}

}//: namespace AudioFileFFT
}//: namespace Processors
