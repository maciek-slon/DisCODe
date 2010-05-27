/*
 * Camera_Source.h
 *
 *  Created on: Apr 17, 2010
 *      Author: mboryn
 */

#ifndef CAMERA_SOURCE_H_
#define CAMERA_SOURCE_H_

#include "ISource.h"
#include <stdexcept>

namespace Core {

namespace Sources {

/**
 * General class for camera sources.
 * Introduces image distortion correction.
 * For image distortion correction requires camera intrinsics matrix and distortion coefficients.
 * These parameters can be obtained from cvCalibrateCamera2().
 * Use JK_CameraCalibration task to get these parameters.
 *
 * There is no correction available until you call setDistortionCorrectionParameters().
 * It is also possible to disable image correction by calling setDistortionCorrectionEnabled().
 *
 * To use this image correction, call doCorrection() and getCorrectedImage().
 */
class Camera_Source: public Core::Sources::ISource
{
public:
	/**
	 * Calls ISource::ISource().
	 * @param id_
	 * @param section_
	 */
	Camera_Source(const string& id_, const string& section_);

	Camera_Source(const Camera_Source& o);
	virtual ~Camera_Source();

	/**
	 * Check if DistortionCorrection is Enabled. By default it is enabled.
	 * @return true if DistortionCorrection is Enabled
	 */
	bool isDistortionCorrectionEnabled() const;
	/**
	 * Enable or disable DistortionCorrection.
	 * @param enabled
	 */
	void setDistortionCorrectionEnabled(bool enabled);
	/**
	 * Check if DistortionCorrection has been Configured
	 * @return true if setDistortionCorrectionParameters() has been called
	 */
	bool isDistortionCorrectionConfigured() const;

	/** @see setDistortionCorrectionParameters */
	static const int intrinsicMatrixSize = 3;
	/** @see setDistortionCorrectionParameters */
	static const int distortionCoeffsSize = 5;
	/**
	 * Set parameters for distortion correction.
	 * @param intrinsicMatrix 3x3 matrix from cvCalibrateCamera2()
	 * @param distortionCoeffs 5 coefficients from cvCalibrateCamera2(): k1, k2, p1,p2, k3
	 */
	void
			setDistortionCorrectionParameters(double intrinsicMatrix[intrinsicMatrixSize][intrinsicMatrixSize], double distortionCoeffs[distortionCoeffsSize]);
protected:
	/**
	 * Processes image from ISource::image field and places result in Camera_Source::correctedImage.
	 * Distortion correction takes place only if it is enabled and configured.
	 * @throw logic_error when not configured.
	 */
	void doCorrection();

	/**
	 * Get image after correcting distortions.
	 * @return correctedImage if doCorrection() has been called and correction is enabled, ISource::image otherwise
	 */
	IplImage * getCorrectedImage();
private:

	void initializeUndistortion();

	bool distortionCorrectionEnabled;
	bool distortionCorrectionConfigured;
	CvMat* intrinsicMatrix;
	CvMat* distortionCoeffs;
	IplImage *mapx;
	IplImage *mapy;
	IplImage *correctedImage;

	void releaseImages();
};

}

}

#endif /* CAMERA_SOURCE_H_ */
