/*!
 * \file
 * \brief
 */

#ifndef VISUALSERVOIB_PROCESSOR_PROCESSOR_HPP_
#define VISUALSERVOIB_PROCESSOR_PROCESSOR_HPP_

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "Panel_Empty.hpp"
#include "DataStream.hpp"
#include "Props.hpp"
#include "Mrrocpp_Proxy/IBReading.hpp"

/*!
 * \defgroup VisualServoIB_Processor VisualServoIB_Processor
 * \ingroup Processors
 *
 * A component for image-based visual servoing.
 * Receives object's position in image or information that object has not been found.
 * Given this information creates reading which will be sent to robot control system.
 *
 * \par Data streams:
 *
 * \streamin{in_position,Types::ImagePosition}
 * Position of object in image.
 *
 * \streamout{out_reading,Types::Mrrocpp_Proxy::IBReading}
 * Reading to send to robot control system.
 *
 * \par Events:
 *
 * \event{readingReady}
 * Raised when new reading is ready.
 *
 * \par Event handlers:
 *
 * \handler{onObjectLocated}
 * If object has been located, raise this event.
 * Event handler will read from in_position data stream.
 * New reading will be written to out_reading and readingReady event will be raised.
 *
 * \handler{onObjectNotFound}
 * If object has not been located, raise this event.
 * Event handler will not be read from in_position data stream.
 * New reading will be written to out_reading and readingReady event will be raised.
 *
 * @{
 *
 * @}
 */

namespace Processors {
namespace VisualServoIB {

/*!
 * \class VisualServoIB_Processor
 * \brief VisualServoIB processor class.
 * See \link VisualServoIB_Processor \endlink
 */
class VisualServoIB_Processor: public Base::Component
{
public:
	/*!
	 * Constructor.
	 */
	VisualServoIB_Processor(const std::string & name = "");

	/*!
	 * Destructor
	 */
	virtual ~VisualServoIB_Processor();
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
private:
	void onObjectLocated();
	void onObjectNotFound();

	Base::DataStreamIn<Types::ImagePosition> in_position;
	Base::DataStreamOut<Types::Mrrocpp_Proxy::IBReading> out_reading;

	Base::EventHandler <VisualServoIB_Processor> h_onObjectLocated;
	Base::EventHandler <VisualServoIB_Processor> h_onObjectNotFound;
	Base::Event* readingReady;
};

}//: namespace VisualServoIB
}//: namespace Processors


/*
 * Register processor component.
 */
REGISTER_PROCESSOR_COMPONENT("VisualServoIB", Processors::VisualServoIB::VisualServoIB_Processor, Common::Panel_Empty)

#endif /* VISUALSERVOIB_PROCESSOR_PROCESSOR_HPP_ */
