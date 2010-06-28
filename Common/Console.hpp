/*!
 * \file Console.hpp
 * \brief Console utility functions, like text coloring
 * \author mstefanc
 * \date 2010-06-25
 */
#ifndef CONSOLE_HPP_
#define CONSOLE_HPP_

#include <iostream>

#if defined(WIN32)
#	define WIN32_LEAN_AND_MEAN
#	include <windows.h>
#endif

namespace Utils {

/*!
 * Set console font color to blue.
 */
inline std::ostream& blue(std::ostream &s)
{
#if defined(WIN32)
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdout, FOREGROUND_BLUE);
#else
	s << "\033[34m";
#endif

	return s;
}

/*!
 * Set console font color to green.
 */
inline std::ostream& green(std::ostream &s)
{
	#if defined(WIN32)
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdout, FOREGROUND_GREEN);
#else
	s << "\033[32m";
#endif

    return s;
}

/*!
 * Set console font color to red.
 */
inline std::ostream& red(std::ostream &s)
{
#if defined(WIN32)
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdout, FOREGROUND_RED);
#else
	s << "\033[31m";
#endif

	return s;
}

/*!
 * Set console font color to white.
 */
inline std::ostream& white(std::ostream &s)
{
#if defined(WIN32)
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdout, FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE);
#else
	s << "\033[37m";
#endif

    return s;
}

/*!
 * Set console font color to yellow.
 */
inline std::ostream& yellow(std::ostream &s)
{
#if defined(WIN32)
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdout, FOREGROUND_RED|FOREGROUND_GREEN);
#else
	s << "\033[33m";
#endif

    return s;
}

/*!
 * Set console font color to magenta.
 */
inline std::ostream& magenta(std::ostream &s)
{
#if defined(WIN32)
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdout, FOREGROUND_RED|FOREGROUND_BLUE);
#else
	s << "\033[35m";
#endif

    return s;
}

/*!
 * Set console font color to cyan.
 */
inline std::ostream& cyan(std::ostream &s)
{
#if defined(WIN32)
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdout, FOREGROUND_GREEN|FOREGROUND_BLUE);
#else
	s << "\033[36m";
#endif

    return s;
}

/*!
 * Reset console colors (ie. font color white, background color black).
 */
inline std::ostream& reset(std::ostream &s)
{
	#if defined(WIN32)
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdout, FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE);
#else
	s << "\033[00m";
#endif

    return s;
}

/*!
 * Intense console font color.
 */
inline std::ostream& intense(std::ostream &s)
{
#if defined(WIN32)
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbiInfo;

	if (! GetConsoleScreenBufferInfo(hStdout, &csbiInfo))
		return s;

	SetConsoleTextAttribute(hStdout, csbiInfo.wAttributes | FOREGROUND_INTENSITY);
#else
	s << "\033[1m";
#endif

	return s;
}

} //: namespace Utils

#endif /* CONSOLE_HPP_ */
