/*!
 * \file
 * \brief
 */

#include <iostream>

#include "Keyboard_Source.hpp"

namespace Sources {
namespace Keyboard {

Keyboard_Source::Keyboard_Source(const std::string & name) : Base::Component(name) {
	LOG(LTRACE) << "Keyboard_Source::Keyboard_Source()\n";
}

Keyboard_Source::~Keyboard_Source() {
	LOG(LTRACE) << "Keyboard_Source::~Keyboard_Source()\n";
}

bool Keyboard_Source::onInit() {
	LOG(LTRACE) << "Keyboard_Source::initialize()\n";

	std::string evname;

	for (std::size_t i = 0; i < props.keys.length(); ++i) {
		if (key_events.count(props.keys[i]) < 1) {
				evname = props.keys[i];
				evname += "_Pressed";
				key_events[props.keys[i]] = registerEvent(evname);
				LOG(LTRACE) << "Event " << evname << " created";
		}
	}

	return true;
}

bool Keyboard_Source::onFinish() {
	LOG(LTRACE) << "Keyboard_Source::finish()\n";

	return true;
}

bool Keyboard_Source::onStep() {
	std::string buf;
	std::cin >> buf;

	for (std::size_t i = 0; i < buf.length(); ++i) {
		if (key_events.count(buf[i]) > 0) {
				LOG(LTRACE) << "Raising event from key " << buf[i];
				key_events[buf[i]]->raise();
		}
	}


	LOG(LTRACE) << "Keyboard_Source::step() end\n";
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
