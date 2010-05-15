/*!
 * \file Movie_Source.cpp
 * \brief Class responsible for retrieving images from movies.
 * \author mstefanc
 * \date 2010-05-14
 */

#include <iostream>

#include "Movie_Source.hpp"

namespace Sources {
namespace Movie {

Movie_Source::Movie_Source() {
	cout << "Movie_Source::Movie_Source()\n";

	fname = "zakaz_1.avi";

	initialize();
}

Movie_Source::~Movie_Source() {
	finish();

	cout << "Movie_Source::~Movie_Source()\n";
}

void Movie_Source::initialize() {
	cout << "Movie_Source::initialize()\n";
	newImage = registerEvent("newImage");

	registerStream("out_img", &out_img);

	capture = cvCaptureFromAVI(fname.c_str());
}


void Movie_Source::finish() {
	cout << "Movie_Source::finish()\n";
	cvReleaseCapture( &capture );
}


int Movie_Source::step() {

	frame = cvQueryFrame( capture );
	if( !frame ) return 0;

	out_img.write(frame);

	newImage->raise();
	return 0;
}


}//: namespace Movie
}//: namespace Sources
