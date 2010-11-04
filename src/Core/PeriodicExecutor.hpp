/*!
 * \file
 * \brief
 */

#ifndef PERIODICEXECUTOR_HPP_
#define PERIODICEXECUTOR_HPP_

#include "Executor.hpp"

namespace Core {

/*!
 * \class PeriodicExecutor
 * \brief Execute component periodically.
 * This object calls step method from it's main component periodically with given interval.
 */
class PeriodicExecutor : public Executor {
public:
	PeriodicExecutor(const std::string & n) : Executor(n) {};

	virtual ~PeriodicExecutor() {
		double spl = elapsed/loops;
		double lps = 1.0 / spl;
		LOG(LNOTICE) << "Executor " << name() << " finished.\n\tDid " << loops << " loops in " << elapsed << " seconds (" << spl << "spl = " << lps << "lps)";
	}

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
	void run();

private:
	/// Maximum number of iterations
	int max_iter;

	/// Main component - it's step method will be called in each loop
	Base::Component * main_component;

	/// Main component name
	std::string mk_name;

	/// Periodic mode interval in seconds
	float interval;

	double elapsed;
	int loops;
};

}//: namespace Core

#endif /* PERIODICEXECUTOR_HPP_ */
