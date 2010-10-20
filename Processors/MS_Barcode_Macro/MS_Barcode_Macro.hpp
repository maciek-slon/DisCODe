/*!
 * \file MS_Barcode_Macro.hpp
 * \brief
 * \author mstefanc
 * \date 2010-07-05
 */

#ifndef MS_BARCODE_MACRO_HPP_
#define MS_BARCODE_MACRO_HPP_

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
 * \brief MS_Barcode_Macro properties
 */
struct Props: public Base::Props
{

	cv::Mat kernel_1;
	cv::Mat kernel_2;

	double thresh;

	int dil0, dil1, dil2, ero;

	/*!
	 * \copydoc Base::Props::load
	 */
	void load(const ptree & pt)
	{
		kernel_1 = str2mat(cv::Size(3,3), pt.get("kernel_1", ""), 1);
		kernel_2 = str2mat(cv::Size(3,3), pt.get("kernel_2", ""), 1);

		thresh = pt.get("thresh", 128.0);

		dil0 = pt.get("dil0", 2);
		dil1 = pt.get("dil1", 3);
		dil2 = pt.get("dil2", 3);
		ero = pt.get("ero", 6);
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
 * \class MS_Barcode_Macro
 * \brief Example processor class.
 */
class MS_Barcode_Macro: public Base::Component
{
public:
	/*!
	 * Constructor.
	 */
	MS_Barcode_Macro(const std::string & name = "");

	/*!
	 * Destructor
	 */
	virtual ~MS_Barcode_Macro();

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

	/// New image is waiting
	Base::EventHandler <MS_Barcode_Macro> h_onNewImage;

	/// Input image
	Base::DataStreamIn <cv::Mat, Base::DataStreamBuffer::Newest> in_img;

	/// Event raised, when data is processed
	Base::Event * newImage;

	/// Output image
	Base::DataStreamOut < cv::Mat > out_img;

	Base::DataStreamOut < cv::Mat > out_sf1;
	Base::DataStreamOut < cv::Mat > out_sfe1;

	Base::DataStreamOut < cv::Mat > out_sf2;
	Base::DataStreamOut < cv::Mat > out_sfe2;

	/// Properties
	Props props;

private:
	cv::Mat sf;
	cv::Mat sf1, sf2, sfe1, sfe2;
};

}//: namespace MS_Barcode
}//: namespace Processors


/*
 * Register processor component.
 */
REGISTER_PROCESSOR_COMPONENT("MS_Barcode_Macro", Processors::MS_Barcode::MS_Barcode_Macro, Common::Panel_Empty)

#endif /* MS_BARCODE_MACRO_HPP_ */

