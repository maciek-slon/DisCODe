/*!
 * \file CvWindow_Sink.hpp
 * \brief
 * \author tkornuta
 * \date 11.03.2008
 */

#ifndef CVWINDOW_SINK_HPP_
#define CVWINDOW_SINK_HPP_

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
 * \ingroup Sinks
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
 *
 *
 * \par Task configuration template:
 *
 * \code
 * <Components>
 *   <Window type="CvWindow" thread="thread_name" group="group_name">
 *     <title>video</title>
 *   </Window>
 * </Components>
 *
 * <Events>
 *   <Event source="Component.Event" destination="Window.onNewImage"/>
 * </Events>
 *
 * <DataStreams>
 *   <Window>
 *     <in_img type="in" group="connection_name"/>
 *   </Window>
 * </DataStreams>
 * \endcode
 *
 * @{
 *
 * @}
 */

namespace Sinks {
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
	 * \copydoc Base::Props::load
	 */
	void load(const ptree & pt) {
		title = pt.get("title", "video");
	}

	/*!
	 * \copydoc Base::Props::save
	 */
	void save(ptree & pt) {
		pt.put("title", title);
	}
};

/*!
 * \class RGBtoHSV_Processor
 * \brief Example processor class.
 */
class CvWindow_Sink: public Base::Component
{
public:
	/*!
	 * Constructor.
	 */
	CvWindow_Sink(const std::string & name = "");

	/*!
	 * Destructor
	 */
	virtual ~CvWindow_Sink();

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
	Base::EventHandler<CvWindow_Sink> h_onNewImage;


	/// Input data stream
	Base::DataStreamIn<Mat> in_img;

	/// Window properties
	WndProps props;
};

}//: namespace CvWindow
}//: namespace Sinks


/*
 * Register processor component.
 */
REGISTER_PROCESSOR_COMPONENT("CvWindow", Sinks::CvWindow::CvWindow_Sink, Common::Panel_Empty)

#endif /* CVWINDOW_SINK_HPP_ */

