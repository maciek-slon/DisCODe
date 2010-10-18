/*
 * CvSolvePnP_Processor.cpp
 *
 *  Created on: 18-10-2010
 *      Author: mateusz
 */

#include "CvSolvePnP_Processor.hpp"
#include "Logger.hpp"

namespace Processors {

namespace CvSolvePnP {

CvSolvePnP_Processor::CvSolvePnP_Processor(const std::string & n) :
	Component(n)
{
	// TODO Auto-generated constructor stub

}

CvSolvePnP_Processor::~CvSolvePnP_Processor()
{
	// TODO Auto-generated destructor stub
}

Base::Props * CvSolvePnP_Processor::getProperties()
{
	return &props;
}

bool CvSolvePnP_Processor::onStart()
{
	return true;
}

bool CvSolvePnP_Processor::onStop()
{
	return true;
}

bool CvSolvePnP_Processor::onInit()
{
	LOG(TRACE) << "CvSolvePnP_Processor::onInit()\n";

	h_onNewObject3D.setup(this, &CvSolvePnP_Processor::onNewObject3D);
	registerHandler("h_onNewObject3D", &h_onNewObject3D);

	registerStream("in_objects3d", &in_object3d);
	registerStream("out_objects3d", &out_object3d);

	return true;
}

bool CvSolvePnP_Processor::onFinish()
{
	return true;
}

bool CvSolvePnP_Processor::onStep()
{
	return true;
}

void CvSolvePnP_Processor::onNewObject3D()
{
	LOG(TRACE) << "CvSolvePnP_Processor::onNewObject3D()\n";
}

} // namespace CvSolvePnP

} // namespace Processors
