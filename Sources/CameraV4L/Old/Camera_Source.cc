/*
 * Camera_Source.cc
 *
 *  Created on: Apr 17, 2010
 *      Author: mboryn
 */

#include "Camera_Source.h"
#include <iostream>

using namespace std;

namespace Core {

namespace Sources {

Camera_Source::Camera_Source(const string& id_, const string& section_) :
	ISource(id_, section_), distortionCorrectionEnabled(true), distortionCorrectionConfigured(false), mapx(NULL),
			mapy(NULL), correctedImage(NULL)
{
	intrinsicMatrix = cvCreateMat(intrinsicMatrixSize, intrinsicMatrixSize, CV_32F);
	distortionCoeffs = cvCreateMat(distortionCoeffsSize, 1, CV_32F);
}

Camera_Source::Camera_Source(const Camera_Source& o) :
	ISource(o)
{
	throw logic_error("Camera_Source::Camera_Source(): not yet implemented.");

}

Camera_Source::~Camera_Source()
{
	releaseImages();
	cvReleaseMat(&intrinsicMatrix);
	cvReleaseMat(&distortionCoeffs);
}

void Camera_Source::releaseImages()
{
	if (mapx)
		cvReleaseImage(&mapx);
	if (mapy)
		cvReleaseImage(&mapy);
	if (correctedImage)
		cvReleaseImage(&correctedImage);
}

void Camera_Source::initializeUndistortion()
{
	if (!distortionCorrectionConfigured) {
		throw logic_error("Camera_Source::initializeUndistortion(): setDistortionCorrectionParameters() has not been called.");
	}

	correctedImage = cvCreateImage(cvGetSize(image), image->depth, image->nChannels);
	mapx = cvCreateImage(cvGetSize(image), IPL_DEPTH_32F, 1);
	mapy = cvCreateImage(cvGetSize(image), IPL_DEPTH_32F, 1);

	cvInitUndistortMap(intrinsicMatrix, distortionCoeffs, mapx, mapy);
}

bool Camera_Source::isDistortionCorrectionEnabled() const
{
	return distortionCorrectionEnabled;
}
void Camera_Source::setDistortionCorrectionEnabled(bool enabled)
{
	distortionCorrectionEnabled = enabled;
}

bool Camera_Source::isDistortionCorrectionConfigured() const
{
	return distortionCorrectionConfigured;
}
void Camera_Source::setDistortionCorrectionParameters(double intrinsicMatrix[intrinsicMatrixSize][intrinsicMatrixSize], double distortionCoeffs[distortionCoeffsSize])
{
	for (int i = 0; i < intrinsicMatrixSize; ++i) {
		for (int j = 0; j < intrinsicMatrixSize; ++j) {
			CV_MAT_ELEM(*this->intrinsicMatrix, float, i, j) = intrinsicMatrix[i][j];
		}
	}

	for (int i = 0; i < distortionCoeffsSize; ++i) {
		CV_MAT_ELEM(*this->distortionCoeffs, float, i, 0) = distortionCoeffs[i];
	}
	distortionCorrectionConfigured = true;

	if (correctedImage != NULL) {
		cvInitUndistortMap(this->intrinsicMatrix, this->distortionCoeffs, mapx, mapy);
	}
}

void Camera_Source::doCorrection()
{
	if (!distortionCorrectionEnabled || !distortionCorrectionConfigured || image == NULL) {
		return;
	}

	CvSize imSize = cvGetSize(image);
	if (correctedImage == NULL) {
//		cout << "Camera_Source::doCorrection(): initializing\n";
//		cout.flush();
		initializeUndistortion();
	}
	CvSize corrSize = cvGetSize(correctedImage);

	// check for images types mismatch
	if (imSize.height != corrSize.height || imSize.width != corrSize.width || image->nChannels
			!= correctedImage->nChannels || image->depth != correctedImage->depth) {
		// reinitialize if necessary
//		cout << "Camera_Source::doCorrection(): reinitializing\n";
//		cout.flush();
		releaseImages();
		initializeUndistortion();
	}

	cvRemap(image, correctedImage, mapx, mapy);
}

IplImage * Camera_Source::getCorrectedImage()
{
	if (!distortionCorrectionEnabled || !distortionCorrectionConfigured || correctedImage == NULL) {
		return image;
	}
	return correctedImage;
}

} // Sources
} // Core
