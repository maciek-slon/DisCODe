/*!
 * \file
 * \brief
 */

#ifndef GRAYIMAGESEGMENTATION_PROCESSOR_HPP_
#define GRAYIMAGESEGMENTATION_PROCESSOR_HPP_

#include <cv.h>

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "Panel_Empty.hpp"
#include "DataStream.hpp"
#include "Props.hpp"

namespace Processors {
namespace GrayImageSegmentation {

/*!
 * \brief GrayImageSegmentation properties
 */
struct GrayImageSegmentation_Props: public Base::Props
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
 * \class GrayImageSegmentation_Processor
 * \brief GrayImageSegmentation processor class.
 */
class GrayImageSegmentation_Processor: public Base::Component
{
public:
	/*!
	 * Constructor.
	 */
	GrayImageSegmentation_Processor(const std::string & name = "");

	/*!
	 * Destructor
	 */
	virtual ~GrayImageSegmentation_Processor();

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
	/// Properties
	GrayImageSegmentation_Props props;

	void onNewImage();

	/// Event handler.
	Base::EventHandler <GrayImageSegmentation_Processor> h_onNewImage;

	/// Input data stream
	Base::DataStreamIn <cv::Mat> in_img;

};

}//: namespace GrayImageSegmentation
}//: namespace Processors


/*
 * Register processor component.
 */
REGISTER_PROCESSOR_COMPONENT("GrayImageSegmentation", Processors::GrayImageSegmentation::GrayImageSegmentation_Processor, Common::Panel_Empty)

#endif /* GRAYIMAGESEGMENTATION_PROCESSOR_HPP_ */
