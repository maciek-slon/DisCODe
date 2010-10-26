/*!
 * \file
 * \brief
 */

#ifndef EXAMPLE_PROCESSOR_HPP_
#define EXAMPLE_PROCESSOR_HPP_

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "Panel_Empty.hpp"
#include "DataStream.hpp"
#include "Props.hpp"

namespace Processors {
namespace Example {

/*!
 * \brief Example properties
 */
struct Props: public Base::Props
{

	int sample;

	/*!
	 * \copydoc Base::Props::load
	 */
	void load(const ptree & pt)
	{
		sample = pt.get("sample", 0);
	}

	/*!
	 * \copydoc Base::Props::save
	 */
	void save(ptree & pt)
	{
		pt.put("sample", sample);
	}

};

/*!
 * \class Example_Processor
 * \brief Example processor class.
 */
class Example_Processor: public Base::Component
{
public:
	/*!
	 * Constructor.
	 */
	Example_Processor(const std::string & name = "");

	/*!
	 * Destructor
	 */
	virtual ~Example_Processor();

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



	/// Threshold properties
	Props props;
};

}//: namespace Example
}//: namespace Processors


/*
 * Register processor component.
 */
REGISTER_PROCESSOR_COMPONENT("Example", Processors::Example::Example_Processor, Common::Panel_Empty)

#endif /* EXAMPLE_PROCESSOR_HPP_ */
