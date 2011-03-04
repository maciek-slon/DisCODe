/*!
 * \file
 * \brief
 */

#include <memory>
#include <string>

#include "NewestImage.hpp"
#include "Common/Logger.hpp"

namespace Processors {
namespace NewestImage {

NewestImage_Processor::NewestImage_Processor(const std::string & name) :
	Base::Component(name)
{
	LOG(LTRACE) << "Hello NewestImage_Processor\n";
}

NewestImage_Processor::~NewestImage_Processor()
{
	LOG(LTRACE) << "Good bye NewestImage_Processor\n";
}

bool NewestImage_Processor::onInit()
{
	LOG(LTRACE) << "NewestImage_Processor::initialize\n";

	// Register data streams, events and event handlers HERE!
	h_onNewImage.setup(this, &NewestImage_Processor::onNewImage);
	registerHandler("onNewImage", &h_onNewImage);

	registerStream("in_img", &in_img);

	registerStream("out_img", &out_img);
	newImage = registerEvent("newImage");
	return true;
}

bool NewestImage_Processor::onFinish()
{
	LOG(LTRACE) << "NewestImage_Processor::finish\n";

	return true;
}

bool NewestImage_Processor::onStep()
{
	LOG(LTRACE) << "NewestImage_Processor::step\n";
	return true;
}

bool NewestImage_Processor::onStop()
{
	return true;
}

bool NewestImage_Processor::onStart()
{
	return true;
}

void NewestImage_Processor::onNewImage()
{
	if(in_img.empty()){
		return;
	}
	out_img.write(in_img.read().clone());
	newImage->raise();
}

}//: namespace NewestImage
}//: namespace Processors
