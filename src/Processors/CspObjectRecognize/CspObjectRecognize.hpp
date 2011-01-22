/*!
 * \file CspObjectRecognize.hpp
 * \brief
 * \author mboryn
 * \date 2011-01-22
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

/**
 * \defgroup CspObjectRecognize CspObjectRecognize
 * \ingroup Processors
 *
 * \brief Recognizes objects with CSP graph search method.
 *
 *
 *
 * \par Data streams:
 *
 * \streamin{in_segmentedImage,Types::Segmentation::SegmentedImage}
 * Input segmented image with line segments estimated.
 *
 * \streamout{out_recognizedDrawableContainer,Types::DrawableContainer}
 * Recognized objects to draw.
 *
 *
 * \par Events:
 *
 * \event{recognized}
 * Objects recognized event.
 *
 *
 * \par Event handlers:
 *
 * \handler{onSegmentedImage}
 * Segmented image event handler.
 *
 *
 * \par Properties:
 *
 * \prop{modelsFilename,string,""}
 * Filename of objects database.
 * Example database with three primitive shapes defined:
 * \code
<models>
	<Triangle numberOfVertices="3">
		<drawColor>
			<R>0</R>
			<G>255</G>
			<B>255</B>
		</drawColor>
		<constraints>
			<c0 type="junction" from="0" to="1"></c0>
			<c1 type="junction" from="1" to="2"></c1>
			<c2 type="junction" from="2" to="0"></c2>
		</constraints>
	</Triangle>

	<Quadrilateral numberOfVertices="4">
		<drawColor>
			<R>255</R>
			<G>0</G>
			<B>255</B>
		</drawColor>
		<constraints>
			<c0 type="junction" from="0" to="1"></c0>
			<c1 type="junction" from="1" to="2"></c1>
			<c2 type="junction" from="2" to="3"></c2>
			<c3 type="junction" from="3" to="0"></c3>
		</constraints>
	</Quadrilateral>
	<Pentagon numberOfVertices="5">
		<drawColor>
			<R>255</R>
			<G>255</G>
			<B>0</B>
		</drawColor>
		<constraints>
			<c0 type="junction" from="0" to="1"></c0>
			<c1 type="junction" from="1" to="2"></c1>
			<c2 type="junction" from="2" to="3"></c2>
			<c3 type="junction" from="3" to="4"></c3>
			<c4 type="junction" from="4" to="0"></c4>
		</constraints>
	</Pentagon>

</models>
 * \endcode
 *
 * @{
 *
 * @}
 */

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
	/**
	 * Recognize objects in segmented image.
	 */
	void onSegmentedImage();

	/**
	 * Segmented image input data stream.
	 */
	Base::DataStreamIn <Types::Segmentation::SegmentedImage> in_segmentedImage;

	/**
	 * Segmented image event handler.
	 */
	Base::EventHandler <CspObjectRecognize_Processor> h_onSegmentedImage;

	/**
	 * Recognized objects out data stream.
	 */
	Base::DataStreamOut <Types::DrawableContainer> out_recognizedDrawableContainer;

	/**
	 * Objects recognized event.
	 */
	Base::Event* recognized;

	/**
	 * Shape recognize object.
	 * Performs CSP graph search.
	 */
	boost::shared_ptr<ShapeRecognize> shapeRegognize;

	/**
	 * Object for loading models database.
	 */
	boost::shared_ptr<ModelsFactory> modelsFactory;

	/**
	 * Filename of models database.
	 *
	 */
	Base::Property<std::string> modelsFilename;
};

}//: namespace CspObjectRecognize
}//: namespace Processors


/*
 * Register processor component.
 */
REGISTER_PROCESSOR_COMPONENT("CspObjectRecognize", Processors::CspObjectRecognize::CspObjectRecognize_Processor, Common::Panel_Empty)

#endif /* CSPOBJECTRECOGNIZE_PROCESSOR_HPP_ */
