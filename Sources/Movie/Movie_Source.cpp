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

Movie_Source::Movie_Source(const std::string & name) : Base::Component(name) {
	LOG(TRACE) << "Movie_Source::Movie_Source()\n";

//	cap = NULL;
	trig = true;
}

Movie_Source::~Movie_Source() {
	LOG(TRACE) << "Movie_Source::~Movie_Source()\n";
}

bool Movie_Source::onInit() {
	LOG(TRACE) << "Movie_Source::initialize()\n";
	newImage = registerEvent("newImage");

	registerStream("out_img", &out_img);

	h_onTrigger.setup(this, &Movie_Source::onTrigger);
	registerHandler("onTrigger", &h_onTrigger);


	cap.open(props.filename);

	return cap.isOpened();
}

bool Movie_Source::onFinish() {
	LOG(TRACE) << "Movie_Source::finish()\n";
	cap.release();

	return true;
}

bool Movie_Source::onStep() {
	if (props.triggered && !trig)
		return true;

	trig = false;

	LOG(TRACE) << "Movie_Source::step() start\n";
	cap >> frame;
	if (frame.empty()) {
		return false;
	}

	cv::Mat img = frame.clone();
	out_img.write(img);

	newImage->raise();

	LOG(TRACE) << "Movie_Source::step() end\n";
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
