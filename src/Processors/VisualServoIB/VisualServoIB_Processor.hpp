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
#include "IBReading.hpp"

namespace Processors {
namespace VisualServoIB {

/*!
 * \brief VisualServoIB_Processor properties
 */
struct VisualServoIB_Props: public Base::Props
{

	/*!
	 * \copydoc Base::Props::load
	 */
	void load(const ptree & pt)
	{

	}

	/*!
	 * \copydoc Base::Props::save
	 */
	void save(ptree & pt)
	{

	}

};

/*!
 * \class VisualServoIB_Processor
 * \brief VisualServoIB processor class.
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

	/*!
	 * Return window properties
	 */
	Base::Props * getProperties()
	{
		return &props;
	}

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

	/// Properties
	VisualServoIB_Props props;
private:
	void onObjectLocated();
	void onObjectNotFound();

	Base::DataStreamIn<Types::ImagePosition> in_position;
	Base::DataStreamOut<IBReading> out_reading;

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
