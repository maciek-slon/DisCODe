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
	LOG(LTRACE) << "Hello Sequence from dl\n";

	frame = 0;
	trig = true;
}

Sequence::~Sequence() {
	LOG(LTRACE) << "Goodbye Sequence from dl\n";
}

bool Sequence::onInit() {
	LOG(LTRACE) << "Sequence::initialize\n";

	h_onTrigger.setup(this, &Sequence::onTrigger);
	registerHandler("onTrigger", &h_onTrigger);

	newImage = registerEvent("newImage");
	endOfSequence = registerEvent("endOfSequence");

	registerStream("out_img", &out_img);

	if (!findFiles()) {
		LOG(LERROR) << name() << ": There are no files matching regex " << props.pattern << " in " << props.directory;
		return false;
	}

	return true;
}

bool Sequence::onFinish() {
	LOG(LTRACE) << "Sequence::finish\n";

	return true;
}

bool Sequence::onStep() {
	LOG(LTRACE) << "Sequence::onStep";

	if (props.triggered && !trig)
		return true;

	trig = false;

	if (frame >= files.size()) {
		LOG(LINFO) << name() << ": end of sequence\n";
		endOfSequence->raise();
		return false;
	}

	LOG(LTRACE) << "Sequence: reading image " << files[frame];
	try {
		img = cv::imread(files[frame++], -1);
	}
	catch(...) {
		LOG(LWARNING) << name() << ": image reading failed! [" << files[frame-1] << "]";
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

	LOG(LINFO) << "Sequence loaded.";
	BOOST_FOREACH(std::string fname, files)
		LOG(LINFO) << fname;

	return !files.empty();
}

void Sequence::onTrigger() {
	trig = true;
}

}//: namespace Sequence
}//: namespace Sources
