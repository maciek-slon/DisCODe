/*!
 * \file Task.hpp
 * \brief
 * \date 08-07-2010
 */

#ifndef TASK_HPP_
#define TASK_HPP_

#include <string>
#include <map>

#include "Subtask.hpp"

namespace Core {

class Executor;

/*!
 * \class Task
 * \brief
 */
class Task {
public:
	/*!
	 * Start all threads of execution.
	 */
	bool start();

	/*!
	 * Stop all threads of execution
	 */
	bool stop();

	/*!
	 * Prepare task to unload.
	 *
	 * All components are stopped and finished, threads are terminated.
	 * Task can't be started again after calling finish.
	 */
	bool finish();

	/*!
	 * Get subtask by name
	 * \param name name of subtask
	 * \return reference to subtask
	 * \note If no subtask with given name is present, then new one is created
	 */
	Subtask & operator[](const std::string & name);

	/*!
	 * Add subtask to task
	 * \param st subtask to be added
	 */
	Task & operator += (Subtask & st) {
		subtasks[st.name()] = st;
		return *this;
	}

	/*!
	 * Add executor to task
	 * \param ex pointer to executor to be added
	 */
	Task & operator += (Executor * ex);

private:
	/// All subtasks
	std::map<std::string, Subtask> subtasks;

	/// All executors in task
	std::map<std::string, Executor*> executors;

	typedef std::pair<std::string, Executor*> ExecutorPair;

	typedef std::pair<std::string, Subtask> SubtaskPair;
};

} //: namespace Core

#endif /* TASK_HPP_ */
