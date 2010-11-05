/*!
 * \file
 * \brief
 */

#ifndef AUDIOFILE_SOURCE_PROCESSOR_HPP_
#define AUDIOFILE_SOURCE_PROCESSOR_HPP_

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "Panel_Empty.hpp"
#include "DataStream.hpp"
#include "Props.hpp"

#include <stdlib.h>
#include <al/al.h>
#include <al/alc.h>
#include <al/alu.h>
#include <al/alut.h>

namespace Processors {
namespace AudioFile_Source {

/*!
 * \brief AudioFile_Source properties
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
 * \class AudioFile_Source_Processor
 * \brief AudioFile_Source processor class.
 */
class AudioFile_Source_Processor: public Base::Component
{
public:
	/*!
	 * Constructor.
	 */
	AudioFile_Source_Processor(const std::string & name = "");

	/*!
	 * Destructor
	 */
	virtual ~AudioFile_Source_Processor();

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



	/// Threshold properties
	Props props;
};

}//: namespace AudioFile_Source
}//: namespace Processors


/*
 * Register processor component.
 */
REGISTER_PROCESSOR_COMPONENT("AudioFile_Source", Processors::AudioFile_Source::AudioFile_Source_Processor, Common::Panel_Empty)

#endif /* AUDIOFILE_SOURCE_PROCESSOR_HPP_ */
