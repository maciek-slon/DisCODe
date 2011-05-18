/*!
 * \file Executor.cpp
 * \brief
 */

#include "Executor.hpp"
#include "Component.hpp"
#include "Timer.hpp"

#include <cstdlib>

#include <boost/foreach.hpp>

namespace Core {

std::vector<std::string> Executor::listComponents() {
	std::vector<std::string> ret;
	BOOST_FOREACH(ComponentPair cp, components) {
		ret.push_back(cp.first);
	}
	return ret;
}


bool Executor::ensureState(ExecutorState st, const std::string & errmsg) {
	if ( (m_state == Loaded) && (st != Loaded) ) {
		LOG(LWARNING) << "Thread " << name() << " is not initialized yet. " << errmsg;
		return false;
	}

	if ( (m_state == Finished) && (st != Finished) ) {
		LOG(LWARNING) << "Thread " << name() << " is already finished. " << errmsg;
		return false;
	}

	if ( (m_state == Running) && (st != Running) ) {
		LOG(LWARNING) << "Thread " << name() << " is running. " << errmsg;
		return false;
	}

	if ( (m_state == Paused) && (st != Paused) ) {
		LOG(LWARNING) << "Thread " << name() << " is paused. " << errmsg;
		return false;
	}

	if ( (m_state == Pausing) && (st != Pausing) ) {
		LOG(LWARNING) << "Thread " << name() << " is pausing. " << errmsg;
		return false;
	}

	if ( (m_state == Starting) && (st != Starting) ) {
		LOG(LWARNING) << "Thread " << name() << " is starting. " << errmsg;
		return false;
	}

	if ( (m_state == Finishing) && (st != Finishing) ) {
		LOG(LWARNING) << "Thread " << name() << " is finishing. " << errmsg;
		return false;
	}


	return true;
}

void Executor::restart() {
	LOG(LTRACE) << "Executor::restart";
	if (!ensureState(Paused, "Can't restart."))
		return;

	// set state to running
	{
		boost::lock_guard<boost::mutex> lock(m_cond_mtx);
		m_state = Starting;
	}
	m_cond.notify_all();
}

void Executor::pause() {
	LOG(LTRACE) << "Executor::pause";
	if (!ensureState(Running, "Can't pause."))
		return;

	// set state to paused
	{
		boost::lock_guard<boost::mutex> lock(m_cond_mtx);
		m_state = Pausing;
	}
	m_cond.notify_all();
}

/*!
 * Initialize all managed components.
 */
void Executor::initialize() {
	LOG(LTRACE) << "Executor::initialize";
	if (!ensureState(Loaded, "Can't initialize."))
		return;

	// set state to paused
	{
		boost::lock_guard<boost::mutex> lock(m_cond_mtx);
		m_state = Paused;
	}
	m_cond.notify_all();
}

/*!
 * Reset execution thread, making it possible to start over again
 * (reinitialize components etc.).
 */
void Executor::reset() {
	/// \todo IMPLEMENT!
}

/*!
 * Finish main Executor loop thus ending associated thread.
 */
void Executor::finish() {
	LOG(LTRACE) << "Executor::finish";
	if (!ensureState(Paused, "Can't finish."))
		return;

	// set state to finished
	{
		boost::lock_guard<boost::mutex> lock(m_cond_mtx);
		m_state = Finishing;
	}
	m_cond.notify_all();
}

void Executor::run() {
	{
		boost::unique_lock<boost::mutex> lock(m_cond_mtx);

		LOG(LINFO) << "Executor " << name() << " thread started.";

		// wait until executor is initialized
		while(m_state == Loaded) {
			m_cond.wait(lock);
		}

		// initialize all components
		BOOST_FOREACH(ComponentPair cmp, components) {
			cmp.second->initialize();
		}

		LOG(LINFO) << "Executor " << name() << " initialized.";
	}

	size_t current_component = 0;

	for(;;) {

		if (m_state == Starting) {
			boost::unique_lock<boost::mutex> lock(m_cond_mtx);

			// start all components
			if (m_state == Starting) {
				BOOST_FOREACH(ComponentPair cmp, components) {
					cmp.second->start();
				}

				m_state = Running;


				LOG(LINFO) << "Executor " << name() << " components started.";
			}
		}

		if (m_state == Running) {

			// handle all pending events
			LOG(LTRACE) << "Executing events in " << name();
			//while (!queue.empty())
				executeEvents();

			if (!active_components.empty()) {
				active_components[current_component]->step();
				++current_component;
				current_component = current_component+1 >= active_components.size() ? 0 : current_component+1;

				LOG(LTRACE) << "Executor " << name() << ": current=" << current_component;
			}

			// if period set, then sleep until next wake-up
			if (m_period > 0) {
				// TODO: use periodic timer
				int msec = 1000*m_period;
				LOG(LTRACE) << "Thread " << name() << " sleep " << msec;
				//Thread::msleep(msec);
				sleep(1);
				LOG(LTRACE) << "Thread " << name() << " wakeup";
			}

			LOG(LTRACE) << "Thread " << name() << " works!";

		}

		// double-check if executor is paused
		if ( (m_state == Paused) || (m_state == Pausing) ) {
			boost::unique_lock<boost::mutex> lock(m_cond_mtx);

			LOG(LTRACE) << "Thread " << name() << " pause or pausing...";

			// pause all components
			if (m_state == Pausing) {
				LOG(LTRACE) << "Thread " << name() << " pausing...";
				BOOST_FOREACH(ComponentPair cmp, components) {
					cmp.second->stop();
				}

				m_state = Paused;
			}

			// wait until component is paused
			while(m_state == Paused) {
				m_cond.wait(lock);
			}
		}

		// stop main execution loop
		if (m_state == Finishing)
			break;

		yield();
	}


	BOOST_FOREACH(ComponentPair cmp, components) {
		cmp.second->finish();
	}

	m_state = Finished;

	LOG(LINFO) << "Executor " << name() << " thread finished.";
}

}//: namespace Core

