/*!
 * \file
 * \brief
 */

#ifndef VISUALSERVOPB_PROCESSOR_PROCESSOR_HPP_
#define VISUALSERVOPB_PROCESSOR_PROCESSOR_HPP_

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "Panel_Empty.hpp"
#include "DataStream.hpp"
#include "Props.hpp"
#include "PBReading.hpp"

namespace Processors {
namespace VisualServoPB {

/*!
 * \brief VisualServoPB_Processor properties
 */
struct VisualServoPB_Props: public Base::Props
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
 * \class VisualServoPB_Processor
 * \brief VisualServoPB processor class.
 */
class VisualServoPB_Processor: public Base::Component
{
public:
	/*!
	 * Constructor.
	 */
	VisualServoPB_Processor(const std::string & name = "");

	/*!
	 * Destructor
	 */
	virtual ~VisualServoPB_Processor();

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
	VisualServoPB_Props props;
private:
	void onObjectLocated();
	void onObjectNotFound();

	Base::DataStreamIn<Types::HomogMatrix> in_position;
	Base::DataStreamOut<PBReading> out_reading;

	Base::EventHandler <VisualServoPB_Processor> h_onObjectLocated;
	Base::EventHandler <VisualServoPB_Processor> h_onObjectNotFound;
	Base::Event* readingReady;
};

}//: namespace VisualServoPB
}//: namespace Processors


/*
 * Register processor component.
 */
REGISTER_PROCESSOR_COMPONENT("VisualServoPB", Processors::VisualServoPB::VisualServoPB_Processor, Common::Panel_Empty)

#endif /* VISUALSERVOPB_PROCESSOR_PROCESSOR_HPP_ */
