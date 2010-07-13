/*!
 * \file CameraOpenCV.cpp
 * \brief Class responsible for retrieving images from camera.
 * \author mstefanc
 * \date 2010-06-10
 */

#include <iostream>

#include "CameraOpenCV_Source.hpp"
#include "Logger.hpp"

namespace Sources {
namespace CameraOpenCV {

CameraOpenCV_Source::CameraOpenCV_Source(const std::string & name) : Base::Component(name) {
	LOG(TRACE) << "CameraOpenCV_Source::CameraOpenCV_Source()\n";
}

CameraOpenCV_Source::~CameraOpenCV_Source() {
	LOG(TRACE) << "CameraOpenCV_Source::~CameraOpenCV_Source()\n";
}

bool CameraOpenCV_Source::onInit() {
	LOG(TRACE) << "CameraOpenCV_Source::initialize()\n";
	newImage = registerEvent("newImage");

	registerStream("out_img", &out_img);

	cap.open(0);

	if (cap.isOpened())
		LOG(TRACE) << "CameraOpenCV: device opened\n";
	else
		LOG(WARNING) << "CameraOpenCV: device NOT opened!\n";

	return cap.isOpened();
}


bool CameraOpenCV_Source::onFinish() {
	LOG(TRACE) << "CameraOpenCV_Source::finish()\n";
	cap.release();

	return !cap.isOpened();
}


bool CameraOpenCV_Source::onStep() {

	cap >> frame;

	if (frame.empty()) {
		return false;
	}

	LOG(TRACE) << "CameraOpenCV: got frame!\n";

	out_img.write(frame);

	newImage->raise();
	return true;
}

bool CameraOpenCV_Source::onStart() {
	return true;
}

bool CameraOpenCV_Source::onStop() {
	return true;
}


}//: namespace CameraOpenCV
}//: namespace Sources
