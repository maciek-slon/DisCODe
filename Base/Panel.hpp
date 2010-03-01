/*!
 * \file Panel.hpp
 * \brief Base class for all task panels.
 * - class declaration
 * \author tkornuta
 * \date 06.03.2008
 */

#ifndef PANEL_HPP_
#define PANEL_HPP_

namespace Base {

/*!
 * \class Panel
 * \brief Base of all panels.
 * \author tkornuta
 */
class Panel
{
public:
	/*!
	 * Basic constructor.
	 */
	Panel()
	{

	}

	/*!
	 * Method responsible for creating of panel's content - abstract.
	 */
	virtual void createContent() = 0;

};

}//: namespace Base

#endif
