/*!
 * \file MS_Barcode_Decide.hpp
 * \brief
 * \author mstefanc
 * \date 2010-07-05
 */

#ifndef MS_BARCODE_DECIDE_HPP_
#define MS_BARCODE_DECIDE_HPP_

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "Panel_Empty.hpp"
#include "DataStream.hpp"
#include "Props.hpp"

#include <cv.h>
#include <highgui.h>

#include <vector>

#include "Types/Blobs/BlobResult.hpp"
#include "Types/DrawableContainer.hpp"

namespace Processors {
namespace MS_Barcode {

using namespace cv;

/*!
 * \brief MS_Barcode_Decide properties
 */
struct Props: public Base::Props
{

	double thresh;

	/*!
	 * \copydoc Base::Props::load
	 */
	void load(const ptree & pt)
	{
		thresh = pt.get("thresh", 0.1);
	}

	/*!
	 * \copydoc Base::Props::save
	 */
	void save(ptree & pt)
	{
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
 * \class MS_Barcode_Decide
 * \brief Example processor class.
 */
class MS_Barcode_Decide: public Base::Component
{
public:
	/*!
	 * Constructor.
	 */
	MS_Barcode_Decide(const std::string & name = "");

	/*!
	 * Destructor
	 */
	virtual ~MS_Barcode_Decide();

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
	void onNewImage1();
	void onNewImage2();
	void onNewImage3();
	void onNewImage4();

	/// New image is waiting
	Base::EventHandler <MS_Barcode_Decide> h_onNewImage1;
	Base::EventHandler <MS_Barcode_Decide> h_onNewImage2;
	Base::EventHandler <MS_Barcode_Decide> h_onNewImage3;
	Base::EventHandler <MS_Barcode_Decide> h_onNewImage4;

	/// Event raised, when data is processed
	Base::Event * newImage;

	/// Output image
	Base::DataStreamOut < cv::Mat > out_img;

	/// Input images
	Base::DataStreamIn < cv::Mat > in_img1;
	Base::DataStreamIn < cv::Mat > in_img2;
	Base::DataStreamIn < cv::Mat > in_img3;
	Base::DataStreamIn < cv::Mat > in_img4;

	/// Properties
	Props props;

private:
	cv::Mat out;
	cv::Mat sum;

	int count;
};

}//: namespace MS_Barcode
}//: namespace Processors


/*
 * Register processor component.
 */
REGISTER_PROCESSOR_COMPONENT("MS_Barcode_Decide", Processors::MS_Barcode::MS_Barcode_Decide, Common::Panel_Empty)

#endif /* MS_BARCODE_DECIDE_HPP_ */
