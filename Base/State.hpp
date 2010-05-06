/*!
 * \file State.hpp
 * \brief File containing the State template class.
 *
 * \author tkornuta
 * \date April 8 2010
 */

#ifndef STATE_HPP_
#define STATE_HPP_

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <string>
#include <iostream>

#include "boost/lexical_cast.hpp"

#include "Singleton.hpp"

using namespace std;

namespace Base {

/*!
 * Macro used during xml strings casting.
 */
#ifndef XMLCHARCAST
#define XMLCHARCAST (const xmlChar *)
#endif

class XMLTranslatableState
{
private:
	/*!
	 * Node related to state of given component.
	 */
	xmlNodePtr node;

	/*!
	 * Loads the values of the state from the xml node.
	 */
	virtual void loadStateFromNode(const xmlNodePtr node_) = 0;

	/*!
	 * Fills the xml node basing on the values of the state.
	 */
	virtual void saveStateToNode(xmlNodePtr& node_) = 0;

	/*!
	 * Fills state with default values.
	 */
	virtual void setDefaultState() = 0;

public:

	/*!
	 *
	 */
	virtual ~XMLTranslatableState() {

	}

	/*!
	 * Copies reference to given node and tries to load state.
	 * If unsuccesfull, creates default state and saves (translates) it to node.
	 */
	void setNode(xmlNodePtr& node_)
	{
		node = node_;
		try {
			// Try to load state from node.
			loadStateFromNode(node);
		} catch (exception& e_) {
			cout << "Configuration:" << e_.what() << endl;
			// Set default values and save them to node.
			setDefaultState();
			saveStateToNode(node);
		}//: catch

		/// \todo Add synchronization.
	}

	/*!
	 * Saves current state to default (private) xml node.
	 */
	void saveStateToNode()
	{
		saveStateToNode(node);
	}

	/*!
	 * Returns node.
	 */
	/*	xmlNodePtr getNode()
	 {
	 /// \todo Add synchronization.
	 return node;
	 }*/

};

/*!
 * \class State
 * \brief Template class responsible for storing different components state.
 * Each state-able component should inherit from this class
 * \date Apr 8, 2010
 * \author tkornuta
 */
template <typename STATE_STRUCT>
class State: public Base::XMLTranslatableState //: public Base::Singleton <State <STATE_STRUCT> >
{
protected:
	/*!
	 * State id, related to the id of related xml node (section in file_.
	 */
	//std::string id;

	/*!
	 * Template-structure storing state.
	 */
	STATE_STRUCT state;

	/*!
	 * Protected constructor, called only by constructors of derived classes.
	 */
	State()
	{
		//cout << "State: Hello private \n";
	}

public:
	/*!
	 * Public destructor.
	 */
	virtual ~State()
	{
		//cout << "State: Goodbye public \n";
	}

	/*!
	 * Returns state structure.
	 */
	STATE_STRUCT getState() const
	{
		/// \todo Add synchronization.
		return state;
	}

	/*!
	 * Sets state structure.
	 */
	void setState(const STATE_STRUCT state_)
	{
		/// \todo Add synchronization.
		state = state_;
	}

};

/////////////////////// TESTS /////////////////////////////////////////

struct my_struct
{
	int port;
	std::string task;
};

class myState: public Base::State <my_struct>, public Base::Singleton <myState>
{
	/*!
	 * Singleton class must be a friend, because only it can call class private constructor.
	 */
	friend class Base::Singleton <myState>;

private:
	/*!
	 * Private constructor, called only by the Singleton::init() method.
	 */
	myState()
	{
		cout << "myState: Hello private \n";
	}

	/*!
	 * Loads the values of the state from the xml node.
	 */
	void loadStateFromNode(const xmlNodePtr node_)
	{
		state.port = boost::lexical_cast <int>(string((char*) xmlGetProp(node_, XMLCHARCAST"port")));
		state.task = string((char*)xmlGetProp (node_, XMLCHARCAST "default_task"));
	}

	/*!
	 * Fills the xml node basing on the values of the state.
	 */
	void saveStateToNode(xmlNodePtr& node_)
	{
		std::string s = boost::lexical_cast<std::string> (state.port);
		xmlSetProp(node_, XMLCHARCAST "port", XMLCHARCAST s.c_str());
		xmlSetProp(node_, XMLCHARCAST "default_task", XMLCHARCAST state.task.c_str());
	}

	/*!
	 * Fills state with default values.
	 */
	void setDefaultState()
	{
		state.port = 40000;
		state.task = "Example";
	}

};

}//: namespace Base

#endif /* STATE_HPP_ */

