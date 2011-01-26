/*
 * AudioPreemphasis.cpp
 *
 *  Created on: Nov 10, 2010
 *      Author: aszymane
 */

#include <memory>
#include <string>

#include "AudioPreemphasis_Processor.hpp"
#include "Logger.hpp"

namespace Processors {
namespace AudioPreemphasis {

AudioPreemphasis_Processor::AudioPreemphasis_Processor(
		const std::string & name) :
	Base::Component(name) {
	LOG(LTRACE) << "Hello AudioPreemphasis_Processor\n";
}

AudioPreemphasis_Processor::~AudioPreemphasis_Processor() {
	LOG(LTRACE) << "Good bye AudioPreemphasis_Processor\n";
}

bool AudioPreemphasis_Processor::onInit() {
	LOG(LTRACE) << "AudioPreemphasis_Processor::initialize\n";

	h_onNewData.setup(this, &AudioPreemphasis_Processor::onNewData);

	registerHandler("onNewData", &h_onNewData);

	registerStream("in_data", &in_data);

	newData = registerEvent("newData");

	registerStream("out_data", &out_data);

	return true;
}

bool AudioPreemphasis_Processor::onFinish() {
	LOG(LTRACE) << "AudioPreemphasis_Processor::finish\n";

	return true;
}

bool AudioPreemphasis_Processor::onStep() {
	LOG(LTRACE) << "AudioPreemphasis_Processor::step\n";

	return true;
}

bool AudioPreemphasis_Processor::onStop() {
	return true;
}

bool AudioPreemphasis_Processor::onStart() {
	return true;
}

void AudioPreemphasis_Processor::onNewData() {
	LOG(LTRACE) << "AudioPreemphasis_Processor::onNewData\n";
	try {
			mat = in_data.read().clone();

			// perform preemphasis filter with given fi
			for(int c=1;c<mat.rows;c++)//channels
				for(int t=1;t<mat.cols;t++)//time
					mat.at<double>(c,t)=mat.at<double>(c,t)-props.fi*mat.at<double>(c,t-1);

			out_data.write(mat);
			newData->raise();
//		}
	} catch (...) {
		LOG(LERROR) << "AudioPreemphasis::onNewData failed\n";
	}

}

}//: namespace AudioPreemphasis
}//: namespace Processors


