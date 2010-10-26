/*!
 * \file
 * \brief
 */

#include <iostream>

#include "Keyboard_Source.hpp"

namespace Sources {
namespace Keyboard {

Keyboard_Source::Keyboard_Source(const std::string & name) : Base::Component(name) {
	LOG(TRACE) << "Keyboard_Source::Keyboard_Source()\n";
}

Keyboard_Source::~Keyboard_Source() {
	LOG(TRACE) << "Keyboard_Source::~Keyboard_Source()\n";
}

bool Keyboard_Source::onInit() {
	LOG(TRACE) << "Keyboard_Source::initialize()\n";

	std::string evname;

	for (int i = 0; i < props.keys.length(); ++i) {
		if (key_events.count(props.keys[i]) < 1) {
				evname += props.keys[i];
				evname += "_Pressed";
				key_events[props.keys[i]] = registerEvent(evname);
				LOG(TRACE) << "Event " << evname << " created";
		}
	}

	return true;
}

bool Keyboard_Source::onFinish() {
	LOG(TRACE) << "Keyboard_Source::finish()\n";

	return true;
}

bool Keyboard_Source::onStep() {
	std::string buf;
	std::cin >> buf;

	for (int i = 0; i < buf.length(); ++i) {
		if (key_events.count(buf[i]) > 0) {
				LOG(TRACE) << "Raising event from key " << buf[i];
				key_events[buf[i]]->raise();
		}
	}


	LOG(TRACE) << "Keyboard_Source::step() end\n";
	return true;
}

bool Keyboard_Source::onStart() {
	return true;
}

bool Keyboard_Source::onStop() {
	return true;
}




}//: namespace Keyboard
}//: namespace Sources
