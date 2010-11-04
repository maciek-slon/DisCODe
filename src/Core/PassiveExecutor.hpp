/*!
 * \file
 * \brief
 */

#ifndef PASSIVEEXECUTOR_HPP_
#define PASSIVEEXECUTOR_HPP_

#include "Executor.hpp"

namespace Core {

/*!
 * \class PassiveExecutor
 * \brief Don't call any step, only react.
 * This object doesn't call any methods from it's components explicitly,
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
	void run();

private:
};

}//: namespace Core

#endif /* PASSIVEEXECUTOR_HPP_ */
