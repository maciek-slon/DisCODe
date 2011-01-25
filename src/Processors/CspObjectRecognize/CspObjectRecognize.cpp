/*!
 * \file
 * \brief
 */

#include <memory>
#include <string>

#include "CspObjectRecognize.hpp"
#include "Common/Logger.hpp"

namespace Processors {
namespace CspObjectRecognize {

using Types::DrawableContainer;

CspObjectRecognize_Processor::CspObjectRecognize_Processor(const std::string & name) :
	Base::Component(name), modelsFilename("modelsFilename")
{
	LOG(LTRACE) << "Hello CspObjectRecognize_Processor\n";
	shapeRegognize = boost::shared_ptr <ShapeRecognize>(new ShapeRecognize);
	modelsFactory = boost::shared_ptr <ModelsFactory>(new ModelsFactory);

	registerProperty(modelsFilename);
}

CspObjectRecognize_Processor::~CspObjectRecognize_Processor()
{
	LOG(LTRACE) << "Good bye CspObjectRecognize_Processor\n";
}

bool CspObjectRecognize_Processor::onInit()
{
	LOG(LTRACE) << "CspObjectRecognize_Processor::initialize\n";

	// Register data streams, events and event handlers HERE!
	registerStream("in_segmentedImage", &in_segmentedImage);

	h_onSegmentedImage.setup(this, &CspObjectRecognize_Processor::onSegmentedImage);
	registerHandler("onSegmentedImage", &h_onSegmentedImage);

	registerStream("out_recognizedDrawableContainer", &out_recognizedDrawableContainer);
	recognized = registerEvent("recognized");

	//read models database
	try {
		modelsFactory->setModelsFilename(modelsFilename);
		shapeRegognize->setModels(modelsFactory->loadModels());
	} catch (exception& e) {
		LOG(LFATAL) << "Error loading models from " << string(modelsFilename) << ": " << e.what() << "\n";
		return false;
	}

	return true;
}

bool CspObjectRecognize_Processor::onFinish()
{
	LOG(LTRACE) << "CspObjectRecognize_Processor::finish\n";

	return true;
}

bool CspObjectRecognize_Processor::onStep()
{
	LOG(LTRACE) << "CspObjectRecognize_Processor::step\n";
	return true;
}

bool CspObjectRecognize_Processor::onStop()
{
	return true;
}

bool CspObjectRecognize_Processor::onStart()
{
	return true;
}

void CspObjectRecognize_Processor::onSegmentedImage()
{
	try {
		LOG(LTRACE) << "CspObjectRecognize_Processor::onSegmentedImage()\n";

		if (in_segmentedImage.empty()) {
			LOG(LWARNING) << "CspObjectRecognize_Processor::onSegmentedImage(): buffer empty.\n";
			return;
		}

		Types::Segmentation::SegmentedImage si = in_segmentedImage.read();

		ObjectInstanceVector instances = shapeRegognize->recognize(si);

		DrawableContainer dc;
		BOOST_FOREACH(boost::shared_ptr<ObjectInstance> inst, instances){
			dc.add(inst->clone());
		}
		out_recognizedDrawableContainer.write(dc);
		recognized->raise();

	} catch (exception& e) {
		LOG(LFATAL) << "CspObjectRecognize_Processor::onSegmentedImage(): exception: " << e.what();
	} catch (const char * e) {
		LOG(LFATAL) << "CspObjectRecognize_Processor::onSegmentedImage(): WTF exception: " << e;
	}

}

}//: namespace CspObjectRecognize
}//: namespace Processors
