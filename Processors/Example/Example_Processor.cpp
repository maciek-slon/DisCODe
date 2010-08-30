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

#include <boost/thread.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace Processors {
namespace Example {

Example_Processor::Example_Processor(const std::string & name) : Base::Component(name) {
	LOG(TRACE)<<"Hello Example_Processor\n";
}

Example_Processor::~Example_Processor() {
	LOG(TRACE)<<"Good bye Example_Processor\n";
}

bool Example_Processor::onInit() {
	LOG(TRACE) << "Example_Processor::initialize\n";

	h_onNewImage.setup(this, &Example_Processor::onNewImage);
	registerHandler("onNewImage", &h_onNewImage);

	registerStream("in_delay", &in_delay);

	return true;
}

bool Example_Processor::onFinish() {
	LOG(TRACE) << "Example_Processor::finish\n";

	return true;
}

bool Example_Processor::onStep()
{
	LOG(TRACE)<<"Example_Processor::step\n";
	return true;
}

bool Example_Processor::onStop()
{
	LOG(TRACE)<<"Example_Processor::stop\n";
	return true;
}

bool Example_Processor::onStart()
{
	LOG(TRACE)<<"Example_Processor::start\n";
	return true;
}

void Example_Processor::onNewImage() {
	LOG(TRACE) << "Example_Processor::onNewImage() called!\n";
	int del = in_delay.read();
	boost::this_thread::sleep(boost::posix_time::milliseconds(del));
	LOG(TRACE) << "Example_Processor::onNewImage() handler was " << del << " mseconds long...\n";
}


}//: namespace Example
}//: namespace Processors
