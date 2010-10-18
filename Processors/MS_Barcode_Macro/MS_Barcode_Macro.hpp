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
#include "CvMatTranslator.hpp"

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
		Types::CvMatTranslator tr(cv::Size(3, 3));

		int s = pt.get<int>("kernel_1", "", tr);

		//kernel_1 = str2mat(cv::Size(3,3), pt.get("kernel_1", ""));
		kernel_1 = pt.get<cv::Mat>("kernel_1", "", tr);

		kernel_2 = str2mat(cv::Size(3,3), pt.get("kernel_2", ""));

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

