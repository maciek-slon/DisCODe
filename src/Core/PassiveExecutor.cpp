/*!
 * \file
 * \brief
 */

#include "PassiveExecutor.hpp"
#include "Timer.hpp"
#include "Component.hpp"

namespace Core {

void PassiveExecutor::run() {
	running = true;

	while(running) {
		if (paused) {
			/// \todo sync with mutex
			Common::Thread::msleep(50);
			continue;
		}

		// here should be mutex, and will be ;-)
		while (queue.empty()) {
			Common::Thread::msleep(1);
		}

		// process all waiting events
		executeEvents();

		yield();
	}
}

}//: namespace Core

