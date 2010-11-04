/*
 * CvSolvePnP_Processor.cpp
 *
 *  Created on: 18-10-2010
 *      Author: mateusz
 */

#include "CvSolvePnP_Processor.hpp"
#include "Logger.hpp"

#include <cv.h>
#include <sstream>

namespace Processors {

namespace CvSolvePnP {

using namespace std;
using namespace cv;
using namespace boost;
using namespace Types::Objects3D;

CvSolvePnP_Processor::CvSolvePnP_Processor(const std::string & n) :
	Component(n)
{
}

CvSolvePnP_Processor::~CvSolvePnP_Processor()
{
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
	LOG(LTRACE) << "CvSolvePnP_Processor::onInit()\n";

	h_onNewObject3D.setup(this, &CvSolvePnP_Processor::onNewObject3D);
	registerHandler("onNewObject3D", &h_onNewObject3D);

	registerStream("in_object3d", &in_object3d);
	registerStream("out_object3d", &out_object3d);

	objectLocated = registerEvent("objectLocated");

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
	LOG(LTRACE) << "CvSolvePnP_Processor::onNewObject3D()\n";
	boost::shared_ptr <Types::Objects3D::Object3D> object3D = in_object3d.read();
	Mat modelPoints(object3D->getModelPoints());
	Mat imagePoints(object3D->getImagePoints());

	Mat_<double> rvec;
	Mat_<double> tvec;

	solvePnP(modelPoints, imagePoints, props.cameraMatrix, props.distCoeffs, rvec, tvec, false);

	Mat_<double> rotationMatrix;
	Rodrigues(rvec, rotationMatrix);

	HomogMatrix hm;

	stringstream ss;
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			hm.elements[i][j] = rotationMatrix(i, j);
			ss << hm.elements[i][j] << "  ";
		}
		hm.elements[i][3] = tvec(i, 0);
		ss << hm.elements[i][3] << "\n";
	}
	LOG(LDEBUG) << "HomogMatrix:\n" << ss.str() << endl;

	object3D->setPosition(hm);
	out_object3d.write(*object3D);

	objectLocated->raise();
}

} // namespace CvSolvePnP

} // namespace Processors
