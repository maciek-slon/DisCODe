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
	/*!
	 * \copydoc Base::Props::load
	 */
	void load(const ptree & pt)
	{
	}

	/*!
	 * \copydoc Base::Props::save
	 */
	void save(ptree & pt)
	{
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
	void onNewImage();

	/// New image is waiting
	Base::EventHandler <MS_Barcode_Decide> h_onNewImage;


	/*!
	 * Event handler function.
	 */
	void onNewBlobs();

	/// New set of blobs is waiting
	Base::EventHandler <MS_Barcode_Decide> h_onNewBlobs;


	/// Input blobs
	Base::DataStreamIn <Types::Blobs::BlobResult> in_blobs;

	/// Input hue image
	Base::DataStreamIn <cv::Mat> in_hue;

	/// Event raised, when data is processed
	Base::Event * newImage;

	/// Output data stream - list of ellipses around found signs
	Base::DataStreamOut < Types::DrawableContainer > out_signs;

	/// Properties
	Props props;

private:
	cv::Mat hue_img;
	cv::Mat segments;

	bool blobs_ready;
	bool hue_ready;

	Types::Blobs::BlobResult blobs;
};

}//: namespace MS_Barcode
}//: namespace Processors


/*
 * Register processor component.
 */
REGISTER_PROCESSOR_COMPONENT("MS_Barcode_Decide", Processors::MS_Barcode::MS_Barcode_Decide, Common::Panel_Empty)

#endif /* MS_BARCODE_DECIDE_HPP_ */
