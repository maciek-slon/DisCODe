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

void* Example_Processor::processData(const void* data_)
{
	cout<<"Example_Processor::processData\n";
}//: process_frame


}//: namespace Example
}//: namespace Processors
