/*!
 * \file OpenCVWnd_Processor.hpp
 * \brief Declaration of an example class, responsible for image processing.
 * \author tkornuta
 * \date 11.03.2008
 */

#ifndef OPENCVWND_PROCESSOR_HPP_
#define OPENCVWND_PROCESSOR_HPP_

#include "Kernel_Aux.hpp"
#include "Kernel.hpp"
#include "Panel_Empty.hpp"
#include "DataStream.hpp"
#include "Props.hpp"
#include "Logger.hpp"

#include <cv.h>
#include <highgui.h>

namespace Processors {
namespace OpenCVWnd {

using namespace cv;

/*!
 * \brief OpenCVWnd properties
 *
 * WndProps contain window properties, such as title
 */
struct WndProps : public Base::Props {

	std::string title;

	/*!
	 * \copydoc Common::Props::load
	 */
	void load(const ptree & pt) {
		title = pt.get("title", "video");
	}

	/*!
	 * \copydoc Common::Props::save
	 */
	void save(ptree & pt) {
		pt.put("title", title);
	}
};

/*!
 * \class RGBtoHSV_Processor
 * \brief Example processor class.
 */
class OpenCVWnd_Processor: public Base::Kernel
{
public:
	/*!
	 * Constructor.
	 */
	OpenCVWnd_Processor();

	/*!
	 * Destructor
	 */
	~OpenCVWnd_Processor();

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

	/*!
	 * Return window properties
	 */
	Base::Props * getProperties() {
		return &props;
	}

protected:
	/*!
	 * Event handler function.
	 */
	void onNewImage();

	/// Event handler.
	Base::EventHandler<OpenCVWnd_Processor> h_onNewImage;


	/// Input data stream
	Base::DataStreamIn<Mat> in_img;

	/// Window properties
	WndProps props;
};

}//: namespace OpenCVWnd
}//: namespace Processors


/*
 * Register processor kernel.
 */
REGISTER_PROCESSOR_KERNEL("OpenCVWnd", Processors::OpenCVWnd::OpenCVWnd_Processor, Common::Panel_Empty)

#endif /* OPENCVWND_PROCESSOR_HPP_ */

