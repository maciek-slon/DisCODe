/*!
 * \file CvPixelOperator_Processor.hpp
 * \brief
 * \author mstefanc
 * \date 2010-07-05
 */

#ifndef CVPIXELOPERATOR_PROCESSOR_HPP_
#define CVPIXELOPERATOR_PROCESSOR_HPP_

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "Panel_Empty.hpp"
#include "DataStream.hpp"
#include "Props.hpp"

#include <Types/stream_OpenCV.hpp>

#include <cv.h>
#include <highgui.h>

#include "PixelOperators.hpp"

#include <sstream>

/**
 * \defgroup CvPixelOperator CvPixelOperator
 * \ingroup Processors
 *
 * Convolves an image with the kernel
 *
 *
 *
 * \par Data streams:
 *
 * \streamin{in_img,cv::Mat}
 * Input image
 * \streamout{out_img,cv::Mat}
 * Output image
 *
 *
 * \par Events:
 *
 * \event{newImage}
 * New image is ready
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
 * \prop{size,cv::Size,3 3}
 * Kernel size
 * \prop{kernel,cv::Mat,}
 * Kernel itself
 * \prop{norm,double,1.0}
 * Normalisation factor
 *
 * \see http://opencv.willowgarage.com/documentation/cpp/image_filtering.html#filter2D
 * @{
 *
 * @}
 */

namespace Processors {
namespace CvPixelOperator {

using namespace cv;

/*!
 * \brief Properties
 */
struct Props: public Base::Props
{
	PixelOperator<double> * op;
	ptree tmp;

	void load(const ptree & pt)
	{
		op = str2operator<double>(pt.get("operator", "unit"));

		tmp = pt.get_child("attr");

	}

	/*!
	 * \copydoc Base::Props::save
	 */
	void save(ptree & pt)
	{
	}

	Props() {
		op = NULL;
	}

	~Props() {
		delete op;
	}

private:
	template<typename T>
	PixelOperator<T> * str2operator(const std::string & name) {
		if (name == "tanh")
			return new TanhOperator<double>;

		return NULL;
	}

};

/*!
 * \class CvPixelOperator_Processor
 * \brief Convolves an image with the kernel
 */
class CvPixelOperator_Processor: public Base::Component
{
public:
	/*!
	 * Constructor.
	 */
	CvPixelOperator_Processor(const std::string & name = "");

	/*!
	 * Destructor
	 */
	virtual ~CvPixelOperator_Processor();

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


	/*!
	 * Event handler function.
	 */
	void onNewImage();

	/// Event handler.
	Base::EventHandler <CvPixelOperator_Processor> h_onNewImage;

	/// Input data stream
	Base::DataStreamIn <Mat> in_img;

	/// Event raised, when image is processed
	Base::Event * newImage;

	/// Output data stream - processed image
	Base::DataStreamOut <Mat> out_img;

	/// Threshold properties
	Props props;

};

}//: namespace CvPixelOperator
}//: namespace Processors


/*
 * Register processor component.
 */
REGISTER_PROCESSOR_COMPONENT("CvPixelOperator", Processors::CvPixelOperator::CvPixelOperator_Processor, Common::Panel_Empty)

#endif /* CVPIXELOPERATOR_PROCESSOR_HPP_ */

