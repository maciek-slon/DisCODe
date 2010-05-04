/*!
 * \file Example_Processor.hpp
 * \brief Declaration of an example class, responsible for image processing.
 * \author tkornuta
 * \date 11.03.2008
 */

#ifndef EXAMPLE_PROCESSOR_HPP_
#define EXAMPLE_PROCESSOR_HPP_

#include "Kernel_Aux.hpp"
#include "Kernel.hpp"
#include "Panel_Empty.hpp"
#include "StringState.hpp"
#include "DataStream.hpp"

namespace Processors {
namespace Example {

/*!
 * \class Example_Processor
 * \brief Example processor class.
 * \author tkornuta
 */
class Example_Processor: public Base::Kernel
{
public:
	/*!
	 * Constructor.
	 */
	Example_Processor();

	/*!
	 * Destructor
	 */
	~Example_Processor();

	/*!
	 * Processor initialization
	 */
	void initialize();

	/*!
	 * Release all resources
	 */
	void finish();

	/*!
	 * Processes given frame.
	 */
	int step();

protected:
	/*!
	 * Event handler function.
	 */
	void onNewImage();

	/// Event handler.
	Base::EventHandler<Example_Processor> h_onNewImage;

	/// Input data stream
	Base::DataStreamIn<int> in_delay;
};

}//: namespace Example
}//: namespace Processors


/*
 * Register processor kernel.
 */
REGISTER_PROCESSOR_KERNEL("Example", Processors::Example::Example_Processor, Common::Panel_Empty, Common::StringState::instance())

#endif

