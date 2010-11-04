/*!
 * \file
 * \brief
 */

#include <memory>
#include <string>

#include "Example.hpp"
#include "Common/Logger.hpp"

namespace Processors {
namespace Example {

Example_Processor::Example_Processor(const std::string & name) : Base::Component(name)
{
	LOG(LTRACE) << "Hello Example_Processor\n";
}

Example_Processor::~Example_Processor()
{
	LOG(LTRACE) << "Good bye Example_Processor\n";
}

bool Example_Processor::onInit()
{
	LOG(LTRACE) << "Example_Processor::initialize\n";

	// Register data streams, events and event handlers HERE!

	return true;
}

bool Example_Processor::onFinish()
{
	LOG(LTRACE) << "Example_Processor::finish\n";

	return true;
}

bool Example_Processor::onStep()
{
	LOG(LTRACE) << "Example_Processor::step\n";
	return true;
}

bool Example_Processor::onStop()
{
	return true;
}

bool Example_Processor::onStart()
{
	return true;
}

}//: namespace Example
}//: namespace Processors
