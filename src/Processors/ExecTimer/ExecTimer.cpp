/*!
 * \file ExecTimer.cpp
 * \brief Declaration of an example class,
 * responsible for image processing.
 * - methods definitions
 * \author tkornuta
 * \date 11.03.2008
 */

#include <memory>
#include <string>
#include <iostream>

#include "ExecTimer.hpp"

#include <boost/thread.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace Processors {
namespace ExecTimer {

ExecTimer::ExecTimer(const std::string & name) :
	Base::Component(name), alwaysPrintTime("alwaysPrintTime")
{
	LOG(LTRACE) << "Hello ExecTimer\n";

	loops = 0;
	total = 0.0;

	registerProperty(alwaysPrintTime);
}

ExecTimer::~ExecTimer()
{
	LOG(LTRACE) << "Good bye ExecTimer\n";
}

bool ExecTimer::onInit()
{
	LOG(LTRACE) << "ExecTimer::initialize\n";

	h_onStartExec.setup(this, &ExecTimer::onStartExec);
	registerHandler("onStartExec", &h_onStartExec);

	h_onStopExec.setup(this, &ExecTimer::onStopExec);
	registerHandler("onStopExec", &h_onStopExec);

	stopped = registerEvent("stopped");
	started = registerEvent("started");

	return true;
}

bool ExecTimer::onFinish()
{
	LOG(LTRACE) << "ExecTimer::finish\n";

	double spl = total / loops;
	double lps = 1.0 / spl;
	LOG(LNOTICE) << "ExecTimer " << name() << ": " << loops << " in " << total << "s. (" << spl << " spl = " << lps
			<< " lps)";

	return true;
}

bool ExecTimer::onStep()
{
	LOG(LTRACE) << "ExecTimer::step\n";
	return true;
}

bool ExecTimer::onStop()
{
	LOG(LTRACE) << "ExecTimer::stop\n";
	return true;
}

bool ExecTimer::onStart()
{
	LOG(LTRACE) << "ExecTimer::start\n";
	return true;
}

void ExecTimer::onStartExec()
{
	timer.restart();
	started->raise();
}

void ExecTimer::onStopExec()
{
	double elapsed = timer.elapsed();
	total += elapsed;
	++loops;

	stopped->raise();

	if(alwaysPrintTime){
		LOG(LFATAL) << "ExecTimer: elapsed = " << elapsed << " s";
	}
}

}//: namespace ExecTimer
}//: namespace Processors
