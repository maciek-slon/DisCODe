/*!
 * \file
 * \brief
 */

#ifndef GRAYCLASSIFICATION_PROCESSOR_HPP_
#define GRAYCLASSIFICATION_PROCESSOR_HPP_

#include <cv.h>

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "Panel_Empty.hpp"
#include "DataStream.hpp"
#include "Props.hpp"

namespace Processors {
namespace GrayClassification {

/*!
 * \brief GrayClassification properties
 */
struct Props: public Base::Props
{

	int sample;

	/*!
	 * \copydoc Base::Props::load
	 */
	void load(const ptree & pt)
	{
		sample = pt.get("sample", 0);
	}

	/*!
	 * \copydoc Base::Props::save
	 */
	void save(ptree & pt)
	{
		pt.put("sample", sample);
	}

};

/*!
 * \class GrayClassification_Processor
 * \brief GrayClassification processor class.
 */
class GrayClassification_Processor: public Base::Component
{
public:
	/*!
	 * Constructor.
	 */
	GrayClassification_Processor(const std::string & name = "");

	/*!
	 * Destructor
	 */
	virtual ~GrayClassification_Processor();

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

private:
	/// Threshold properties
	Props props;

	/*!
	 * Event handler function.
	 */
	void onNewImage();

	/// Event handler.
	Base::EventHandler <GrayClassification_Processor> h_onNewImage;

	/// Input data stream
	Base::DataStreamIn <cv::Mat> in_img;

	/// Event raised, when image is processed
	Base::Event * newImage;

	/// Output data stream - processed image
	Base::DataStreamOut <cv::Mat> out_img;

};

}//: namespace GrayClassification
}//: namespace Processors


/*
 * Register processor component.
 */
REGISTER_PROCESSOR_COMPONENT("GrayClassification", Processors::GrayClassification::GrayClassification_Processor, Common::Panel_Empty)

#endif /* GRAYCLASSIFICATION_PROCESSOR_HPP_ */
