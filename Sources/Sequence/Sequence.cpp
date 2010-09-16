/*!
 * \file Sequence.cpp
 * \brief Class responsible for retrieving images from image sequences - methods definition.
 */

#include "Sequence.hpp"

#include "Utils.hpp"

#include <algorithm>
#include <boost/foreach.hpp>

#include <highgui.h>

namespace Sources {
namespace Sequence {

Sequence::Sequence(const std::string & name) : Base::Component(name) {
	LOG(TRACE) << "Hello Sequence from dl\n";

	frame = 0;
}

Sequence::~Sequence() {
	LOG(TRACE) << "Goodbye Sequence from dl\n";
}

bool Sequence::onInit() {
	LOG(TRACE) << "Sequence::initialize\n";

	newImage = registerEvent("newImage");

	registerStream("out_img", &out_img);

	if (!findFiles())
		return false;

	return true;
}

bool Sequence::onFinish() {
	LOG(TRACE) << "Sequence::finish\n";

	return true;
}

bool Sequence::onStep() {
	if (frame >= files.size())
		return false;

	img = cv::imread(files[frame++], -1);

	out_img.write(img);
	newImage->raise();
	return true;
}

bool Sequence::onStart() {
	return true;
}

bool Sequence::onStop() {
	return true;
}



bool Sequence::findFiles() {
	files.clear();

	files = Utils::searchFiles(props.directory, props.pattern);

	if (props.sort)
		std::sort(files.begin(), files.end());

	LOG(INFO) << "Sequence loaded.";
	BOOST_FOREACH(std::string fname, files)
		LOG(INFO) << fname;

	return !files.empty();
}

}//: namespace Sequence
}//: namespace Sources
