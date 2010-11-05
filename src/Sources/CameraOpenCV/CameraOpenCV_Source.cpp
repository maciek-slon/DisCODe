/*!
 * \file CameraOpenCV_Source.cpp
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
	LOG(LTRACE) << "CameraOpenCV_Source::CameraOpenCV_Source()\n";
	trig = true;
}

CameraOpenCV_Source::~CameraOpenCV_Source() {
	LOG(LTRACE) << "CameraOpenCV_Source::~CameraOpenCV_Source()\n";
}

bool CameraOpenCV_Source::onInit() {
	LOG(LTRACE) << "CameraOpenCV_Source::initialize()\n";
	newImage = registerEvent("newImage");



	h_onTrigger.setup(this, &CameraOpenCV_Source::onTrigger);
	registerHandler("onTrigger", &h_onTrigger);


	registerStream("out_img", &out_img);

	cap.open(0);

	if (cap.isOpened())
		LOG(LTRACE) << "CameraOpenCV: device opened\n";
	else
		LOG(LWARNING) << "CameraOpenCV: device NOT opened!\n";

	return cap.isOpened();
}


bool CameraOpenCV_Source::onFinish() {
	LOG(LTRACE) << "CameraOpenCV_Source::finish()\n";
	cap.release();

	return !cap.isOpened();
}


bool CameraOpenCV_Source::onStep() {
	if (props.triggered && !trig)
		return true;

	trig = false;
	cap >> frame;

	if (frame.empty()) {
		return false;
	}

	LOG(LTRACE) << "CameraOpenCV: got frame!\n";

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


void CameraOpenCV_Source::onTrigger() {
	trig = true;
}

}//: namespace CameraOpenCV
}//: namespace Sources
