/*!
 * \file
 * \brief
 */

#include "PeriodicExecutor.hpp"
#include "Timer.hpp"
#include "Component.hpp"

namespace Core {

void PeriodicExecutor::run() {
	/// Timer used in periodic mode
	Common::Timer timer, main_timer;

	elapsed = 0;
	loops = 0;

	double time_elapsed = 0;
	double to_sleep = interval;

	running = true;

	if (components.count(mk_name) < 1) {
		LOG(LERROR) << "Component " << mk_name << " is not executed in this thread.\n";
		main_component = NULL;
		return;
	} else {
		main_component = components[mk_name];
	}

	timer.restart();
	main_timer.restart();
	while(running) {
		if (paused) {
			/// \todo sync with mutex
			Common::Thread::msleep(50);
			continue;
		}

		executeEvents();

		to_sleep -= timer.elapsed();
		timer.restart();

		if (to_sleep <= 0) {

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
				LOG(LTRACE) << "Executor " << name() << ": " << loops << " loops in " << elapsed << " seconds (" << spl << "spl = " << lps << "lps)";
			} else {
				Common::Thread::msleep(50);
			}

			time_elapsed += main_timer.elapsed();
			main_timer.restart();

			to_sleep += interval - timer.elapsed();
			timer.restart();
			LOG(LTRACE) << "Thread " << name() << " is going asleep for " << to_sleep;

		}
		try {
			Common::Thread::msleep(to_sleep * 1000);
		}
		catch (const boost::thread_interrupted & ex) {
		}
	}

	double spl = time_elapsed/loops;
	double lps = 1.0 / spl;

	LOG(LNOTICE) << "Executor " << name() << ": " << loops << " loops in " << time_elapsed << " seconds (" << spl << "spl = " << lps << "lps)";
	LOG(LINFO) << "Executor " << name() << " is periodic, actual period was " << spl << " while it should be " << interval << " (" << (spl / interval * 100) << "%)";
}

}//: namespace Core

