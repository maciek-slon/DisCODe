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

	//read models database
	modelsFactory->setModelsFilename(modelsFilename);
	shapeRegognize->setModels(modelsFactory->loadModels());

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
		LOG(LFATAL) << "CspObjectRecognize_Processor::onSegmentedImage()\n";

		if (in_segmentedImage.empty()) {
			LOG(LWARNING) << "CspObjectRecognize_Processor::onSegmentedImage(): buffer empty.\n";
			return;
		}

		Types::Segmentation::SegmentedImage si = in_segmentedImage.read();

		shapeRegognize->recognize(si);

	} catch (exception& e) {
		LOG(LFATAL) << "CspObjectRecognize_Processor::onSegmentedImage(): exception: " << e.what();
	} catch (const char * e) {
		LOG(LFATAL) << "CspObjectRecognize_Processor::onSegmentedImage(): WTF exception: " << e;
	}

}

}//: namespace CspObjectRecognize
}//: namespace Processors
