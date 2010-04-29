/*!
 * \file Example_Processor.cpp
 * \brief Declaration of an example class,
 * responsible for image processing.
 * - methods definitions
 * \author tkornuta
 * \date 11.03.2008
 */

#include <memory>
#include <string>
#include <iostream>

#include "Example_Processor.hpp"

namespace Processors {
namespace Example {

Example_Processor::Example_Processor()
{
	cout<<"Hello Example_Processor\n";
}

void Example_Processor::initialize() {
	std::cout << "Example_Processor::initialize\n";
}

void Example_Processor::finish() {
	std::cout << "Example_Processor::finish\n";
}

int Example_Processor::step()
{
	cout<<"Example_Processor::step\n";
	return 0;
}//: process_frame


}//: namespace Example
}//: namespace Processors
