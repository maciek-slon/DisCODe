/*!
 * \file KernelFactory.hpp
 * \brief File containing the KernelFactory template class.
 *
 * \author tkornuta
 * \date Mar 11, 2010
 */

#ifndef KERNELFACTORY_HPP_
#define KERNELFACTORY_HPP_

#include <boost/function.hpp>
#include <boost/utility.hpp>
#include <boost/property_tree/ptree.hpp>

#include <string>

#include "Kernel_Aux.hpp"
#include "SharedLibrary.hpp"

/*!
 * \namespace KernelFactoryAux
 * \brief The KernelFactoryAux namespace contains names used for passing kernel functions and kernel factories names as "Template Non-Type Parameters".
 * \author tkornuta
 */

namespace KernelFactoryAux {

using namespace boost::property_tree;

/*!
 * Name of function returning source from kernel.
 */
char functionName_returnSource[] = "returnSource";

/*!
 * Name of kernel factory producing sources.
 */
char factoryName_SourceFactory[] = "SourceFactory";

/*!
 * Name of function returning data processor from kernel.
 */
char functionName_returnProcessor[] = "returnProcessor";

/*!
 * Name of kernel factory producing data processors.
 */
char factoryName_ProcessorFactory[] = "ProcessorFactory";

}//: namespace KernelFactoryAux


namespace Core {

/*!
 * \class KernelFactory
 * \brief Template factory producing different kernel types.
 * \author tkornuta
 */
template <Base::kernelType KERNEL_TYPE, typename OBJECT_TYPE, typename OBJECT_FUNCTOR, char* FUNCTION_NAME,
		char* FACTORY_NAME>
class KernelFactory: boost::noncopyable
{
private:
	/*!
	 * Kernel name.
	 */
	std::string name;

	/*!
	 * Pointer to the associated dynamic library.
	 */
	Common::SharedLibrary lib;

	/*!
	 * Variable used for storing address of the functor returning created object.
	 */
	OBJECT_FUNCTOR ret_object;

	/*!
	 * Pointer to object.
	 */
	OBJECT_TYPE* object;

	/*!
	 * Variable used for storing address of the functor returning created panel.
	 */
	Base::returnPanel ret_panel;

	/*!
	 * Pointer to panel object.
	 */
	Base::Panel* panel;

	/*!
	 * Pointer to node containing kernel configuration
	 */
	ptree * config_node;

public:
	/*!
	 * Constructor. Resets pointers to objects.
	 */
	KernelFactory()
	{
		// NULL pointers.
		panel = 0;
		object = 0;
		config_node = 0;
		cout << FACTORY_NAME << ": Hello\n";
	}

	/*!
	 * Destructor - closes dl if it was previously opened, deletes objects.
	 */
	virtual ~KernelFactory()
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
		cout << FACTORY_NAME << ": Goodbye\n";
	}

	/*!
	 * Returns kernel name.
	 */
	std::string& getName()
	{
		return name;
	}

	/*!
	 * Return pointer to created object
	 */
	OBJECT_TYPE * getObject() {
		return object;
	}

	/*!
	 * Methods activates kernel - initializes its elements, etc.
	 */
	void activate()
	{
		cout << FACTORY_NAME << ": Lazy activate!\n";
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
		}//: if !object
	}

	/*!
	 * Methods deactivates kernel - deletes its unused objects, etc.
	 */
	void deactivate()
	{
#ifdef DESTROY_DEACTIVED_KERNEL
		// Hide panel.
		//	panel->hide();
		// Destroy objects.
		if (object)
			if (object->getProperties())
				object->getProperties()->save(*config_node);
			delete (object);
		if (panel)
			delete (panel);
		// Set pointers to NULL.
		panel=0;
		object=0;
#else
		// Hide panel.
		//	panel->hide();
#endif
	}

	/*!
	 * Method tries to load kernel information from given dynamic library.
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
			ret_type = lib.get<Base::kernelType>("returnType");
			if (!ret_type)
				throw Common::FraDIAException(std::string("Can't find returnType() in library: ") + lib.error());
			// Check type.
			if (ret_type() != KERNEL_TYPE)
				throw Common::FraDIAException(filename_ + string(" doesn't contain a kernel of given type."));

			// Try to retrieve method returning kernel name.
			Base::returnName ret_name;
			ret_name = lib.get<std::string>("returnName");
			if (!ret_name)
				throw Common::FraDIAException(std::string("Can't find returnName() in library: ") + lib.error());
			// Retrieve kernel name.
			name = ret_name();

			// The rest is "lazy" - retrieve only functors, leave pointers to processor and panel unset.

			// Try to retrieve method returning processor.
			ret_object = lib.get<Base::Kernel*>("returnKernel");
			if (!ret_object)
				throw Common::FraDIAException(std::string("Can't load ret_object from library: ") + lib.error());

			// Try to retrieve method returning panel.
			ret_panel = lib.get<Base::Panel*>("returnPanel");
			if (!ret_panel)
				throw Common::FraDIAException(std::string("Can't load ret_panel from library: ") + lib.error());

			// Kernel initialized properly.
			cout << FACTORY_NAME << ": Dynamic library " << filename_ << " containing " << name
					<< " kernel was properly loaded." << endl;
			return true;
		} catch (Common::FraDIAException& ex) {
			cout << FACTORY_NAME << ": " << ex.what() << endl;
		}
		return false;
	}

	/*!
	 * Set config node associated to this kernel
	 */
	void setConfigNode(ptree * node) {
		config_node = node;
	}

};

/*!
 * \typedef SourceFactory
 * \brief Factory loading source-type kernels from shared objects (so).
 * \author tkornuta
 * \date Mar 13, 2010
 */
typedef Core::KernelFactory <Base::KERNEL_SOURCE, Base::Kernel, Base::returnKernel,
		KernelFactoryAux::functionName_returnSource, KernelFactoryAux::factoryName_SourceFactory> SourceFactory;


/*!
 * \typedef ProcessorFactory
 * \brief Factory loading processor-type kernels from shared objects (so).
 * \author tkornuta
 * \date Mar 13, 2010
 */
typedef Core::KernelFactory <Base::KERNEL_PROCESSOR, Base::Kernel, Base::returnKernel,
		KernelFactoryAux::functionName_returnProcessor, KernelFactoryAux::factoryName_ProcessorFactory>
		ProcessorFactory;


}//: namespace Core

#endif /* KERNELFACTORY_HPP_ */
