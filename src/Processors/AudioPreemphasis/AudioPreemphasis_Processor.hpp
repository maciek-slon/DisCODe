/*
 * AudioPreemphasis.cpp
 *
 *  Created on: Nov 18, 2010
 *      Author: aszymane
 */

#ifndef AUDIOPREEMPHASIS_H_
#define AUDIOPREEMPHASIS_H_

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "Panel_Empty.hpp"
#include "DataStream.hpp"
#include "Props.hpp"

#include <cv.h>
#include <highgui.h>

#include <sndfile.h>

namespace Processors {
namespace AudioPreemphasis {

/*!
 * \brief AudioPreemphasis properties
 */
struct Props: public Base::Props {

	double fi;

	/*!
	 * \copydoc Base::Props::load
	 */
	void load(const ptree & pt)
	{
		fi = pt.get("fi", 0.95);
	}

	/*!
	 * \copydoc Base::Props::save
	 */
	void save(ptree & pt)
	{
		pt.put("fi", fi);
	}


};

/*!
 * \class AudioPreemphasis_Processor
 * \brief Example processor class.
 */
class AudioPreemphasis_Processor: public Base::Component {
public:
	/*!
	 * Constructor.
	 */
	AudioPreemphasis_Processor(const std::string & name = "");

	/*!
	 * Destructor
	 */
	virtual ~AudioPreemphasis_Processor();

	/*!
	 * Return filter properties
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

	/*!
	 * Event handler function.
	 */
	void onNewData();

	/// Event handler.
	Base::EventHandler<AudioPreemphasis_Processor> h_onNewData;

	/// Input data stream
	Base::DataStreamIn<cv::Mat> in_data;

	/// Event raised, when image is processed
	Base::Event * newData;

	/// Output data stream - processed image
	Base::DataStreamOut<cv::Mat> out_data;

	/// Filter properties
	Props props;

	/// Array with sound data
	cv::Mat mat;
};

}//: namespace AudioPreemphasis
}//: namespace Processors


/*
 * Register processor component.
 */
REGISTER_PROCESSOR_COMPONENT("AudioPreemphasis", Processors::AudioPreemphasis::AudioPreemphasis_Processor, Common::Panel_Empty)

#endif /* AUDIOPREEMPHASIS_H_ */
