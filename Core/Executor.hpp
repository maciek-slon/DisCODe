/*!
 * \file Executor.hpp
 * \brief
 *
 * \author mstefanc
 * \date 2010-05-02
 */

#ifndef EXECUTOR_HPP_
#define EXECUTOR_HPP_

#include <iostream>
#include <vector>
#include <queue>
#include <map>

#include "Thread.hpp"
#include "Component.hpp"
#include "EventHandler.hpp"
#include "Timer.hpp"
#include "Props.hpp"

namespace Core {

/*!
 * \class Executor
 * \brief Executor object holds \ref Base::Component "components" and implements message queue.
 *
 * Executor is only interface for concrete implementations. These implementations
 * can differ in the way components are managed etc.
 *
 * \author mstefanc
 */
class Executor : public Common::Thread, public Base::Props {
public:

	Executor(const std::string & n) : running(false), paused(true), name_(n) {
	}

	virtual ~Executor() {
	}

	/*!
	 * Add new Component to Executor.
	 * \param name name of component
	 * \param component component to be added to executor
	 */
	void addComponent(const std::string & name, Base::Component * component) {
		components[name] = component;
	}

	/*!
	 * Queue event handler in internal FIFO buffer
	 */
	void queueEvent(Base::EventHandlerInterface * h) {
		/// \todo Add synchronization
		queue.push(h);
	}

	/*!
	 * Returns event handler scheduler for given handler.
	 * \param h event handler to be scheduled
	 * \returns pointer to event handler scheduler
	 */
	Base::EventHandlerInterface * scheduleHandler(Base::EventHandlerInterface * h) {
		Base::EventScheduler<Executor> * handler = new Base::EventScheduler<Executor>();
		handler->setup(this, &Executor::queueEvent);
		handler->registerHandler(h);
		return handler;
	}

	void restart() {
		paused = false;
		if (!running)
			start();

	}

	void pause() {
		paused = true;
	}


	/*!
	 * Finish main Executor loop thus ending associated thread.
	 */
	void finish() {
		running = false;
	}

	/*!
	 * Save configuration
	 */
	void save(ptree & pt) {
	}

	/*!
	 * Return name
	 */
	const std::string & name() const {
		return name_;
	}

protected:
	/// List of components managed by this Executor
	std::map<std::string, Base::Component *> components;

	/// Flag indicating that executor is running
	volatile bool running;

	/// Flag indicating that executor is paused
	volatile bool paused;

	/// FIFO queue for incoming events
	std::queue<Base::EventHandlerInterface *> queue;

	/// Name of execution thread
	std::string name_;
};




/*!
 * \class ContinousExecutor
 * \brief Call step continously.
 * This object calls step method from it's main component continously.
 */
class ContinousExecutor : public Executor {
public:

	ContinousExecutor(const std::string & n) : Executor(n) {};

	virtual ~ContinousExecutor() {
		double spl = elapsed/loops;
		double lps = 1.0 / spl;
		LOG(NOTICE) << "Executor " << name() << " finished.\n\tDid " << loops << " loops in " << elapsed << " seconds (" << spl << "spl = " << lps << "lps)";
	}

	/*!
	 * Load executor settings from given configuration node
	 */
	void load(const ptree & pt) {
		max_iter = pt.get("iterations", -1);
		mk_name = pt.get("main_component", "");
	}

protected:
	/*!
	 * Implementation of run method from Thread.
	 */
	void run() {
		elapsed = 0;
		loops = 0;

		running = true;
		paused = false;

		if (components.count(mk_name) < 1) {
			LOG(ERROR) << "Component " << mk_name << " is not being executed in this thread.\n";
			main_component = NULL;
			return;
		} else {
			main_component = components[mk_name];
		}

		while(running) {
			if (paused) {
				/// \todo sync with mutex
				Common::Thread::msleep(50);
				yield();
				continue;
			}

			while (!queue.empty()) {
				queue.front()->execute();
				queue.pop();
			}

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
				LOG(INFO) << "Executor " << name() << ": " << loops << " loops in " << elapsed << " seconds (" << spl << "spl = " << lps << "lps)";

			} else {
				Common::Thread::msleep(50);
			}

			yield();
		}
	}

private:
	/// Maximum number of iterations
	int max_iter;

	/// Main component - it's step method will be called in each loop
	Base::Component * main_component;

	/// Main component name
	std::string mk_name;


	double elapsed;
	int loops;
};



/*!
 * \class PassiveExecutor
 * \brief Don't call any step, only react.
 * This object doesn't call any methods from it's components explicitely,
 * but only process events and calls connected handlers.
 */
class PassiveExecutor : public Executor {
public:
	PassiveExecutor(const std::string & n) : Executor(n) {};

	/*!
	 * Load executor settings from given configuration node
	 */
	void load(const ptree & pt) {
	}

protected:
	/*!
	 * Implementation of run method from Thread.
	 */
	void run() {
		running = true;

		while(running) {
			if (paused) {
				/// \todo sync with mutex
				Common::Thread::msleep(50);
				continue;
			}

			// here should be mutex, and will be ;-)
			while (queue.empty()) {
				Common::Thread::msleep(10);
			}

			// process all waiting events
			while (!queue.empty()) {
				queue.front()->execute();
				queue.pop();
			}

			yield();
		}
	}

private:
};



/*!
 * \class PeriodicExecutor
 * \brief Execute component periodically.
 * This object calls step method from it's main component periodically with given interval.
 */
class PeriodicExecutor : public Executor {
public:
	PeriodicExecutor(const std::string & n) : Executor(n) {};

	/*!
	 * Load executor settings from given configuration node
	 */
	void load(const ptree & pt) {
		max_iter = pt.get("iterations", -1);
		mk_name = pt.get("main_component", "");
		interval = pt.get("interval", 1.0);
	}

protected:
	/*!
	 * Implementation of run method from Thread.
	 */
	void run() {
		running = true;

		if (components.count(mk_name) < 1) {
			LOG(ERROR) << "Component " << mk_name << " is not executed in this thread.\n";
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

			while (!queue.empty()) {
				queue.front()->execute();
				queue.pop();
			}

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

					main_component->step();
				} else {
					Common::Thread::msleep(50);
				}
			} else {
				Common::Thread::msleep(100*interval);
			}

			yield();
		}
	}

private:
	/// Maximum number of iterations
	int max_iter;

	/// Main component - it's step method will be called in each loop
	Base::Component * main_component;

	/// Main component name
	std::string mk_name;

	/// Timer used in periodic mode
	Common::Timer timer;

	/// Periodic mode interval in seconds
	float interval;
};

}//: namespace Core

#endif /* EXECUTOR_HPP_ */
