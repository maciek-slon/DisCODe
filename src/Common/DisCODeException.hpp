/*!
 * \file DisCODeException.hpp
 * \brief Class used for exception handling.
 * \date 25.04.2008
 */

#ifndef _DisCODeException_HPP_
#define _DisCODeException_HPP_

#include <string>
#include <exception>
#include <iostream>

#if defined(WIN32)
#else
#	include <execinfo.h>
#	include <stdlib.h>
#endif

namespace Common
{

/*!
 * \class DisCODeException
 * \brief Class used during the exception handling.
 * \author tkornuta
 */
class DisCODeException : public std::exception
{
public:
	/*!
	* Exception description.
	*/
	const std::string description;

	/*!
	 *
	 */
	#if defined(WIN32)

	#else
		int nSize;
		char ** symbols;
	#endif

public:

	/*!
	* Constructor - creates description on the base of string.
	*/
	DisCODeException(const std::string & description_) : std::exception(), description(description_) {
	#if defined(WIN32)
	#else
		void * array[25];
		nSize = backtrace(array, 25);
		symbols = backtrace_symbols(array, nSize);
	#endif
	}

	void printStackTrace() {
	#if defined(WIN32)
		std::cout << "Backtrace (Win32):\n";
		std::cout << "NOT IMPLEMENTED\n";
	#else
		std::cout << "Backtrace (*nix):\n";
		for (int i = 0; i < nSize; i++)
		{
			std::cout << symbols[i] << std::endl;
		}
	#endif
	}

	/*!
	* Property - returns exception description.
	*/
	virtual const char* what() const throw() { return description.c_str(); }

	/*!
	* Destructor.
	*/
	virtual ~DisCODeException () throw () {
	#if defined(WIN32)

	#else
		free(symbols);
	#endif
	}

};

}//: namespace

#endif
