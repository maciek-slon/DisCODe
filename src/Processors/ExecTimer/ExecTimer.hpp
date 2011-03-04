/*!
 * \file ExecTimer.hpp
 * \brief Simple processor to measure algorithm execution time.
 */

#ifndef ExecTimer_HPP_
#define ExecTimer_HPP_

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "Panel_Empty.hpp"
#include "DataStream.hpp"
#include "Property.hpp"

#include "Timer.hpp"

namespace Processors {
namespace ExecTimer {

/*!
 * \class ExecTimer
 * \brief Example processor class.
 * \author tkornuta
 */
class ExecTimer: public Base::Component
{
public:
	/*!
	 * Constructor.
	 */
	ExecTimer(const std::string & name = "");

	/*!
	 * Destructor
	 */
	virtual ~ExecTimer();

protected:

	/*!
	 * Connects source to given device.
	 */
	bool onInit();

	/*!
	 * Disconnect source from device, closes streams, etc.
	 */
	bool onFinish();

	/*!
	 * Retrieves data from device.
	 */
	bool onStep();

	/*!
	 * Start component
	 */
	bool onStart();

	/*!
	 * Stop component
	 */
	bool onStop();



	/*!
	 * Event handler function.
	 */
	void onStartExec();

	/// Event handler.
	Base::EventHandler<ExecTimer> h_onStartExec;

	/*!
	 * Event handler function.
	 */
	void onStopExec();

	/// Event handler.
	Base::EventHandler<ExecTimer> h_onStopExec;


	Base::Event * stopped;

	Base::Event * started;

private:
	Common::Timer timer;

	int loops;
	double total;

	Base::Property<bool> alwaysPrintTime;
};

}//: namespace ExecTimer
}//: namespace Processors


/*
 * Register processor component.
 */
REGISTER_PROCESSOR_COMPONENT("ExecTimer", Processors::ExecTimer::ExecTimer, Common::Panel_Empty)

#endif

