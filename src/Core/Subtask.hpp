/*!
 * \file Subtask.hpp
 * \brief
 */

#ifndef SUBTASK_HPP_
#define SUBTASK_HPP_

#include <vector>
#include <string>
#include <map>

namespace Base {
	class Component;
}

namespace Core {

	class Executor;

	typedef std::pair<std::string, Executor*> ExecutorPair;
/*!
 * \class Subtask
 * \brief
 */
class Subtask
{
public:
	/*!
	 *
	 */
	Subtask(const std::string & n = "") : name_(n) {}

	/*!
	 *
	 */
	virtual ~Subtask();

	/*!
	 * Start all components in this subtask.
	 *
	 * \retval true if every component started without problems
	 * \retval false if at least one component returned false in it's start method
	 */
	bool start();

	/*!
	 * Stop all components in this subtask.
	 *
	 * \retval true if every component stopped without problems
	 * \retval false if at least one component returned false in it's stop method
	 */
	void stop();

	void initialize();

	void finish();

	const std::string & name() {
		return name_;
	}

	/*!
	 * Add executor to subtask
	 * \param ex pointer to executor to be added
	 */
	Subtask & operator += (Executor * ex);


	std::vector<std::string> listExecutors();

private:
	typedef std::vector<Base::Component*>::iterator comp_it;

	std::vector<Base::Component*> components;

	std::string name_;


	/// All executors in task
	std::map<std::string, Executor*> executors;

};

} //: namespace Core

#endif /* SUBTASK_HPP_ */
