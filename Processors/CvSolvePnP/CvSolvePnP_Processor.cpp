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

	//	boost::shared_ptr<Types::Objects3D::Object3D> object3D;
	//	boost::shared_ptr<Types::Drawable> d;
	//	d = in_object3d.read();
	//	object3D = boost::dynamic_pointer_cast<Types::Objects3D::Object3D>(d);
	//	if(object3D.get() == NULL){
	//		LOG(TRACE) << "object3D == NULL\n";
	//		return;
	//	}
	boost::shared_ptr <Types::Objects3D::Object3D> object3D = in_object3d.read();

	LOG(TRACE) << "object3D != NULL\n";

	Mat modelPoints(object3D->getModelPoints().size(), 3, CV_32F);

	Mat imagePoints(object3D->getImagePoints().size(), 2, CV_32F);

	for (size_t i = 0; i < object3D->getModelPoints().size(); ++i) {
		modelPoints.at <float> (i, 0) = object3D->getModelPoints()[i].x;
		modelPoints.at <float> (i, 1) = object3D->getModelPoints()[i].y;
		modelPoints.at <float> (i, 2) = object3D->getModelPoints()[i].z;

		imagePoints.at <float> (i, 0) = object3D->getImagePoints()[i].x;
		imagePoints.at <float> (i, 1) = object3D->getImagePoints()[i].y;
	}

	Mat rotationMatrix;

	LOG(DEBUG) << "modelPoints.depth(): " << modelPoints.depth() << endl;
	LOG(DEBUG) << "modelPoints.rows: " << modelPoints.rows << endl;
	LOG(DEBUG) << "modelPoints.cols: " << modelPoints.cols << endl;
	LOG(DEBUG) << "modelPoints.channels(): " << modelPoints.channels() << endl;

	LOG(DEBUG) << "imagePoints.depth(): " << imagePoints.depth() << endl;
	LOG(DEBUG) << "imagePoints.rows: " << imagePoints.rows << endl;
	LOG(DEBUG) << "imagePoints.cols: " << imagePoints.cols << endl;
	LOG(DEBUG) << "imagePoints.channels(): " << imagePoints.channels() << endl;

	LOG(DEBUG) << "props.cameraMatrix.depth(): " << props.cameraMatrix.depth() << endl;
	LOG(DEBUG) << "props.cameraMatrix.rows: " << props.cameraMatrix.rows << endl;
	LOG(DEBUG) << "props.cameraMatrix.cols: " << props.cameraMatrix.cols << endl;
	LOG(DEBUG) << "props.cameraMatrix.channels(): " << props.cameraMatrix.channels() << endl;

	LOG(DEBUG) << "props.distCoeffs.depth(): " << props.distCoeffs.depth() << endl;
	LOG(DEBUG) << "props.distCoeffs.rows: " << props.distCoeffs.rows << endl;
	LOG(DEBUG) << "props.distCoeffs.cols: " << props.distCoeffs.cols << endl;
	LOG(DEBUG) << "props.distCoeffs.channels(): " << props.distCoeffs.channels() << endl;

	for (int i = 0; i < modelPoints.rows; ++i) {
		for (int j = 0; j < modelPoints.cols; ++j) {
			LOG(DEBUG) << "modelPoints(" << i << ", " << j << "): " << modelPoints.at <float> (i, j) << endl;
		}
	}

	for (int i = 0; i < imagePoints.rows; ++i) {
		for (int j = 0; j < imagePoints.cols; ++j) {
			LOG(DEBUG) << "imagePoints(" << i << ", " << j << "): " << imagePoints.at <float> (i, j) << endl;
		}
	}

	Mat rvec = Mat::zeros(3, 1, CV_32F);
	Mat tvec = Mat::zeros(3, 1, CV_32F);

	solvePnP(modelPoints, imagePoints, props.cameraMatrix, props.distCoeffs, rvec, tvec);

	LOG(TRACE) << "after solvePnP()\n";

	for (int i = 0; i < 3; ++i) {
		LOG(DEBUG) << "rvec[i]: " << rvec.at <float> (i, 0) << endl;
	}

	for (int i = 0; i < 3; ++i) {
			LOG(DEBUG) << "tvec[i]: " << tvec.at <float> (i, 0) << endl;
		}

	LOG(TRACE) << "before Rodrigues()\n";
	Rodrigues(rvec, rotationMatrix);
	LOG(TRACE) << "after Rodrigues()\n";

	HomogMatrix hm;

	stringstream ss;

	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			hm.elements[i][j] = rotationMatrix.at <float> (i, j);
			ss << hm.elements[i][j] << "  ";
		}
		hm.elements[i][3] = tvec.at <float> (i, 0);
		ss << hm.elements[i][3] << "\n";
	}
	LOG(DEBUG) << "HomogMatrix:\n" << ss.str() << endl;

}

} // namespace CvSolvePnP

} // namespace Processors
