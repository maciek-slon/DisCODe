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
	trig = true;
}

Sequence::~Sequence() {
	LOG(TRACE) << "Goodbye Sequence from dl\n";
}

bool Sequence::onInit() {
	LOG(TRACE) << "Sequence::initialize\n";

	h_onTrigger.setup(this, &Sequence::onTrigger);
	registerHandler("onTrigger", &h_onTrigger);

	newImage = registerEvent("newImage");

	registerStream("out_img", &out_img);

	if (!findFiles()) {
		LOG(ERROR) << name() << ": There are no files matching regex " << props.pattern << " in " << props.directory;
		return false;
	}

	return true;
}

bool Sequence::onFinish() {
	LOG(TRACE) << "Sequence::finish\n";

	return true;
}

bool Sequence::onStep() {
	LOG(TRACE) << "Sequence::onStep";

	if (props.triggered && !trig)
		return true;

	trig = false;

	if (frame >= files.size()) {
		LOG(INFO) << name() << ": end of sequence\n";
		return false;
	}

	LOG(TRACE) << "Sequence: reading image\n";
	try {
		img = cv::imread(files[frame++], -1);
	}
	catch(...) {
		LOG(WARNING) << name() << ": image reading failed! [" << files[frame-1] << "]";
	}

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

void Sequence::onTrigger() {
	trig = true;
}

}//: namespace Sequence
}//: namespace Sources
