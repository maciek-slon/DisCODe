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

	cap = NULL;
}

Movie_Source::~Movie_Source() {
	LOG(TRACE) << "Movie_Source::~Movie_Source()\n";
}

bool Movie_Source::onInit() {
	LOG(TRACE) << "Movie_Source::initialize()\n";
	newImage = registerEvent("newImage");

	registerStream("out_img", &out_img);

	cap.open(props.filename);

	return true;
}

bool Movie_Source::onFinish() {
	LOG(TRACE) << "Movie_Source::finish()\n";
	cap.release();

	return true;
}

bool Movie_Source::onStep() {

	cap >> frame;
	if (frame.empty()) {
		return false;
	}

	out_img.write(frame);

	newImage->raise();

	return true;
}

bool Movie_Source::onStart() {
	return true;
}

bool Movie_Source::onStop() {
	return true;
}



}//: namespace Movie
}//: namespace Sources
