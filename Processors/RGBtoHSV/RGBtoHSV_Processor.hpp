/*!
 * \file RGBtoHSV_Processor.hpp
 * \brief Declaration of an example class, responsible for image processing.
 * \author tkornuta
 * \date 11.03.2008
 */

#ifndef RGBTOHSV_PROCESSOR_HPP_
#define RGBTOHSV_PROCESSOR_HPP_

#include "Kernel_Aux.hpp"
#include "Kernel.hpp"
#include "Panel_Empty.hpp"
#include "StringState.hpp"
#include "DataStream.hpp"

#include <cv.h>
#include <highgui.h>

namespace Processors {
namespace RGBtoHSV {

/*!
 * \class RGBtoHSV_Processor
 * \brief Example processor class.
 */
class RGBtoHSV_Processor: public Base::Kernel
{
public:
	/*!
	 * Constructor.
	 */
	RGBtoHSV_Processor();

	/*!
	 * Destructor
	 */
	~RGBtoHSV_Processor();

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
	Base::EventHandler<RGBtoHSV_Processor> h_onNewImage;

	/// Input data stream
	Base::DataStreamIn<IplImage *> in_img;
};

}//: namespace RGBtoHSV
}//: namespace Processors


/*
 * Register processor kernel.
 */
REGISTER_PROCESSOR_KERNEL("RGBtoHSV", Processors::RGBtoHSV::RGBtoHSV_Processor, Common::Panel_Empty, Common::StringState::instance())

#endif /* RGBTOHSV_PROCESSOR_HPP_ */

