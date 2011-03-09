/*!
 * \file
 * \brief
 */

#ifndef EVENTTIMESTAMP_PROCESSOR_HPP_
#define EVENTTIMESTAMP_PROCESSOR_HPP_

#include <ctime>

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "Panel_Empty.hpp"
#include "DataStream.hpp"

namespace Processors {
namespace EventTimestamp {

/*!
 * \class EventTimestamp_Processor
 * \brief EventTimestamp processor class.
 */
class EventTimestamp_Processor: public Base::Component
{
public:
	/*!
	 * Constructor.
	 */
	EventTimestamp_Processor(const std::string & name = "");

	/*!
	 * Destructor
	 */
	virtual ~EventTimestamp_Processor();
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


	void onEvent();

	Base::EventHandler <EventTimestamp_Processor> h_onEvent;
	Base::Event *event;
	Base::DataStreamOut <struct timespec> out_timestamp;

	struct timespec clockResolution;
};

}//: namespace EventTimestamp
}//: namespace Processors


/*
 * Register processor component.
 */
REGISTER_PROCESSOR_COMPONENT("EventTimestamp", Processors::EventTimestamp::EventTimestamp_Processor, Common::Panel_Empty)

#endif /* EVENTTIMESTAMP_PROCESSOR_HPP_ */
