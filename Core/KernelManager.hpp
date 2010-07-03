/*!
 * \file KernelManager.hpp
 * \brief File containing the KernelManager class
 *
 * \author tkornuta
 * \date Feb 10, 2010
 */

#ifndef KERNELMANAGER_HPP_
#define KERNELMANAGER_HPP_

#include <string>
#include <iostream>
#include <vector>
#include <map>

#include <boost/foreach.hpp>
#include <boost/ptr_container/ptr_map.hpp>

#include "boost/filesystem.hpp"
using namespace boost::filesystem;

#include "Kernel_Aux.hpp"
#include "FraDIAException.hpp"
#include "SharedLibraryCommon.hpp"
#include "Utils.hpp"
#include "Logger.hpp"
#include "KernelFactory.hpp"


using namespace boost::property_tree;

using namespace std;

namespace Core {

/*!
 * \class KernelManager
 * \brief
 * \author tkornuta
 */
class KernelManager
{
protected:
	/*!
	 * List of kernel factories properly loaded by the manager.
	 */
	boost::ptr_map <string, Core::KernelFactory> kernel_factories;

	/*!
	 * List of created kernels
	 */
	std::map <string, Base::Kernel*> kernels;

public:
	/*!
	 * Constructor
	 */
	KernelManager()
	{
		cout << "KernelManager: Hello private \n";
	}

	/*!
	 * Public destructor.
	 */
	~KernelManager()
	{
		cout << "KernelManager: Goodbye public\n";
	}

	/*!
	 * Method tries to create kernels from all shared libraries loaded from the . directory.
	 */
	void initializeKernelsList()
	{
		// Retrieve node with default settings from configurator.
		//ptree * tmp_node = CONFIGURATOR.returnManagerNode(KERNEL_TYPE);

		// Get filenames.
		vector <string> files = vector <string> ();
		getSOList(".", files);

		// Check number of so's to import.
		if (files.size() == 0) {
			// I think, that throwing here is much to brutal
			//throw Common::FraDIAException(string(MANAGER_NAME)+string("Manager: There are no dynamic libraries in the current directory."));
			LOG(WARNING) << "KernelManager: There are no dynamic libraries in the current directory.\n";
			return;
		}

		// Iterate through so names and add retrieved kernels to list.
		BOOST_FOREACH(string file, files)
		{
			// Create kernel empty "shell".
			KernelFactory* k = new KernelFactory();
			// Try to initialize kernel.
			if (k->lazyInitialize(file))
			{
				// Add kernel to list.
				kernel_factories.insert(k->getName(), k);
			}
			else
				// Delete incorrect kernel.
				delete (k);
		}//: FOREACH

		// Check number of successfully loaded kernels.
		//if (!kernel_factories.size())
		//	throw Common::FraDIAException(string(MANAGER_NAME)+string("Manager: There are no compatible dynamic libraries in current directory."));
		LOG(INFO) << "Found " << kernel_factories.size() << " components\n";
	}

	/*!
	 *
	 */
	void deactivateKernelList() {
		kernel_factories.release();
	}

	/*!
	 *
	 * @param name
	 * @param type
	 * @return
	 */
	Base::Kernel* createKernel(const std::string & name, const std::string & type) {
		if (kernels.count(name) > 0) {
			LOG(WARNING) << "Module " << name << " already created. Returning previous one.\n";
			return kernels[name];
		}

		if (kernel_factories.count(type) < 1) {
			LOG(ERROR) << "Module type " << type << " not found!\n";
			throw Common::FraDIAException("createKernel");
		}

		kernels[name] = kernel_factories[type].create();
		LOG(INFO) << name << " (" << type << ") component created\n";
		return kernels[name];
	}

	/*!
	 *
	 * @param name
	 * @return
	 */
	Base::Kernel * getKernel(const std::string & name) {
		if (kernels.count(name) < 1) {
			LOG(ERROR) << "Module " << name << " can't be found!\n";
			throw Common::FraDIAException("getKernel");
		}

		return kernels[name];
	}

protected:

	/*!
	 *
	 * @param dir_
	 * @param files
	 */
	void getSOList(string dir_, vector <string>& files)
	{
		std::string regexp = "\\w*.";
		regexp += LIB_EXT;

		cout << "LIB_EXT = " LIB_EXT << endl;

		files = Utils::searchFiles(dir_, regexp);
	}

};

}//: namespace Core

#endif /* KERNELMANAGER_HPP_ */
