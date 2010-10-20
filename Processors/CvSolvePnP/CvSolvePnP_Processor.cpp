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
	registerHandler("onNewObject3D", &h_onNewObject3D);

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

	boost::shared_ptr<Types::Objects3D::Object3D> object3D;
	boost::shared_ptr<Types::Drawable> d;
	d = in_object3d.read();
	object3D = boost::dynamic_pointer_cast<Types::Objects3D::Object3D>(d);
	if(object3D.get() == NULL){
		LOG(TRACE) << "object3D == NULL\n";
		return;
	}

	LOG(TRACE) << "object3D != NULL\n";

}

} // namespace CvSolvePnP

} // namespace Processors
