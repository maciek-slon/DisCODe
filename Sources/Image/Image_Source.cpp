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

void Image_Source::connect()
{
	cout << "Image_Source::connect\n";
}


void Image_Source::disconnect()
{
	cout << "Image_Source::disconnect\n";
}


void* Image_Source::retrieveData()
{
	cout << "Image_Source::retrieveData\n";
	return NULL;
}


}//: namespace Image
}//: namespace Sources
