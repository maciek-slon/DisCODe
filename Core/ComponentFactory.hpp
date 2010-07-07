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
		deactivate();

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
	Base::Component * create() {
		return ret_object();
	}

	Base::Component * operator()() {
		return ret_object();
	}

	/*!
	 * Methods activates component - initializes its elements, etc.
	 */
	void activate()
	{
		LOG(TRACE) << "ComponentFactory: Lazy activate!\n";
		if (!panel) {
			// Get task panel.
			panel = ret_panel();
			// Initialize panel.
			panel->createContent();
		}//: if !panel
		// Show panel.
		//panel->show();

		if (!object) {
			object = ret_object();
			if (object->getProperties())
				object->getProperties()->load(*config_node);
			object->initialize();
		}//: if !object
	}

	/*!
	 * Methods deactivates component - deletes its unused objects, etc.
	 */
	void deactivate()
	{
#ifdef DESTROY_DEACTIVED_COMPONENT
		// Hide panel.
		//	panel->hide();
		// Destroy objects.
		if (object) {
			if (object->getProperties())
				object->getProperties()->save(*config_node);
			object->finish();
			delete (object);
		}
		if (panel) {
			delete (panel);
		}
		// Set pointers to NULL.
		panel=0;
		object=0;
#else
		// Hide panel.
		//	panel->hide();
#endif
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
				throw Common::FraDIAException(std::string("Library open error: ") + lib.error());

			// Try to retrieve method returning type.
			Base::returnType ret_type;
			ret_type = lib.get<Base::componentType>("returnType");
			if (!ret_type)
				throw Common::FraDIAException(std::string("Can't find returnType() in library: ") + lib.error());
			// Check type.
			/*if (ret_type() != COMPONENT_TYPE)
				throw Common::FraDIAException(filename_ + string(" doesn't contain a component of given type."));*/

			// Try to retrieve method returning component name.
			Base::returnName ret_name;
			ret_name = lib.get<std::string>("returnName");
			if (!ret_name)
				throw Common::FraDIAException(std::string("Can't find returnName() in library: ") + lib.error());
			// Retrieve component name.
			name = ret_name();

			// The rest is "lazy" - retrieve only functors, leave pointers to processor and panel unset.

			// Try to retrieve method returning processor.
			ret_object = lib.get<Base::Component*>("returnComponent");
			if (!ret_object)
				throw Common::FraDIAException(std::string("Can't load ret_object from library: ") + lib.error());

			// Try to retrieve method returning panel.
			ret_panel = lib.get<Base::Panel*>("returnPanel");
			if (!ret_panel)
				throw Common::FraDIAException(std::string("Can't load ret_panel from library: ") + lib.error());

			// Component initialized properly.
			LOG(INFO) << "ComponentFactory: Dynamic library " << filename_ << " containing " << name
					<< " component was properly loaded.\n";
			return true;
		} catch (Common::FraDIAException& ex) {
			LOG(INFO) << "ComponentFactory: " << ex.what() << "\n";
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
