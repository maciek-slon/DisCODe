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
#include "Kernel.hpp"
#include "EventHandler.hpp"
#include "Timer.hpp"
#include "Props.hpp"

namespace Core {

/*!
 * \class Executor
 * \brief Executor object holds \ref Base::Kernel "kernels" and implements message queue.
 *
 * Executor is only interfave for concrete implementations. These implenetations
 * can differ in the way kernels are managed etc.
 *
 * \author mstefanc
 */
class Executor : public Common::Thread, public Base::Props {
public:
	/*!
	 * Add new Kernel to Executor.
	 * \param kernel kernel to be added to executor
	 * \param main if set to true then added kernel is set to be main
	 */
	void addKernel(const std::string & name, Base::Kernel * kernel) {
		kernels[name] = kernel;
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

protected:
	/// List of kernels managed by this Executor
	std::map<std::string, Base::Kernel *> kernels;

	/// Flag indicating that executor is running
	volatile bool running;

	/// FIFO queue for incoming events
	std::queue<Base::EventHandlerInterface *> queue;
};




/*!
 * \class ContinousExecutor
 * \brief This object calls step method from it's main kernel continously.
 */
class ContinousExecutor : public Executor {
public:
	/*!
	 * Load executor settings from given configuration node
	 */
	void load(const ptree & pt) {
		max_iter = pt.get("iterations", -1);
		mk_name = pt.get("main_kernel", "");
	}

protected:
	/*!
	 * Implementation of run method from Thread.
	 */
	void run() {
		running = true;

		if (kernels.count(mk_name) < 1) {
			LOG(ERROR) << "Kernel " << mk_name << " is not executed in this thread.\n";
			main_kernel = NULL;
			return;
		} else {
			main_kernel = kernels[mk_name];
		}

		while(running) {

			while (!queue.empty()) {
				queue.front()->execute();
				queue.pop();
			}

			// check number of iterations
			if (max_iter >= 0) {
				--max_iter;
				if (max_iter < 0)
					break;
			}

			// check if there is any kernel to execute
			if (main_kernel)
				main_kernel->step();

			yield();
		}
	}

private:
	/// Maximum number of iterations
	int max_iter;

	/// Main kernel - it's step method will be called in each loop
	Base::Kernel * main_kernel;

	/// Main kernel name
	std::string mk_name;
};



/*!
 * \class PassiveExecutor
 * \brief This object doesn't call any methods from it's kernels explicitely,
 * but only process events and calls connected handlers.
 */
class PassiveExecutor : public Executor {
public:
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
 * \brief This object calls step method from it's main kernel periodically
 * with given interval.
 */
class PeriodicExecutor : public Executor {
public:
	/*!
	 * Load executor settings from given configuration node
	 */
	void load(const ptree & pt) {
		max_iter = pt.get("iterations", -1);
		mk_name = pt.get("main_kernel", "");
		interval = pt.get("interval", 1.0);
	}

protected:
	/*!
	 * Implementation of run method from Thread.
	 */
	void run() {
		running = true;

		if (kernels.count(mk_name) < 1) {
			LOG(ERROR) << "Kernel " << mk_name << " is not executed in this thread.\n";
			main_kernel = NULL;
			return;
		} else {
			main_kernel = kernels[mk_name];
		}

		while(running) {

			while (!queue.empty()) {
				queue.front()->execute();
				queue.pop();
			}

			if (timer.elapsed() > interval) {
				timer.restart();

				// check number of iterations
				if (max_iter >= 0) {
					--max_iter;
					if (max_iter < 0)
						break;
				}

				// check if there is any kernel to execute
				if (main_kernel)
					main_kernel->step();
			} else {
				Common::Thread::msleep(100*interval);
			}

			yield();
		}
	}

private:
	/// Maximum number of iterations
	int max_iter;

	/// Main kernel - it's step method will be called in each loop
	Base::Kernel * main_kernel;

	/// Main kernel name
	std::string mk_name;

	/// Timer used in periodic mode
	Common::Timer timer;

	/// Periodic mode interval in seconds
	float interval;
};

}//: namespace Core

#endif /* EXECUTOR_HPP_ */
