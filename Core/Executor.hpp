/*!
 * \file Executor.hpp
 * \brief
 *
 * \author mstefanc
 * \date 2010-05-02
 */

#ifndef EXECUTOR_HPP_
#define EXECUTOR_HPP_

#include <vector>
#include <queue>

#include "Thread.hpp"
#include "Kernel.hpp"
#include "EventHandler.hpp"

namespace Core {

/*!
 * \class Executor
 * \brief Executor object holds \ref Base::Kernel "kernels" and implements message queue.
 *
 * Executor can manage more than one kernel, however only one can be set as main.
 * Main kernel is used in some modes of operation.
 *
 * Executor has three modes of operation:
 *  - continous: main kernel's step method is called infinitely
 *  - periodic: main kernel's step method is called periodically
 *  - passive: all kernels only react on events
 *
 * \author mstefanc
 */
class Executor : public Common::Thread {
	typedef std::pair<std::string, Base::EventHandlerInterface *> HandlerPair;
public:
	/*!
	 * Executor's operation mode
	 */
	enum ExecOpMode {
		ExecContinous,
		ExecPeriodic,
		ExecPassive
	};

	/*!
	 * Constructor.
	 * Default operation mode is set to ExecContinous.
	 */
	Executor() {
		op_mode = ExecContinous;
		main_kernel = NULL;
		running = false;
		max_iter = -1;
	}

	/*!
	 * Destructor. Deletes all allocated handler schedulers.
	 */
	~Executor() {
		// delete all handler schedulers
		BOOST_FOREACH(Base::EventHandlerInterface * handler, handlers) {
			delete handler;
		}
	}

	/*!
	 * Add new Kernel to Executor.
	 * \param kernel kernel to be added to executor
	 * \param main if set to true then added kernel is set to be main
	 */
	void addKernel(Base::Kernel * kernel, bool main = false) {
		kernels.push_back(kernel);
		if (main) main_kernel = kernel;
	}

	/*!
	 * Finish main Executor loop thus ending associated thread.
	 */
	void finish() {
		running = false;
	}

	/*!
	 * Queue event handler in internal FIFO buffer
	 */
	void queueEvent(Base::EventHandlerInterface * h) {
		queue.push(h);
	}

	/*!
	 * Set maximum number of iterations
	 */
	void setIterationsCount(int i) {
		max_iter = i;
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

protected:
	/*!
	 * Implementation of run method from Thread.
	 */
	void run() {
		running = true;
		while(running) {

			while (!queue.empty()) {
				queue.front()->execute();
				queue.pop();
			}

			if (max_iter >= 0) {
				--max_iter;
				if (max_iter < 0)
					break;
			}


			switch(op_mode) {
			case ExecContinous:
				if (main_kernel)
					main_kernel->step();
				break;
			case ExecPeriodic:
				break;
			case ExecPassive:
				break;
			default:
				break;
			}

			yield();
		}
	}

private:
	/// List of kernels managed by this Executor
	std::vector<Base::Kernel *> kernels;

	/// Main kernel
	Base::Kernel * main_kernel;

	/// Current mode of operation
	ExecOpMode op_mode;

	/// Flag indicating that executor is running
	volatile bool running;

	/// List of all handler schedulers associated for managed kernels
	std::vector<Base::EventHandlerInterface *> handlers;

	/// FIFO queue for incoming events
	std::queue<Base::EventHandlerInterface *> queue;

	/// Maximum number of iterations
	int max_iter;
};

}//: namespace Core

#endif /* EXECUTOR_HPP_ */
