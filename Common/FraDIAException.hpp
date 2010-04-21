/*!
 * \file FraDIAException.hpp
 * \brief Class used for exception handling.
 * \date 25.04.2008
 */

#ifndef _FraDIAException_HPP_
#define _FraDIAException_HPP_

#include <string>
#include <exception>


namespace Common
{

/*!
 * \class FraDIAException
 * \brief Class used during the exception handling.
 * \author tkornuta
 */
class FraDIAException : public std::exception
{
public:
  /*!
   * Exception description.
   */
  const std::string description;

public:
  /*!
   * Constructor - creates description on the base of char*.
   */
  FraDIAException(const char* description_) : description(std::string(description_)), std::exception() { }

  /*!
   * Constructor - creates description on the base of string.
   */
  FraDIAException(std::string description_) : description(description_), std::exception() { }

  /*!
   * Property - returns exception description.
   */
  virtual const char* what() const throw() { return description.c_str(); }

  /*!
   * Destructor.
   */
  virtual ~FraDIAException () throw () {}

};

}//: namespace

#endif
