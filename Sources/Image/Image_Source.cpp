/*!
 * \file Source_Image.cpp
 * \brief Class responsible for retrieving images from files - methods definition.
 * \author tkornuta
 * \date 2009-11-05
 */

#include <iostream>

#include "Image_Source.hpp"

namespace Sources {
namespace Image {

Image_Source::Image_Source() {
	cout << "Hello Image_Source from dl\n";

	initialize();
}

Image_Source::~Image_Source() {
	finish();

	cout << "Goodbye Image_Source from dl\n";
}

bool Image_Source::initialize() {
	cout << "Image_Source::initialize\n";
	newImage = registerEvent("newImage");

	registerStream("out_delay", &out_delay);

	return true;
}


bool Image_Source::finish() {
	cout << "Image_Source::finish\n";

	return true;
}


int Image_Source::step() {
	int del = rand()%20 * 100;
	cout << "Image_Source::step -> del = " << del << endl;
	out_delay.write(del);
	newImage->raise();
	return 0;
}


}//: namespace Image
}//: namespace Sources
