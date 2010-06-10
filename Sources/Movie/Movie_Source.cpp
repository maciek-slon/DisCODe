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
	cap = NULL;

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

	cap.open(fname);
}


void Movie_Source::finish() {
	cout << "Movie_Source::finish()\n";
	cap.release();
}


int Movie_Source::step() {

	cap >> frame;

	out_img.write(frame);

	newImage->raise();
	return 0;
}


}//: namespace Movie
}//: namespace Sources
