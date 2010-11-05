/*!
 * \file Movie_Source.cpp
 * \brief Class responsible for retrieving images from movies.
 * \author mstefanc
 * \date 2010-05-14
 */

#include <iostream>

#include "Movie_Source.hpp"

#include <boost/filesystem.hpp>

namespace Sources {
namespace Movie {

Movie_Source::Movie_Source(const std::string & name) : Base::Component(name) {
	LOG(LTRACE) << "Movie_Source::Movie_Source()\n";

//	cap = NULL;
	trig = true;
}

Movie_Source::~Movie_Source() {
	LOG(LTRACE) << "Movie_Source::~Movie_Source()\n";
}

bool Movie_Source::onInit() {
	LOG(LTRACE) << "Movie_Source::initialize()\n";
	newImage = registerEvent("newImage");

	registerStream("out_img", &out_img);

	h_onTrigger.setup(this, &Movie_Source::onTrigger);
	registerHandler("onTrigger", &h_onTrigger);

	if (!boost::filesystem::exists(props.filename)) {
		LOG(LERROR) << "File " << props.filename << " doesn't exist.";
		LOG(LNOTICE) << "Check config file or override movie filename thorugh -S switch.";
		return false;
	}

	cap.open(props.filename);

	if (!cap.isOpened()) {
		LOG(LERROR) << "Couldn't open movie " << props.filename;
		LOG(LNOTICE) << "Check if you have proper codecs installed.";
		return false;
	}

	return true;
}

bool Movie_Source::onFinish() {
	LOG(LTRACE) << "Movie_Source::finish()\n";
	cap.release();

	return true;
}

bool Movie_Source::onStep() {
	if (props.triggered && !trig)
		return true;

	trig = false;

	LOG(LTRACE) << "Movie_Source::step() start\n";
	cap >> frame;
	if (frame.empty()) {
		return false;
	}

	cv::Mat img = frame.clone();
	out_img.write(img);

	newImage->raise();

	LOG(LTRACE) << "Movie_Source::step() end\n";
	return true;
}

bool Movie_Source::onStart() {
	return true;
}

bool Movie_Source::onStop() {
	return true;
}

void Movie_Source::onTrigger() {
	trig = true;
}



}//: namespace Movie
}//: namespace Sources
