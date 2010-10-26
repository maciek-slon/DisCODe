/*!
 * \file Panel.hpp
 * \brief Base class for all task panels.
 * - class declaration
 * \author tkornuta
 * \date 06.03.2008
 */

#ifndef _PANEL_HPP_
#define _PANEL_HPP_

#include <iostream>

using namespace std;

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
	Panel()	{
		cout << "Hello abstract Panel\n";
	}

	/*!
	 * Virtual destructor
	 */
	virtual ~Panel() {

	}

	/*!
	 * Method responsible for creating of panel's content - abstract.
	 */
	virtual void createContent() = 0;

};

}//: namespace Base

#endif

