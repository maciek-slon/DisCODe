/*!
 * \file CvFilter2D_Processor.hpp
 * \brief
 * \author mstefanc
 * \date 2010-07-05
 */

#ifndef CVFILTER2D_PROCESSOR_HPP_
#define CVFILTER2D_PROCESSOR_HPP_

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "Panel_Empty.hpp"
#include "DataStream.hpp"
#include "Props.hpp"

#include <Types/stream_OpenCV.hpp>

#include <cv.h>
#include <highgui.h>

#include <sstream>

/**
 * \defgroup CvFilter2D CvFilter2D
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
 * \prop{kernel,cv::Mat,&nbsp;}
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
namespace CvFilter2D {

using namespace cv;

/*!
 * \brief Properties
 */
struct Props: public Base::Props
{
	cv::Size size;
	cv::Mat kernel;
	double delta;
	double norm;

	/*!
	 * \copydoc Base::Props::load
	 */
	void load(const ptree & pt)
	{
		size = pt.get("size", cv::Size(3,3));

		norm = pt.get("norm", 1.0);

		kernel = str2mat(size, pt.get("kernel", ""), norm);

		delta = pt.get("delta", 0.0);
	}

	/*!
	 * \copydoc Base::Props::save
	 */
	void save(ptree & pt)
	{
		pt.put("size", size);

		// \todo write kernel (cv::Mat)

		pt.put("delta", delta);

		pt.put("norm", norm);
	}

protected:
	cv::Mat str2mat(cv::Size size, std::string s, double norm) {
		std::stringstream ss;
		cv::Mat mat = cv::Mat::eye(size, CV_32F);
		double val;

		ss << s;

		for (int i = 0; i < size.height; ++i) {
			for (int j = 0; j < size.width; ++j) {
				ss >> val;
				val /= norm;
				mat.at<float>(i,j) = val;
			}
		}

		return mat;
	}

};

/*!
 * \class CvFilter2D_Processor
 * \brief Convolves an image with the kernel
 */
class CvFilter2D_Processor: public Base::Component
{
public:
	/*!
	 * Constructor.
	 */
	CvFilter2D_Processor(const std::string & name = "");

	/*!
	 * Destructor
	 */
	virtual ~CvFilter2D_Processor();

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
	Base::EventHandler <CvFilter2D_Processor> h_onNewImage;

	/// Input data stream
	Base::DataStreamIn <Mat> in_img;

	/// Event raised, when image is processed
	Base::Event * newImage;

	/// Output data stream - processed image
	Base::DataStreamOut <Mat> out_img;

	/// Threshold properties
	Props props;

private:
	cv::Mat tmp;
};

}//: namespace CvFilter2D
}//: namespace Processors


/*
 * Register processor component.
 */
REGISTER_PROCESSOR_COMPONENT("CvFilter2D", Processors::CvFilter2D::CvFilter2D_Processor, Common::Panel_Empty)

#endif /* CVFILTER2D_PROCESSOR_HPP_ */

