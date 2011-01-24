/*!
 * \file CvUndistort_Processor.hpp
 * \brief Camera distortion correction component.
 * \date Oct 20, 2010
 * \author mboryn
 */

#ifndef CVUNDISTORT_PROCESSOR_HPP_
#define CVUNDISTORT_PROCESSOR_HPP_

#include <cv.h>
#include <boost/shared_ptr.hpp>

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "Panel_Empty.hpp"


/**
 * \defgroup CvUndistort CvUndistort
 * \ingroup Processors
 *
 * \brief Applies distortion correction to the image.
 *
 * \par Data streams:
 *
 * \streamin{in_img,cv::Mat}
 * Input image
 * \streamout{out_img,cv::Mat}
 * Output image
 *
 * \par Events:
 *
 * \event{newImage}
 * New image is ready
 *
 *
 * \par Event handlers:
 *
 * \handler{onNewImage}
 * New image arrived
 *
 * \par Properties:
 *
 * \prop{cameraMatrix,boost::numeric::ublas::matrix \<double\> 3x3,""}
 * Camera matrix.
 * For example:
 *
\code
[3,3](
	(647.07647705, 0, 383.50000000),
	(0, 632.76348877, 287.50000000),
	(0, 0, 1)
)
\endcode
 *
 * \prop{distCoeffs,boost::numeric::ublas::matrix \<double\> 1x5,""}
 * Distortion correction coefficients.
 * For example:
 * \code
[1,5](
	(-0.51084775, 0.23313555, 4.50637517e-03, -6.86281081e-03, 4.94365674e-03)
)
	\endcode
 *
 * \see http://opencv.willowgarage.com/documentation/cpp/camera_calibration_and_3d_reconstruction.html#cv-undistort
 * @{
 *
 * @}
 */

namespace Processors {

namespace CvUndistort {

/**
 * CvUndistort properties.
 */
struct CvUndistortProps : public Base::Props
{
	cv::Mat cameraMatrix;
	cv::Mat distCoeffs;

	/*!
	 * Load settings
	 *
	 * @param pt root property tree to load settings from
	 */
	virtual void load(const ptree & pt)
	{
		LOG(LTRACE) << "loading camera parameters.\n";
		boost::numeric::ublas::matrix <double> cameraMatrixUblas = str2mat(pt.get <std::string> ("cameraMatrix"), 3, 3);
		cameraMatrix = cv::Mat(3, 3, CV_32F);
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				cameraMatrix.at <float> (i, j) = cameraMatrixUblas(i, j);
				LOG(LDEBUG) << "cameraMatrix(" << i << ", " << j << "): " << cameraMatrix.at <float> (i, j) << endl;
			}
		}

		boost::numeric::ublas::matrix <double> distCoeffsUblas = str2mat(pt.get <std::string> ("distCoeffs"), 1, 5);
		distCoeffs = cv::Mat(1, 5, CV_32F);
		for (int j = 0; j < 5; ++j) {
			distCoeffs.at <float> (0, j) = distCoeffsUblas(0, j);
			LOG(LDEBUG) << "distCoeffs(" << 0 << ", " << j << "): " << distCoeffs.at <float> (0, j) << endl;
		}
	}

	/*!
	 * Save settings
	 *
	 * @param pt root property tree to save settings
	 */
	virtual void save(ptree & pt)
	{
	}
};

/**
 * Component for distortion correction.
 */
class CvUndistort_Processor : public Base::Component
{
public:
	CvUndistort_Processor(const std::string& n);
	virtual ~CvUndistort_Processor();

	virtual Base::Props * getProperties();
protected:
	/*!
	 * Method called when component is started
	 * \return true on success
	 */
	virtual bool onStart();

	/*!
	 * Method called when component is stopped
	 * \return true on success
	 */
	virtual bool onStop();

	/*!
	 * Method called when component is initialized
	 * \return true on success
	 */
	virtual bool onInit();

	/*!
	 * Method called when component is finished
	 * \return true on success
	 */
	virtual bool onFinish();

	/*!
	 * Method called when step is called
	 * \return true on success
	 */
	virtual bool onStep();

private:
	/*!
	 * Event handler function.
	 */
	void onNewImage();

	/// Event handler.
	Base::EventHandler <CvUndistort_Processor> h_onNewImage;
	Base::Event *newUndistortedImage;

	Base::DataStreamIn <cv::Mat> in_img;
	Base::DataStreamOut <cv::Mat> out_img;

	cv::Mat map1;
	cv::Mat map2;
	int interpolation;

	CvUndistortProps props;
};

} // namespace CvUndistort

} // namespace Processors

REGISTER_PROCESSOR_COMPONENT("CvUndistort", Processors::CvUndistort::CvUndistort_Processor, Common::Panel_Empty)

#endif /* CVUNDISTORT_PROCESSOR_HPP_ */
