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


public:
	/*!
	 * Constructor. Resets pointers to objects.
	 */
	ComponentFactory()
	{
		// NULL pointers.
		object = 0;
		ret_object = NULL;
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

			// Try to retrieve method returning component name.
			Base::returnName ret_name;
			ret_name = lib.get<std::string>("returnName");
			if (!ret_name)
				throw Common::DisCODeException(std::string("Can't find returnName() in library: ") + lib.error());
			// Retrieve component name.
			name = ret_name();

			// The rest is "lazy" - retrieve only functors, leave pointers to processor and not set.

			// Try to retrieve method returning processor.
			ret_object = lib.get<Base::Component*, const std::string &>("returnComponent");
			if (!ret_object)
				throw Common::DisCODeException(std::string("Can't load ret_object from library: ") + lib.error());

			// Component initialized properly.
			LOG(LINFO) << "ComponentFactory: Dynamic library " << filename_ << " containing " << name
					<< " component was properly loaded.";
			return true;
		} catch (Common::DisCODeException& ex) {
			LOG(LINFO) << "ComponentFactory: " << ex.what() << "";
		}
		return false;
	}

};

}//: namespace Core

#endif /* COMPONENTFACTORY_HPP_ */
