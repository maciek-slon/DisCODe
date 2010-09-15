/*
 * CameraCalib.h
 *
 *  Created on: 2010-06-21
 *      Author: konradb3
 */

#ifndef CAMERACALIB_H_
#define CAMERACALIB_H_

#include "Kernel_Aux.hpp"
#include "Kernel.hpp"
#include "Panel_Empty.hpp"
#include "DataStream.hpp"
#include "Props.hpp"
#include "Logger.hpp"

#include <cv.h>

namespace Processors {

namespace CameraCalib {

using namespace cv;

class CameraCalib_Processor: public Base::Kernel
{
public:
	CameraCalib_Processor();
	virtual ~CameraCalib_Processor();

	/*!
	 * Processor initialization
	 */
	bool initialize();

	/*!
	 * Release all resources
	 */
	bool finish();

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
	Base::EventHandler<CameraCalib_Processor> h_onNewImage;

	/// Input data stream
	Base::DataStreamIn<Mat> in_img;

private:
	Mat frame;

};

}

}

REGISTER_PROCESSOR_KERNEL("CameraCalib", Processors::CameraCalib::CameraCalib_Processor, Common::Panel_Empty);

#endif /* CAMERACALIB_H_ */
