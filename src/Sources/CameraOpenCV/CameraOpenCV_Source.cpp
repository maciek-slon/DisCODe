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

CameraOpenCV_Source::CameraOpenCV_Source(const std::string & name) : Base::Component(name),
		m_device("device", boost::bind(&CameraOpenCV_Source::onDeviceCahnged, this, _1, _2), 0),
		m_width("width", 640, "combo"),
		m_height("width", 480, "combo")
{
	LOG(LTRACE) << "CameraOpenCV_Source::CameraOpenCV_Source()\n";
	trig = true;

	registerProperty(m_device);

	m_width.addConstraint("320");
	m_width.addConstraint("640");
	registerProperty(m_width);

	m_height.addConstraint("240");
	m_height.addConstraint("480");
	registerProperty(m_height);

	valid = false;
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

	cap.open(m_device);

	if (cap.isOpened()) {
		LOG(LTRACE) << "CameraOpenCV: device opened\n";

		if (!cap.set(CV_CAP_PROP_FRAME_WIDTH, m_width))
			LOG(LWARNING) << "Couldn't set frame width.";
		if (!cap.set(CV_CAP_PROP_FRAME_HEIGHT, m_height))
			LOG(LWARNING) << "Couldn't set frame height.";


		LOG(LINFO) << "Frame width: " << cap.get(CV_CAP_PROP_FRAME_WIDTH);
		LOG(LINFO) << "Frame height: " << cap.get(CV_CAP_PROP_FRAME_HEIGHT);

	}
	else {
		LOG(LWARNING) << "CameraOpenCV: device NOT opened!\n";
	}

	valid = true;
	return cap.isOpened();
}


bool CameraOpenCV_Source::onFinish() {
	LOG(LTRACE) << "CameraOpenCV_Source::finish()\n";
	cap.release();

	return !cap.isOpened();
}


bool CameraOpenCV_Source::onStep() {
	if (!valid)
		return true;

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

void CameraOpenCV_Source::onDeviceCahnged(int old_device, int new_device) {
	valid = false;
	cap.release();
	cap.open(new_device);

	if (!cap.isOpened()) {
		LOG(LWARNING) << "Couldn't set new device!";
		m_device = old_device;
	}

	cap.open(m_device);
	valid = true;
}

}//: namespace CameraOpenCV
}//: namespace Sources
