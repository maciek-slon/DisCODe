/*!
 * \file
 * \brief
 */

#ifndef CONTINUOUSEXECUTOR_HPP_
#define CONTINUOUSEXECUTOR_HPP_

#include "Executor.hpp"

namespace Core {

/*!
 * \class ContinuousExecutor
 * \brief Call step continuously.
 * This object calls step method from it's main component continuously.
 */
class ContinuousExecutor : public Executor {
public:

	ContinuousExecutor(const std::string & n) : Executor(n) {};

	virtual ~ContinuousExecutor() {
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


	double elapsed;

	int loops;
};

}//: namespace Core

#endif /* CONTINUOUSEXECUTOR_HPP_ */
