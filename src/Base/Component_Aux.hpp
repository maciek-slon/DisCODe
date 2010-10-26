/*!
 * \file Component_Aux.hpp
 * \brief File containing definitions of component-related functors and macros.
 *
 * \author tkornuta
 * \date Feb 10, 2010
 */

#ifndef COMPONENT_AUX_HPP_
#define COMPONENT_AUX_HPP_

#include <string>

#include "Component.hpp"
#include "Panel.hpp"
#include "Props.hpp"
#include "Event.hpp"
#include "EventHandler.hpp"
#include "DataStreamInterface.hpp"
#include "DataStream.hpp"

namespace Base {

/*!
 * \def DESTROY_DEACTIVED_COMPONENT
 * Destroys objects during component deactivation.
 */
#define DESTROY_DEACTIVED_COMPONENT 1

/*!
 * \enum componentType
 * Type of component (shared library).
 */
enum componentType
{
	COMPONENT_SOURCE, COMPONENT_PROCESSOR
};

/*!
 * \typedef returnType
 * \brief Functor used for retrieving component type.
 * \author tkornuta
 */
typedef Base::componentType (*returnType)(void);

/*!
 * \typedef returnName
 * \brief Functor used for retrieving component name.
 * \author tkornuta
 */
typedef std::string (*returnName)(void);

/*!
 * \typedef returnComponent
 * \brief Functor used for retrieving component object (source, processor, ...) from SO.
 * \author mstefanc
 */
typedef Base::Component* (*returnComponent)(const std::string & name);

/*!
 * \typedef returnPanel
 * \brief Functor used for retrieving Panel object from SO.
 * \author tkornuta
 */
typedef Base::Panel* (*returnPanel)(void);

}//: namespace Base


/*!
 * Use this macro to register a source component. The must be exactly only one macro call for every component (shared library).
 * It adds four basic functions:
 *  - 'returnType', which returns component type (in this case COMPONENT_SOURCE),
 *  - 'returnName', which returns component name (COMPONENT_NAME),
 *  - 'returnSource', which will return a new instance of SOURCE_CLASS_NAME source,
 *  - 'returnPanel', which will return a new instance of PANEL_CLASS_NAME panel.
 *
 * \param COMPONENT_NAME the component name.
 * \param SOURCE_CLASS_NAME the class name of the source you are adding to the library.
 * \param PANEL_CLASS_NAME the class name of the panel you are adding to the library.
 * \author tkornuta
 */
#define REGISTER_SOURCE_COMPONENT(COMPONENT_NAME, SOURCE_CLASS_NAME, PANEL_CLASS_NAME) \
extern "C" { \
  Base::componentType returnType() \
  { \
    return Base::COMPONENT_SOURCE; \
  } \
  std::string returnName() \
  { \
    return COMPONENT_NAME; \
  } \
  Base::Component* returnComponent(const std::string & name) \
  { \
    return new SOURCE_CLASS_NAME(name); \
  } \
  Base::Panel* returnPanel() \
  { \
    return new PANEL_CLASS_NAME(); \
  } \
} /* extern "C" */

/*!
 * Use this macro to register a task component. The must be exactly only one macro call for every component (shared library).
 * It adds four basic functions:
 *  - 'returnType', which returns component type (in this case COMPONENT_PROCESSOR),
 *  - 'returnName', which returns component name (COMPONENT_NAME),
 *  - 'returnProcessor', which will return a new instance of PROCESSOR_CLASS_NAME source,
 *  - 'returnPanel', which will return a new instance of PANEL_CLASS_NAME panel.
 *
 * \param COMPONENT_NAME the component name.
 * \param PROCESSOR_CLASS_NAME the class name of the processor you are adding to the library.
 * \param PANEL_CLASS_NAME the class name of the panel you are adding to the library.
 * \author tkornuta
 */
#define REGISTER_PROCESSOR_COMPONENT(COMPONENT_NAME, PROCESSOR_CLASS_NAME, PANEL_CLASS_NAME) \
extern "C" { \
  Base::componentType returnType() \
  { \
    return Base::COMPONENT_PROCESSOR; \
  } \
  std::string returnName() \
  { \
    return COMPONENT_NAME; \
  } \
  Base::Component* returnComponent(const std::string & name) \
  { \
    return new PROCESSOR_CLASS_NAME(name); \
  } \
  Base::Panel* returnPanel() \
  { \
    return new PANEL_CLASS_NAME(); \
  } \
} /* extern "C" */

#endif /* COMPONENT_AUX_HPP_ */
