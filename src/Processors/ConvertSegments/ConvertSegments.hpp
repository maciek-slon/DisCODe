/*!
 * \file
 * \brief
 */

#ifndef CONVERTSEGMENTS_PROCESSOR_HPP_
#define CONVERTSEGMENTS_PROCESSOR_HPP_

#include <cv.h>

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "Panel_Empty.hpp"
#include "DataStream.hpp"
#include "Props.hpp"

#include "../GrayImageSegmentation/Segment.hpp"
#include "../GrayImageSegmentation/SegmentedImage.hpp"

namespace Processors {
namespace ConvertSegments {

/*!
 * \brief ConvertSegments properties
 */
struct ConvertSegments_Props: public Base::Props
{
	bool showEdgeImage;
	/*!
	 * \copydoc Base::Props::load
	 */
	void load(const ptree & pt)
	{
		showEdgeImage = pt.get <bool> ("showEdgeImage");
	}

	/*!
	 * \copydoc Base::Props::save
	 */
	void save(ptree & pt)
	{
		pt.put <bool> ("showEdgeImage", showEdgeImage);
	}

};

/*!
 * \class ConvertSegments_Processor
 * \brief ConvertSegments processor class.
 */
class ConvertSegments_Processor: public Base::Component
{
public:
	/*!
	 * Constructor.
	 */
	ConvertSegments_Processor(const std::string & name = "");

	/*!
	 * Destructor
	 */
	virtual ~ConvertSegments_Processor();

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

	/// Properties
	ConvertSegments_Props props;
private:
	void onSegmented();

	Base::DataStreamIn <Types::Segmentation::SegmentedImage> in_segmented;
	Base::EventHandler <ConvertSegments_Processor> h_onSegmented;

	Base::DataStreamOut <cv::Mat> out_img;
	Base::Event* onNewImage;

	static const int colorsSize = 16;
	u_int8_t colors[colorsSize];
};

}//: namespace ConvertSegments
}//: namespace Processors


/*
 * Register processor component.
 */
REGISTER_PROCESSOR_COMPONENT("ConvertSegments", Processors::ConvertSegments::ConvertSegments_Processor, Common::Panel_Empty)

#endif /* CONVERTSEGMENTS_PROCESSOR_HPP_ */
