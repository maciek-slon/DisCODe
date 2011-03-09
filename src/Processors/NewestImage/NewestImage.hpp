/*!
 * \file
 * \brief
 */

#ifndef NEWESTIMAGE_PROCESSOR_HPP_
#define NEWESTIMAGE_PROCESSOR_HPP_

#include <cv.h>
#include "Component_Aux.hpp"
#include "Component.hpp"
#include "Panel_Empty.hpp"
#include "DataStream.hpp"

namespace Processors {
namespace NewestImage {

/*!
 * \class NewestImage_Processor
 * \brief NewestImage processor class.
 */
class NewestImage_Processor: public Base::Component
{
public:
	/*!
	 * Constructor.
	 */
	NewestImage_Processor(const std::string & name = "");

	/*!
	 * Destructor
	 */
	virtual ~NewestImage_Processor();
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

	void onNewImage();

	Base::DataStreamIn <cv::Mat, Base::DataStreamBuffer::Newest, Base::Synchronization::Mutex> in_img;
	Base::EventHandler <NewestImage_Processor> h_onNewImage;

	Base::DataStreamOut<cv::Mat> out_img;
	Base::Event * newImage;
};

}//: namespace NewestImage
}//: namespace Processors


/*
 * Register processor component.
 */
REGISTER_PROCESSOR_COMPONENT("NewestImage", Processors::NewestImage::NewestImage_Processor, Common::Panel_Empty)

#endif /* NEWESTIMAGE_PROCESSOR_HPP_ */
