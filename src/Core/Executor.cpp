/*!
 * \file Executor.cpp
 * \brief
 */

#include "Executor.hpp"
#include "Component.hpp"
#include "Timer.hpp"

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


void PeriodicExecutor::run() {
	/// Timer used in periodic mode
	Common::Timer timer;
	timer.restart();

	elapsed = 0;
	loops = 0;

	running = true;

	if (components.count(mk_name) < 1) {
		LOG(LERROR) << "Component " << mk_name << " is not executed in this thread.\n";
		main_component = NULL;
		return;
	} else {
		main_component = components[mk_name];
	}

	while(running) {
		if (paused) {
			/// \todo sync with mutex
			Common::Thread::msleep(50);
			continue;
		}

		executeEvents();

		if (timer.elapsed() > interval) {
			timer.restart();

			// check if there is any component to execute
			if (main_component && main_component->running()) {
				// check number of iterations
				if (max_iter >= 0) {
					--max_iter;
					if (max_iter < 0)
						break;
				}

				elapsed += main_component->step();
				loops++;
				double spl = elapsed/loops;
				double lps = 1.0 / spl;
				LOG(LDEBUG) << "Executor " << name() << ": " << loops << " loops in " << elapsed << " seconds (" << spl << "spl = " << lps << "lps)";
			} else {
				Common::Thread::msleep(50);
			}
		} else {
			Common::Thread::msleep(100*interval);
		}

		yield();
	}
}

}//: namespace Core

