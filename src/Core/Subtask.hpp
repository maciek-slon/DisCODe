/*!
 * \file Subtask.hpp
 * \brief
 */

#ifndef SUBTASK_HPP_
#define SUBTASK_HPP_

#include <vector>
#include <string>

namespace Base {
	class Component;
}

namespace Core {

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
	Subtask(const std::string & n = "") : name_(n) {};

	/*!
	 *
	 */
	virtual ~Subtask();

	/*!
	 * Add new component to subtask.
	 *
	 * Component have to be created and initialized before.
	 *
	 * \param comp component to be added
	 * \return reference to itself, so it's possible to chain add components
	 */
	Subtask & operator +=(Base::Component* comp);

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
	bool stop();

	bool finish();

	const std::string & name() {
		return name_;
	}

private:
	typedef std::vector<Base::Component*>::iterator comp_it;

	std::vector<Base::Component*> components;

	std::string name_;
};

} //: namespace Core

#endif /* SUBTASK_HPP_ */
