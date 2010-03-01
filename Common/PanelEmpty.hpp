/*!
 * \file PanelEmpty.hpp
 * \brief Empty panel - used for all kernels that doesn't require panel.
 * - class declaration
 * \author tkornuta
 * \date 04.03.2008
 */


#ifndef PANEL_EMPTY_HPP_
#define PANEL_EMPTY_HPP_

#include "Panel.hpp"

namespace Common {

/*!
 * \class PanelEmpty
 * \brief Empty panel - used for all kernels that doesn't require task panel.
 * \author tkornuta
 */
class PanelEmpty: public Base::Panel
{
public:

  /*!
   * Method responsible for creation of panel's content.
   */
  void createContent() {};

};

}//: namespace Common

#endif
