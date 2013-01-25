/*!
 * \file Panel_Empty.hpp
 * \brief Empty panel - used for all components that doesn't require panel.
 * - class declaration
 * \author tkornuta
 * \date 04.03.2008
 */

#warning "Panel_Empty class is deprecated. Please use REGISTER_COMPONENT macros instead of REGISTER_XXX_COMPONENT."

#ifndef PANEL_EMPTY_HPP_
#define PANEL_EMPTY_HPP_

#include "Panel.hpp"

namespace Common {

/*!
 * \class Panel_Empty
 * \brief Empty panel - used for all components that doesn't require task panel.
 * \author tkornuta
 */
class Panel_Empty: public Base::Panel
{
public:

	/*!
	* Method responsible for creation of panel's content.
	*/
	void createContent() {
		cout << "Panel_Empty::createContent()\n";
	}

} __attribute__ ((deprecated));

}//: namespace Common

#endif
