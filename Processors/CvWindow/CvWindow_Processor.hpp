/*!
 * \file CvWindow_Processor.hpp
 * \brief Declaration of an example class, responsible for image processing.
 * \author tkornuta
 * \date 11.03.2008
 */

#ifndef CVWINDOW_PROCESSOR_HPP_
#define CVWINDOW_PROCESSOR_HPP_

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "Panel_Empty.hpp"
#include "DataStream.hpp"
#include "Props.hpp"
#include "Logger.hpp"

#include <cv.h>
#include <highgui.h>

/**
 * \defgroup CvWindow CvWindow
 * \ingroup Processors
 *
 * Creates a window and displays the image
 *
 *
 *
 * \par Data streams:
 *
 * \streamin{in_img,cv::Mat}
 * Input image
 *
 *
 * \par Event handlers:
 *
 * \handler{onNewImage}
 * New image arrived
 *
 *
 * \par Properties:
 *
 * \prop{title,string,"video"}
 * Window caption
 *
 *
 * \see http://opencv.willowgarage.com/documentation/cpp/user_interface.html#namedWindow
 * \see http://opencv.willowgarage.com/documentation/cpp/user_interface.html#cv-imshow
 * @{
 *
 * @}
 */

namespace Processors {
namespace CvWindow {

using namespace cv;

/*!
 * \brief CvWindow properties
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
class CvWindow_Processor: public Base::Component
{
public:
	/*!
	 * Constructor.
	 */
	CvWindow_Processor(const std::string & name = "");

	/*!
	 * Destructor
	 */
	virtual ~CvWindow_Processor();

	/*!
	 * Return window properties
	 */
	Base::Props * getProperties() {
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



	/*!
	 * Event handler function.
	 */
	void onNewImage();

	/// Event handler.
	Base::EventHandler<CvWindow_Processor> h_onNewImage;


	/// Input data stream
	Base::DataStreamIn<Mat> in_img;

	/// Window properties
	WndProps props;
};

}//: namespace CvWindow
}//: namespace Processors


/*
 * Register processor component.
 */
REGISTER_PROCESSOR_COMPONENT("CvWindow", Processors::CvWindow::CvWindow_Processor, Common::Panel_Empty)

#endif /* CVWINDOW_PROCESSOR_HPP_ */

