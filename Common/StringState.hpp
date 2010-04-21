/*!
 * \file StringState.hpp
 * \brief File containing the StringState class.
 *
 * \author tkornuta
 * \date Apr 9, 2010
 */

#ifndef STRINGSTATE_HPP_
#define STRINGSTATE_HPP_

#include <string>
#include "State.hpp"
#include "Singleton.hpp"

namespace Common {

/*!
 * \class StringState
 * \brief State class storing only one field with string-typed value.
 * \date Apr 9, 2010
 * \author tkornuta
 */
class StringState: public Base::State <std::string>, public Base::Singleton <StringState>
{
	/*!
	 * Singleton class must be a friend, because only it can call class private constructor.
	 */
	friend class Base::Singleton <StringState>;

private:
	/*!
	 * Private constructor, called only by the Singleton::init() method.
	 */
	StringState()
	{
	}

	/*!
	 * Loads the values of the state from the xml node.
	 */
	void loadStateFromNode(const xmlNodePtr node_)
	{
		state = string((char*) xmlGetProp(node_, XMLCHARCAST"value"));
	}

	/*!
	 * Fills the xml node basing on the values of the state.
	 */
	void saveStateToNode(xmlNodePtr& node_)
	{
		xmlSetProp(node_, XMLCHARCAST "value", XMLCHARCAST state.c_str());
	}

	/*!
	 * Fills state with default values.
	 */
	void setDefaultState()
	{
		state = "default";
	}

};

}

#endif /* STRINGSTATE_HPP_ */
