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

#include "DataProcessor.hpp"
#include "DataSource.hpp"
#include "Kernel.hpp"
#include "State.hpp"
#include "Panel.hpp"

namespace Base {


/*!
 * \def DESTROY_DEACTIVED_KERNEL
 * Destroys objects during kernel deactivation.
 */
#define DESTROY_DEACTIVED_KERNEL 1

/*!
 * \enum kernelType
 * Type of kernel (shared library).
 */
enum kernelType
{
	KERNEL_SOURCE, KERNEL_PROCESSOR
};


/*!
 * \typedef returnType
 * \brief Functor used for retrieving kernel type.
 * \author tkornuta
 */
typedef Base::kernelType (*returnType)(void);

/*!
 * \typedef returnName
 * \brief Functor used for retrieving kernel name.
 * \author tkornuta
 */
typedef std::string (*returnName)(void);

/*!
 * \typedef returnSource
 * \brief Functor used for retrieving source object from SO.
 * \author tkornuta
 */
typedef Base::DataSource* (*returnSource)(void);

/*!
 * \typedef returnProcessor
 * \brief Functor used for retrieving data processor object from SO.
 * \author tkornuta
 */
typedef Base::DataProcessor* (*returnProcessor)(void);

/*!
 * \typedef returnKernel
 * \brief Functor used for retrieving kernel object (source, processor, ...) from SO.
 * \author mstefanc
 */
typedef Base::Kernel* (*returnKernel)(void);

/*!
 * \typedef returnPanel
 * \brief Functor used for retrieving Panel object from SO.
 * \author tkornuta
 */
typedef Base::Panel* (*returnPanel)(void);

/*!
 * \typedef returnState
 * \brief Functor used for retrieving instance of State object from SO.
 * \date Apr 9, 2010
 * \author tkornuta
 */
typedef Base::XMLTranslatableState* (*returnState)(void);


}//: namespace Base


/*!
 * Use this macro to register a source kernel. The must be exactly only one macro call for every kernel (shared library).
 * It adds four basic functions:
 *  - 'returnType', which returns kernel type (in this case KERNEL_SOURCE),
 *  - 'returnName', which returns kernel name (KERNEL_NAME),
 *  - 'returnSource', which will return a new instance of SOURCE_CLASS_NAME source,
 *  - 'returnPanel', which will return a new instance of PANEL_CLASS_NAME panel.
 *
 * \param KERNEL_NAME the kernel name.
 * \param SOURCE_CLASS_NAME the class name of the source you are adding to the library.
 * \param PANEL_CLASS_NAME the class name of the panel you are adding to the library.
 * \param STATE_INSTANCE instance of the component state.
 * \author tkornuta
 */
#define REGISTER_SOURCE_KERNEL(KERNEL_NAME, SOURCE_CLASS_NAME, PANEL_CLASS_NAME, STATE_INSTANCE) \
extern "C" { \
  Base::kernelType returnType() \
  { \
    return Base::KERNEL_SOURCE; \
  } \
  std::string returnName() \
  { \
    return KERNEL_NAME; \
  } \
  Base::Kernel* returnKernel() \
  { \
    return new SOURCE_CLASS_NAME(); \
  } \
  Base::Panel* returnPanel() \
  { \
    return new PANEL_CLASS_NAME(); \
  } \
  Base::XMLTranslatableState* returnState() \
  { \
    return &STATE_INSTANCE; \
  } \
} /* extern "C" */


/*!
 * Use this macro to register a task kernel. The must be exactly only one macro call for every kernel (shared library).
 * It adds four basic functions:
 *  - 'returnType', which returns kernel type (in this case KERNEL_PROCESSOR),
 *  - 'returnName', which returns kernel name (KERNEL_NAME),
 *  - 'returnProcessor', which will return a new instance of PROCESSOR_CLASS_NAME source,
 *  - 'returnPanel', which will return a new instance of PANEL_CLASS_NAME panel.
 *
 * \param KERNEL_NAME the kernel name.
 * \param PROCESSOR_CLASS_NAME the class name of the processor you are adding to the library.
 * \param PANEL_CLASS_NAME the class name of the panel you are adding to the library.
 * \param STATE_INSTANCE instance of the component state.
 * \author tkornuta
 */
#define REGISTER_PROCESSOR_KERNEL(KERNEL_NAME, PROCESSOR_CLASS_NAME, PANEL_CLASS_NAME, STATE_INSTANCE) \
extern "C" { \
  Base::kernelType returnType() \
  { \
    return Base::KERNEL_PROCESSOR; \
  } \
  std::string returnName() \
  { \
    return KERNEL_NAME; \
  } \
  Base::Kernel* returnKernel() \
  { \
    return new PROCESSOR_CLASS_NAME(); \
  } \
  Base::Panel* returnPanel() \
  { \
    return new PANEL_CLASS_NAME(); \
  } \
  Base::XMLTranslatableState* returnState() \
  { \
    return &STATE_INSTANCE; \
  } \
} /* extern "C" */

#endif /* KERNEL_AUX_HPP_ */
