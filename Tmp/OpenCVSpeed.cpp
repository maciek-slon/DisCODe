// OpenCV library
#include <cv.h>
#include <cvaux.h>
#include <cxcore.h>
#include <highgui.h>

using namespace cv;

// standard library
#include <iostream>
#include <cstdlib>
#include <cstdio>

using namespace std;

// simple timer
#include "Timer.hpp"

// OpenCV writes hue in range 0..180 instead of 0..360
#define H(x) (x>>1)

int main(int argc, char * argv[]) {
	if (argc < 2) {
		std::cout << "Usage: " << argv[0] << " movie_filename\n";
		return -1;
	}

	VideoCapture cap;
	Common::Timer total_time, step_time;
	int frames = 0;
	int frame_limit = 200;

	char buf[256];

	// try to open movie
	cap.open(argv[1]);
	if(!cap.isOpened()) {
		std::cout << "Can't open " << argv[1] << "\n";
		return -1;
	}

	Mat frame;
	Mat out;
	Mat hsv;
	vector<Mat> planes;

	total_time.restart();

	while(frames < frame_limit)
	{

		// grab next frame
		cap >> frame; // get a new frame
		if (frame.empty()) break;
		frames++;

		// clone grabbed frame
		out = frame.clone();

		// if set - do blur
		GaussianBlur(frame, out, Size(7,7), 0.0);

		hsv = out.clone();

		// convert image to HSV color space and split into separate planes
		cvtColor(out, hsv, CV_BGR2HSV);
		split(hsv, planes);

		//imshow("preview", planes[0]);

		//waitKey(2);
	}

	// calculate total running time
	float tot = total_time.elapsed();
	std::cout << "Total time: " << tot << " (avg: " << tot/frames << "spf = " << 1.0*frames/tot << "fps)\n";

	cap.release();

	return 0;
}
