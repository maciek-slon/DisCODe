/*!
 * \file
 * \brief
 */

#include <memory>
#include <string>

#include "AudioFile_Source.hpp"
#include "Common/Logger.hpp"

namespace Processors {
namespace AudioFile_Source {

AudioFile_Source_Processor::AudioFile_Source_Processor(const std::string & name) : Base::Component(name)
{
	LOG(TRACE) << "Hello AudioFile_Source_Processor\n";
}

AudioFile_Source_Processor::~AudioFile_Source_Processor()
{
	LOG(TRACE) << "Good bye AudioFile_Source_Processor\n";
}

bool AudioFile_Source_Processor::onInit()
{
	LOG(TRACE) << "AudioFile_Source_Processor::initialize\n";

	// Register data streams, events and event handlers HERE!

	return true;
}

bool AudioFile_Source_Processor::onFinish()
{
	LOG(TRACE) << "AudioFile_Source_Processor::finish\n";

	return true;
}

bool AudioFile_Source_Processor::onStep()
{
	LOG(TRACE) << "AudioFile_Source_Processor::step\n";
	return true;
}

bool AudioFile_Source_Processor::onStop()
{
	return true;
}

bool AudioFile_Source_Processor::onStart()
{
	return true;
}

}//: namespace AudioFile_Source
}//: namespace Processors
