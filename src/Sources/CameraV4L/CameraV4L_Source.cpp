/*!
 * \file CameraV4L_Source.cpp
 * \brief
 * \author mstefanc
 * \date 2010-05-27
 */

#include <iostream>

#include "CameraV4L_Source.hpp"

#include "VL_Common.hpp"

#include "VL.hpp"
#include "V4L.hpp"
#include "V4L2.hpp"

#include "Logger.hpp"

namespace Sources {
namespace CameraV4L {

CameraV4L_Source::CameraV4L_Source(const std::string & name) : Base::Component(name) {
	cout << "Hello CameraV4L_Source from dl\n";
}

CameraV4L_Source::~CameraV4L_Source() {
	cout << "Goodbye CameraV4L_Source from dl\n";
}

bool CameraV4L_Source::onInit() {
	cout << "CameraV4L_Source::initialize\n";

	newImage = registerEvent("newImage");

	registerStream("out_img", &out_img);

	LOG(LINFO) << "Trying possible libraries...\n";
	int whichCam = tryLib(props.device, props.io);
	LOG(LINFO) << "device: " << props.device << " | tryLib=" << whichCam << "\n";
	switch (whichCam)
	{
		case 0:
			cam = NULL;
			break;
		case 1:
			LOG(LINFO) << "Using V4L\n";
			cam = new V4L();
			break;
		case 2:
			LOG(LINFO) << "Using V4L2\n";
			cam = new V4L2();
			break;
		default:
			cam = NULL;
	}
	if (cam != NULL) {
		cam->loadFrameGrabber(1, props);
		return true;
	} else {
		return false;
	}
}


bool CameraV4L_Source::onFinish() {
	cout << "CameraV4L_Source::finish\n";
	return true;
}


bool CameraV4L_Source::onStep() {
	frame = Mat(cam->getOneFrame());

	out_img.write(frame);

	newImage->raise();

	return true;
}

bool CameraV4L_Source::onStart() {
	return true;
}

bool CameraV4L_Source::onStop() {
	return true;
}


}//: namespace CameraV4L
}//: namespace Sources
