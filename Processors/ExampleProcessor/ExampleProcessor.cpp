/*!
 * \file ExampleProcessor.cpp
 * \brief Declaration of an example class, responsible for image processing.
 * \author tkornuta
 * \date 11.03.2008
 */

#ifndef EXAMPLE_PROCESSOR_HPP_
#define EXAMPLE_PROCESSOR_HPP_

#include "Kernel_Aux.hpp"
#include "PanelEmpty.hpp"

/*!
 * \class ExampleProcessor
 * \brief Example processor.
 * \author tkornuta
 */
class ExampleProcessor: public Base::Kernel
{
public:
	/*!
	 * Constructor.
	 */
	ExampleProcessor() {};

	/*!
	 * Processes given frame.
	 */
	void* processData(const void* data_) {};

	bool setup() {
		return true;
	}

	bool clean() {
		return true;
	}
};

/*
 * Register processor kernel.
 */
REGISTER_KERNEL("ExampleProcessor", Base::KERNEL_PROCESSOR, ExampleProcessor, Common::PanelEmpty)

#endif
