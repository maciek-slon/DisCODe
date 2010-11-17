/*
 * AudioFileSpectrogram.cpp
 *
 *  Created on: Nov 10, 2010
 *      Author: aszymane
 */

#include <memory>
#include <string>

#include "AudioFileSpectrogram_Processor.hpp"
#include "Logger.hpp"

namespace Processors {
namespace AudioFileSpectrogram {

AudioFileSpectrogram_Processor::AudioFileSpectrogram_Processor(
		const std::string & name) :
	Base::Component(name) {
	LOG(LTRACE) << "Hello AudioFileSpectrogram_Processor\n";
}

AudioFileSpectrogram_Processor::~AudioFileSpectrogram_Processor() {
	LOG(LTRACE) << "Good bye AudioFileSpectrogram_Processor\n";
}

bool AudioFileSpectrogram_Processor::onInit() {
	LOG(LTRACE) << "AudioFileSpectrogram_Processor::initialize\n";

	h_onNewData.setup(this, &AudioFileSpectrogram_Processor::onNewData);

	registerHandler("onNewData", &h_onNewData);

	registerStream("in_data", &in_data);

	newData = registerEvent("newData");

	registerStream("out_data", &out_data);

	licznik = 1;

	return true;
}

bool AudioFileSpectrogram_Processor::onFinish() {
	LOG(LTRACE) << "AudioFileSpectrogram_Processor::finish\n";

	return true;
}

bool AudioFileSpectrogram_Processor::onStep() {
	LOG(LTRACE) << "AudioFileSpectrogram_Processor::step\n";

	return true;
}

bool AudioFileSpectrogram_Processor::onStop() {
	return true;
}

bool AudioFileSpectrogram_Processor::onStart() {
	return true;
}

void AudioFileSpectrogram_Processor::onNewData() {
	LOG(LTRACE) << "AudioFileSpectrogram_Processor::onNewData\n";
	try {
		if (licznik == 1) {
			printf("aa\n");

			cv::Mat mat = in_data.read().clone();

			printf("ab\n");
			cv::Mat mat_out = cv::Mat(mat.cols / 2, mat.rows, CV_64FC1);


			for (int i = 0; i < 10; i++) {
				for (int j = 0; j < 6; j++)
					printf("%f\t\t", mat.at<double> (i, j));
				printf("\n");
			}

			printf("rows=%d\tcols=%d\n",mat.rows,mat.cols);
			printf("rows=%d\tcols=%d\n",mat_out.rows,mat_out.cols);
			// kwadrat amplitudy
			for (int i = 0; i < mat.rows; i++)
				for (int j = 0; j < mat.cols; j += 2)
				{
//					printf("row=%d\tcol=%d\tamp=%f\n",i,j,mat.at<double>(i,j)*mat.at<double>(i,j) + mat.at<double>(i,j+1)*mat.at<double>(i,j+1));
					mat_out.at<double>(j/2,i) = mat.at<double>(i,j)*mat.at<double>(i,j) + mat.at<double>(i,j+1)*mat.at<double>(i,j+1);
				}

			double max = -100000, min = 100000;
			printf("bb\n");

			for (int i = 0; i < mat_out.rows; i++)
				for (int j = 0; j < mat_out.cols; j++) {

//					printf("%f<%f\t\t", mat.at<double> (i, j),max);
					if (mat_out.at<double> (i, j) > max) {
						max = mat_out.at<double> (i, j);
						printf("a max=%f, mat=%f\n", max,mat_out.at<double> (i, j));
					}

					if (mat_out.at<double> (i, j) < min) {
						min = mat_out.at<double> (i, j);
						printf("b min=%f, mat=%f\n", min,mat_out.at<double> (i, j));
					}
				}

			printf("min=%f\tmax=%f\n",min, max);
			//			printf("cc\n");
			for (int i = 0; i < mat_out.rows; i++)
				for (int j = 0; j < mat_out.cols; j++)
					mat_out.at<double> (i, j) -= min;

			for (int i = 0; i < 10; i++) {
				for (int j = 0; j < 6; j++)
					printf("%f\t\t", mat_out.at<double> (i, j));
				printf("\n");
			}


			max = -100000;
			min = 100000;

			for (int i = 0; i < mat_out.rows; i++)
				for (int j = 0; j < mat_out.cols; j++) {
					if (mat_out.at<double> (i, j) > max) {
						max = mat_out.at<double> (i, j);
						printf("a max=%f, (%d,%d)\n", max,i,j);
					}

					if (mat_out.at<double> (i, j) < min) {
						min = mat_out.at<double> (i, j);
						printf("b min=%f, (%d,%d)\n", min,i,j);
					}
				}

			printf("%f\n",((max - min )+ 1));

			printf("row=%d\tcol=%d\n",mat_out.rows,mat_out.cols);
			for(int i=0;i<mat_out.rows;i++)
				for(int j=0;j<mat_out.cols;j++)
				{
//					printf("row=%d\tcol=%d\t",mat_out.rows,mat_out.cols);
//					printf("row=%d\tcol=%d\tnorm=%f\n",i,j,(mat_out.at<double> (i, j) / (max - min + 1)) * 255);
					mat_out.at<double> (i, j) = (mat_out.at<double> (i, j) / (max - min + 1)) * 255;
				}


/*

 			for (int i = 40; i < 50; i++) {
				for (int j = 14; j < 20; j++)
					printf("%f\t\t", mat.at<double> (i, j));
				printf("\n");
			}
			printf("\n");
			*/

			for (int i = 0; i < 10; i++) {
				for (int j = 0; j < 6; j++)
					printf("%f\t\t", mat_out.at<double> (i, j));
				printf("\n");
			}

			cv::Mat img_out=Mat(mat_out.cols, mat_out.rows,CV_8UC1);
			for(int i=0;i<mat_out.rows;i++)
							for(int j=0;j<mat_out.cols;j++)
								img_out.at<double> (i, j) = (int) mat_out.at<double> (i, j);

			cv::Mat img_out_transp=Mat(mat_out.rows, mat_out.cols,CV_8UC1);
			img_out_transp=img_out.t();
printf("dd\n");
			licznik++;
//			out_data.write(img_out.clone());
			out_data.write(img_out_transp.clone());
			newData->raise();

		}
	} catch (...) {
		LOG(LERROR) << "AudioFileSpectrogram::onNewData failed\n";
	}
}

}//: namespace AudioFileSpectrogram
}//: namespace Processors

