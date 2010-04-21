/*!
 * \file KernelManager.hpp
 * \brief
 *
 * \author mstefanc
 * \date 2010-02-28
 */

#ifndef KERNELMANAGER_HPP_
#define KERNELMANAGER_HPP_

#include "Base/Singleton.hpp"
#include "Base/Kernel.hpp"

#include <string>

namespace Core {

/*!
 * \class KernelManager
 * \brief
 *
 * \author mstefanc
 */
class KernelManager : public Base::Singleton<KernelManager> {
	/*!
	 * Singleton class must be a friend, because only it can call protected constructor.
	 */
	friend class Base::Singleton <KernelManager> ;

public:
	/*!
	 * Destructor, called only once, when related singleton is destroyed.
	 */
	~KernelManager() {

	}

	/*!
	 * Add new type of kernel to the manager.
	 */
	void addKernelLibrary(const KernelHeader & header);

	/*!
	 * Return kernel with given name.
	 */
	Base::KernelPtr getKernel(const std::string & name);

protected:
	/*!
	 * Protected constructor, called only by the Singleton::init() method.
	 */
	KernelManager() {

	}

};

}

#endif /* KERNELMANAGER_HPP_ */
