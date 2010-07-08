/*!
 * \file FraDIAException.hpp
 * \brief Class used for exception handling.
 * \date 25.04.2008
 */

#ifndef _FraDIAException_HPP_
#define _FraDIAException_HPP_

#include <string>
#include <exception>

#if defined(WIN32)
#else
#	include <execinfo.h>
#	include <signal.h>
#endif

#include <iostream>

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
	* Constructor - creates description on the base of string.
	*/
	FraDIAException(const std::string & description_) : std::exception(), description(description_) {
	#if defined(WIN32)
		std::cout << "Backtrace (Win32):\n";
	#else
		std::cout << "Backtrace (*nix):\n";
		void * array[25];
		int nSize = backtrace(array, 25);
		char ** symbols = backtrace_symbols(array, nSize);

		for (int i = 0; i < nSize; i++)
		{
			std::cout << symbols[i] << std::endl;
		}

		free(symbols);
	#endif
	}

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
