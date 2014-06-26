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

	typedef enum {Running, Stopped, Initializing} TaskState;

	Task() : m_state(Task::Initializing) {std::cout << "Kopiowanie TASKA!\n";}

	/*!
	 * Start all subtasks.
	 */
	bool start(bool init = false);

	/*!
	 * Stop all subtasks.
	 */
	bool stop();

	/*!
	 * Initialize all subtasks.
	 */
	void initialize();

	/*!
	 * Finish task. Stop all subtasks and prepare to unload (or start over again).
	 */
	bool finish();

	/*!
	 * Get subtask by name
	 * \param name name of subtask
	 * \return reference to subtask
	 * \note If no subtask with given name is present, then new one is created
	 */
	Subtask * operator[](const std::string & name);

	/*!
	 * Add subtask to task
	 * \param st subtask to be added
	 */
	Task & operator += (Subtask * st) {
		subtasks[st->name()] = st;
		return *this;
	}

	/*!
	 * Check, whether subtask exists
	 * \arg name name of the subtask to check
	 * \return
	 */
	bool checkSubtask(const std::string & name) {
		return (subtasks.count(name) == 1);
	}

	std::vector<std::string> listSubtasks();
	std::vector<std::string> listExecutors();
	
	TaskState state() const {
		return m_state;
	}

private:
	/// All subtasks
	std::map<std::string, Subtask*> subtasks;

	typedef std::pair<std::string, Subtask*> SubtaskPair;
	
	TaskState m_state;
};

} //: namespace Core

#endif /* TASK_HPP_ */
