/*!
 * \file Image_Panel.hpp
 * \brief Panel responsible for image management - class declaration.
 *
 * \author tkornuta
 * \date 2009-11-08
 */

#ifndef IMAGE_PANEL_HPP_
#define IMAGE_PANEL_HPP_

#include "Panel.hpp"

namespace Sources {
namespace Image {

/*!
 * \class Image_Panel
 * \brief Panel responsible for image management.
 * \author tkornuta
 * \date 2009-11-05
 */
class Image_Panel: public Base::Panel
{
public:
	/*!
	 * Required definition of parametrized constructor.
	 */
	Image_Panel();

	/*!
	 * Destructor.
	 */
	virtual ~Image_Panel();

	/*!
	 * Method responsible for creation of panel's content.
	 */
	void createContent();

};

}//: namespace Image
}//: namespace Sources

#endif /* IMAGE_PANEL_HPP_ */
