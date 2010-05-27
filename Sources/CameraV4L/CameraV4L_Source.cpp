/*!
 * \file CameraV4L_Source.cpp
 * \brief
 * \author mstefanc
 * \date 2010-05-27
 */

#include <iostream>

#include "CameraV4L_Source.hpp"

namespace Sources {
namespace CameraV4L {

CameraV4L_Source::CameraV4L_Source() {
	cout << "Hello CameraV4L_Source from dl\n";

	initialize();
}

CameraV4L_Source::~CameraV4L_Source() {
	finish();

	cout << "Goodbye CameraV4L_Source from dl\n";
}

void CameraV4L_Source::initialize() {
	cout << "CameraV4L_Source::initialize\n";
}


void CameraV4L_Source::finish() {
	cout << "CameraV4L_Source::finish\n";
}


int CameraV4L_Source::step() {
	return 0;
}


}//: namespace CameraV4L
}//: namespace Sources
