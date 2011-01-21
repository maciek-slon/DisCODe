/*!
 * \file
 * \brief
 */

#ifndef CSPOBJECTRECOGNIZE_PROCESSOR_HPP_
#define CSPOBJECTRECOGNIZE_PROCESSOR_HPP_

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "Panel_Empty.hpp"
#include "DataStream.hpp"
#include "Property.hpp"
#include "../GrayImageSegmentation/SegmentedImage.hpp"
#include "Types/DrawableContainer.hpp"
#include "ShapeRecognize.hpp"
#include "ModelsFactory.hpp"

namespace Processors {
namespace CspObjectRecognize {

/*!
 * \class CspObjectRecognize_Processor
 * \brief CspObjectRecognize processor class.
 */
class CspObjectRecognize_Processor: public Base::Component
{
public:
	/*!
	 * Constructor.
	 */
	CspObjectRecognize_Processor(const std::string & name = "");

	/*!
	 * Destructor
	 */
	virtual ~CspObjectRecognize_Processor();
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
	void onSegmentedImage();
	Base::DataStreamIn <Types::Segmentation::SegmentedImage> in_segmentedImage;
	Base::EventHandler <CspObjectRecognize_Processor> h_onSegmentedImage;

	Base::DataStreamOut <Types::DrawableContainer> out_recognizedDrawableContainer;
	Base::Event* recognized;


	boost::shared_ptr<ShapeRecognize> shapeRegognize;
	boost::shared_ptr<ModelsFactory> modelsFactory;

	Base::Property<std::string> modelsFilename;
};

}//: namespace CspObjectRecognize
}//: namespace Processors


/*
 * Register processor component.
 */
REGISTER_PROCESSOR_COMPONENT("CspObjectRecognize", Processors::CspObjectRecognize::CspObjectRecognize_Processor, Common::Panel_Empty)

#endif /* CSPOBJECTRECOGNIZE_PROCESSOR_HPP_ */
