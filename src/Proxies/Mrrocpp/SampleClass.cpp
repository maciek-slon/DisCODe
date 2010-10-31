/*
 * SampleClass.cpp
 *
 *  Created on: 31-10-2010
 *      Author: mateusz
 */

#include <cstring>

#include "SampleClass.hpp"

namespace Proxies {

namespace Mrrocpp {

SampleClass::SampleClass()
{
}

SampleClass::SampleClass(int numberOne, int numberTwo, const char *text, const std::string &str) :
	numberOne(numberOne), numberTwo(numberTwo), str(str)
{
	strcpy(this->text, text);
}

SampleClass::~SampleClass()
{
}

} // namespace Mrrocpp

} // namespace Proxies
