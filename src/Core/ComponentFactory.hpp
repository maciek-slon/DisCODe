/*!
 * \file ComponentFactory.hpp
 * \brief File containing the ComponentFactory template class.
 *
 * \author tkornuta
 * \date Mar 11, 2010
 */

#ifndef COMPONENTFACTORY_HPP_
#define COMPONENTFACTORY_HPP_

#include <boost/function.hpp>
#include <boost/utility.hpp>
#include <boost/property_tree/ptree.hpp>

#include <string>

#include "Logger.hpp"
#include "Component_Aux.hpp"
#include "SharedLibrary.hpp"
#include "Props.hpp"

namespace Core {

/*!
 * \class ComponentFactory
 * \brief Template factory producing different component types.
 * \author tkornuta
 */
class ComponentFactory: boost::noncopyable
{
private:
	/*!
	 * Component name.
	 */
	std::string name;

	/*!
	 * Pointer to the associated dynamic library.
	 */
	Common::SharedLibrary lib;

	/*!
	 * Variable used for storing address of the functor returning created object.
	 */
	Base::returnComponent ret_object;

	/*!
	 * Pointer to object.
	 */
	Base::Component* object;

	/*!
	 * Variable used for storing address of the functor returning created panel.
	 */
	Base::returnPanel ret_panel;

	/*!
	 * Pointer to panel object.
	 */
	Base::Panel* panel;

	/*!
	 * Pointer to node containing component configuration
	 */
	ptree * config_node;

public:
	/*!
	 * Constructor. Resets pointers to objects.
	 */
	ComponentFactory()
	{
		// NULL pointers.
		panel = 0;
		object = 0;
		config_node = 0;
	}

	/*!
	 * Destructor - closes dl if it was previously opened, deletes objects.
	 */
	virtual ~ComponentFactory()
	{
		// Close dl if required.
		if (lib.loaded())
			lib.unload();

		// Free memory.
		if (object)
			delete (object);
		if (panel)
			delete (panel);
	}

	/*!
	 * Returns component name.
	 */
	std::string& getName()
	{
		return name;
	}

	/*!
	 * Return pointer to created object
	 */
	Base::Component * create(const std::string & name) {
		Base::Component * ret = ret_object(name);
		//ret->setName(name);
		return ret;
	}

	Base::Component * operator()(const std::string & name) {
		return create(name);
	}

	/*!
	 * Method tries to load component information from given dynamic library.
	 * \param filename_ shared library filename.
	 */
	bool lazyInitialize(string filename_)
	{
		try {
			// Try to open dll.
			lib.setLocation(filename_);
			// Validate operation.
			if (!lib.load())
				throw Common::DisCODeException(std::string("Library open error: ") + lib.error());

			// Try to retrieve method returning type.
			Base::returnType ret_type;
			ret_type = lib.get<Base::componentType>("returnType");
			if (!ret_type)
				throw Common::DisCODeException(std::string("Can't find returnType() in library: ") + lib.error());
			// Check type.
			/*if (ret_type() != COMPONENT_TYPE)
				throw Common::DisCODeException(filename_ + string(" doesn't contain a component of given type."));*/

			// Try to retrieve method returning component name.
			Base::returnName ret_name;
			ret_name = lib.get<std::string>("returnName");
			if (!ret_name)
				throw Common::DisCODeException(std::string("Can't find returnName() in library: ") + lib.error());
			// Retrieve component name.
			name = ret_name();

			// The rest is "lazy" - retrieve only functors, leave pointers to processor and panel unset.

			// Try to retrieve method returning processor.
			ret_object = lib.get<Base::Component*, const std::string &>("returnComponent");
			if (!ret_object)
				throw Common::DisCODeException(std::string("Can't load ret_object from library: ") + lib.error());

			// Try to retrieve method returning panel.
			ret_panel = lib.get<Base::Panel*>("returnPanel");
			if (!ret_panel)
				throw Common::DisCODeException(std::string("Can't load ret_panel from library: ") + lib.error());

			// Component initialized properly.
			LOG(LINFO) << "ComponentFactory: Dynamic library " << filename_ << " containing " << name
					<< " component was properly loaded.\n";
			return true;
		} catch (Common::DisCODeException& ex) {
			LOG(LINFO) << "ComponentFactory: " << ex.what() << "\n";
		}
		return false;
	}

	/*!
	 * Set config node associated to this component
	 */
	void setConfigNode(ptree * node) {
		config_node = node;
	}

};

}//: namespace Core

#endif /* COMPONENTFACTORY_HPP_ */
