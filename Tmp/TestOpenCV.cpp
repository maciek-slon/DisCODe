/*
 * TestOpenCV.cpp
 *
 *  Created on: 2010-06-10
 *      Author: mstefanc
 */

#include <cv.h>
#include <cv.hpp>
#include <cvaux.hpp>
#include <cxcore.hpp>
#include <highgui.h>
#include <highgui.hpp>

using namespace cv;

#include <iostream>

#include "Timer.hpp"

#define H(x) (x>>1)

class Regions {
	vector< vector<Point> > rgs;

	Mat labels;

	int size;
public:
	Regions(const Mat & m) {
		labels = Mat::zeros(m.size(), CV_16UC1);
		size = 0;
	}

	void addPoint(int x, int y) {
		bool added = false;
		short l, added_to;

		// todo: mozna sprawdzac jedynie 4 piksele zamiast 8
		for (int i = -1; i <= 1; ++i) {
			for (int j = -1; j <= 1; ++j) {
				if (i == j == 0)
					continue;

				l = labels.at<short>(y-j, x-i);
				if (l > 0) {
					if (!added) {
						added = true;
						rgs[l-1].push_back(Point(x,y));
						added_to = l;
					} else {
					}
				} else {

				}
			}
		}
	}
};


char show = 'f';
bool stepmode = false;
bool trig = false;
bool do_blur = true;
bool cont = false;

void processKey(char ch) {
	switch(ch) {
	case ' ':
		trig = true;
		break;
	case 'a':
		stepmode = !stepmode;
		break;
	case 'b':
		do_blur = !do_blur;
		break;
	case 'c':
		cont = !cont;
		break;
	case 'e':
		show = 'e';
		break;
	case 'f':
		show = 'f';
		break;
	case 'h':
		show = 'h';
		break;
	case 's':
		show = 's';
		break;
	case 'v':
		show = 'v';
		break;
	}
}

int main(int argc, char * argv[]) {
	if (argc < 2) {
		std::cout << "Supply filename\n";
		return -1;
	}

	VideoCapture cap;
	Common::Timer step_time, total_time;
	int frames = 0;


	cap.open(argv[1]);
	if(!cap.isOpened()) {
		std::cout << "Can't open " << argv[1] << "\n";
		return -1;
	}

	Mat edges;
	Mat frame;
	Mat hsv;
	vector<Mat> planes;
	char ch;

	total_time.restart();

	for(;;)
	{
		//step_time.restart();

		if ( (stepmode && trig) || (!stepmode) ) {
			cap >> frame; // get a new frame
			if (frame.empty()) break;
			frames++;
			trig = false;
			if (do_blur)
				GaussianBlur(frame, frame, Size(7,7), 1.0);
		}

		cvtColor(frame, hsv, CV_BGR2HSV);
		split(hsv, planes);

		for (int y = 0; y < planes[0].rows; ++y) {
			for (int x = 0; x < planes[0].cols; ++x) {
				uchar col = planes[0].at<uchar>(y,x);
				uchar sat = planes[1].at<uchar>(y,x);
				uchar val = planes[2].at<uchar>(y,x);
				if (col < H(30))
					col = 255; // red
				else if (col < H(220))
					col = 0;
				else if (col < H(280))
					col = 0; // blue
				else if (col < H(320))
					col = 0;
				else
					col = 255; //red

//				if (sat < 20)
//					col = 0;

				if ( (val > 240) && (sat < 50) )
					col = 0;

				planes[0].at<uchar>(y,x) = col;
			}
		}

		//GaussianBlur(edges, edges, Size(7,7), 1.5, 1.5);
		Canny(planes[0], edges, 0, 30, 3);


		if (cont) {
			IplImage img = IplImage(frame);
			vector<vector<Point> > contours;
			vector<Vec4i> hierarchy;

			findContours( planes[0], contours, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );

			for (int i = 0; i < contours.size(); ++i) {
				Point2f center;
				float radius;
				minEnclosingCircle(Mat(contours[i]), center, radius);
				if (radius > 20 && radius < 150)
					cvCircle(&img, center, radius, CV_RGB(255, 0, 255), 2);
			}

			drawContours( frame, contours, -1, cvScalar(0, 0, 255, 0), 2 );
			frame = Mat(&img);
		}


		switch (show) {
		case 'e':
			imshow("preview", edges);
			break;
		case 'f':
			imshow("preview", frame);
			break;
		case 'h':
			imshow("preview", planes[0]);
			break;
		case 's':
			imshow("preview", planes[1]);
			break;
		case 'v':
			imshow("preview", planes[2]);
			break;
		}

		//std::cout << "Frame time: " << step_time.elapsed() << "\n";

		ch = waitKey(2);
		if (ch == 'q')
			break;
		else
			processKey(ch);

	}

	float tot = total_time.elapsed();
	std::cout << "Total time: " << tot << " (avg: " << tot/frames << "spf = " << 1.0*frames/tot << "fps)\n";

	cap.release();

	return 0;
}
