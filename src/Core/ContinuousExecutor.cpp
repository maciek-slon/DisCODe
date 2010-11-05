/*!
 * \file
 * \brief
 */

#include "ContinuousExecutor.hpp"
#include "Timer.hpp"
#include "Component.hpp"

namespace Core {

void ContinuousExecutor::run() {
	elapsed = 0;
	loops = 0;

	running = true;
	paused = false;

	LOG(LTRACE) << name() << "::run()";

	if (components.count(mk_name) < 1) {
		LOG(LERROR) << "Component " << mk_name << " is not being executed in this thread.\n";
		main_component = NULL;
		return;
	} else {
		main_component = components[mk_name];
	}

	while(running) {
		if (paused) {
			/// \todo sync with mutex
			LOG(LTRACE) << name() << " paused";
			Common::Thread::msleep(50);
			yield();
			continue;
		}

		LOG(LTRACE) << name() << " executing events";
		executeEvents();

		// check if there is any component to execute
		if (main_component && main_component->running()) {
			// check number of iterations
			if (max_iter >= 0) {
				--max_iter;
				if (max_iter < 0)
					break;
			}

			LOG(LTRACE) << name() << " making step (" << main_component->name() << ")";
			elapsed += main_component->step();
			loops++;
			double spl = elapsed/loops;
			double lps = 1.0 / spl;
			LOG(LDEBUG) << "Executor " << name() << ": " << loops << " loops in " << elapsed << " seconds (" << spl << "spl = " << lps << "lps)";

		} else {
			Common::Thread::msleep(50);
		}

		yield();
	}
}

}//: namespace Core

