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

/*!
 * \class Task
 * \brief
 */
class Task {
public:
	/*!
	 * Get subtask by name
	 * \param name name of subtask
	 * \return reference to subtask
	 * \note If no subtask with given name is present, then new one is created
	 */
	Subtask & operator[](const std::string & name) {
		return subtasks[name];
	}

private:
	std::map<std::string, Subtask> subtasks;
};

} //: namespace Core

#endif /* TASK_HPP_ */
