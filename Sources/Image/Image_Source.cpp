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

Image_Source::Image_Source()
{
	cout << "Hello Image_Source from dl\n";
}

Image_Source::~Image_Source()
{
	cout << "Goodbye Image_Source from dl\n";
}

void Image_Source::initialize()
{
	cout << "Image_Source::initialize\n";
}


void Image_Source::finish()
{
	cout << "Image_Source::finish\n";
}


int Image_Source::step()
{
	cout << "Image_Source::step\n";
	return 0;
}


}//: namespace Image
}//: namespace Sources
