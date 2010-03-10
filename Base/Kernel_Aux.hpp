/*!
 * \file Kernel_Aux.hpp
 * \brief File containing definitions of kernel-related functors and macros.
 *
 * \author tkornuta
 * \date Feb 10, 2010
 */

#ifndef KERNEL_AUX_HPP_
#define KERNEL_AUX_HPP_

#include <string>

#include <boost/extension/extension.hpp>

#include "Kernel.hpp"
#include "Panel.hpp"

namespace Base {

/**
 * \enum kernelType
 * \brief Type of kernel (shared library).
 */
enum kernelType
{
	KERNEL_SOURCE,    ///< Data source, for example camera
	KERNEL_PROCESSOR, ///< Data processor
	KERNEL_SINK       ///< Data sink, for example movie writer
};


/*!
 * \typedef returnType
 * \brief Type representing function used for retrieving kernel type.
 * \author tkornuta
 */
typedef Base::kernelType (*returnType)(void);

/*!
 * \typedef returnName
 * \brief Type representing function used for retrieving kernel name.
 * \author tkornuta
 */
typedef std::string (*returnName)(void);

/*!
 * \typedef returnPanel
 * \brief Type representing function used for retrieving Panel object from SO.
 * \author tkornuta
 */
typedef Base::Panel* (*returnPanel)(void);

/*!
 * \typedef returnKernel
 * \brief Type representing function used for retrieving kernel object from SO.
 * \author tkornuta
 */
typedef Base::Kernel* (*returnKernel)(void);

}//: namespace Base


/*!
 * Use this macro to register a kernel. There must be exactly only one macro call for every kernel (shared library).
 * It adds four basic functions:
 *  - 'returnType', which returns kernel type (in this case KERNEL_SOURCE),
 *  - 'returnName', which returns kernel name (KERNEL_NAME),
 *  - 'returnKernel', which will return a new instance of SOURCE_CLASS_NAME source,
 *  - 'returnPanel', which will return a new instance of PANEL_CLASS_NAME panel.
 *
 * \param KERNEL_NAME the kernel name.
 * \param KERNEL_TYPE type of kernel
 * \param KERNEL_CLASS_NAME the class name of the kernel you are adding to the library.
 * \param PANEL_CLASS_NAME the class name of the panel you are adding to the library.
 * \author tkornuta
 */
#define REGISTER_KERNEL(KERNEL_NAME, KERNEL_TYPE, KERNEL_CLASS_NAME, PANEL_CLASS_NAME) \
extern "C" { \
  Base::kernelType BOOST_EXTENSION_EXPORT_DECL returnType() \
  { \
    return KERNEL_TYPE; \
  } \
  std::string BOOST_EXTENSION_EXPORT_DECL returnName() \
  { \
    return KERNEL_NAME; \
  } \
  Base::Kernel* BOOST_EXTENSION_EXPORT_DECL returnSource() \
  { \
    return new KERNEL_CLASS_NAME(); \
  } \
  Base::Panel* BOOST_EXTENSION_EXPORT_DECL returnPanel() \
  { \
    return new PANEL_CLASS_NAME(); \
  } \
} /* extern "C" */

#endif /* KERNEL_AUX_HPP_ */
