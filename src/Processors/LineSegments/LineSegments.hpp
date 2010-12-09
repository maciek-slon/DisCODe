/*!
 * \file
 * \brief
 */

#ifndef LINESEGMENTS_PROCESSOR_HPP_
#define LINESEGMENTS_PROCESSOR_HPP_

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "Panel_Empty.hpp"
#include "DataStream.hpp"
#include "Props.hpp"

#include "../GrayImageSegmentation/SegmentedImage.hpp"
#include "DrawableContainer.hpp"

namespace Processors {
namespace LineSegments {

/*!
 * \brief LineSegments properties
 */
struct LineSegments_Props: public Base::Props
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
 * \class LineSegments_Processor
 * \brief LineSegments processor class.
 */
class LineSegments_Processor: public Base::Component
{
public:
	/*!
	 * Constructor.
	 */
	LineSegments_Processor(const std::string & name = "");

	/*!
	 * Destructor
	 */
	virtual ~LineSegments_Processor();

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

	void onEdgesDetected();

	/// Properties
	LineSegments_Props props;

	Base::EventHandler <LineSegments_Processor> h_onEdgesDetected;
	Base::DataStreamIn <Types::Segmentation::SegmentedImage> in_edgesDetected;
	Base::DataStreamOut <Types::Segmentation::SegmentedImage> out_lineSegmentsEstimated;
	Base::DataStreamOut <Types::DrawableContainer> out_lineSegments;
	Base::Event* lineSegmentsEstimated;
};

}//: namespace LineSegments
}//: namespace Processors


/*
 * Register processor component.
 */
REGISTER_PROCESSOR_COMPONENT("LineSegments", Processors::LineSegments::LineSegments_Processor, Common::Panel_Empty)

#endif /* LINESEGMENTS_PROCESSOR_HPP_ */
