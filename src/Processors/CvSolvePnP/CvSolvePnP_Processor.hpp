/*!
 * \file CvSolvePnP_Processor.hpp
 * \brief Position estimation component.
 * \date Oct 20, 2010
 * \author mboryn
 */

#ifndef CVSOLVEPNP_PROCESSOR_HPP_
#define CVSOLVEPNP_PROCESSOR_HPP_

#include <cv.h>

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "Panel_Empty.hpp"
#include "Props.hpp"
#include "EventHandler.hpp"
#include "DataStream.hpp"
#include "Objects3D/Object3D.hpp"
#include "HomogMatrix.hpp"


/**
 * \defgroup CvSolvePnP CvSolvePnP
 * \ingroup Processors
 *
 * Maps object found on input image to 3D object with given object's model.
 *
  * \par Data streams:
 *
 * \streamin{in_object3d,Types::Objects3D::Object3D}
 * Input 3D object.
 *
 * \streamout{out_object3d,Types::Objects3D::Object3D}
 * Output 3D object with estimated pose.
 *
 * \par Events:
 *
 * \event{objectLocated}
 * New object has been located.
 *
 * \par Event handlers:
 *
 * \handler{onNewObject3D}
 * New 3D object arrived.
 *
 * \par Properties:
 *
 * \prop{cameraMatrix,boost::numeric::ublas::matrix <double> 3x3,}
 * Camera matrix.
 * For example:
 * \verbatim
[3,3](
	(647.07647705, 0., 383.50000000),
	(0., 632.76348877, 287.50000000),
	(0, 0, 1)
)
\endverbatim
 *
 * \prop{distCoeffs,boost::numeric::ublas::matrix <double> 1x5,}
 * Distortion correction coefficients.
 * For example:
 * \verbatim
[1,5](
	(-0.51084775, 0.23313555, 4.50637517e-03, -6.86281081e-03, 4.94365674e-03)
)
	\endverbatim
 *
 * \see http://opencv.willowgarage.com/documentation/cpp/camera_calibration_and_3d_reconstruction.html#cv-solvepnp
 * @{
 *
 * @}
 */


namespace Processors {
namespace CvSolvePnP {

struct CvSolvePnPProps: Base::Props
{
	cv::Mat_<double> cameraMatrix;
	cv::Mat_<double> distCoeffs;

	/*!
	 * Load settings
	 *
	 * @param pt root property tree to load settings from
	 */
	virtual void load(const ptree & pt)
	{
		LOG(LTRACE) << "loading camera parameters.\n";
		boost::numeric::ublas::matrix <double> cameraMatrixUblas = str2mat(pt.get <std::string> ("cameraMatrix"), 3, 3);
		cameraMatrix = cv::Mat_<double>(3, 3);
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				cameraMatrix(i, j) = cameraMatrixUblas(i, j);
				LOG(LDEBUG) << "cameraMatrix(" << i << ", " << j << "): " << cameraMatrix(i, j) << endl;
			}
		}

		boost::numeric::ublas::matrix <double> distCoeffsUblas = str2mat(pt.get <std::string> ("distCoeffs"), 1, 5);
		distCoeffs = cv::Mat_<double>(1, 5);
		for (int j = 0; j < 5; ++j) {
			distCoeffs(0, j) = distCoeffsUblas(0, j);
			LOG(LDEBUG) << "distCoeffs(" << 0 << ", " << j << "): " << distCoeffs(0, j) << endl;
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

class CvSolvePnP_Processor: public Base::Component
{
public:
	CvSolvePnP_Processor(const std::string & n);
	virtual ~CvSolvePnP_Processor();

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

	void onNewObject3D();

	CvSolvePnPProps props;

	Base::DataStreamInPtr <Types::Objects3D::Object3D> in_object3d;
	Base::DataStreamOut <Types::HomogMatrix> out_homogMatrix;

	Base::EventHandler <CvSolvePnP_Processor> h_onNewObject3D;
	Base::Event *objectLocated;
};

} // namespace CvSolvePnP

} // namespace Processors

REGISTER_PROCESSOR_COMPONENT("CvSolvePnP", Processors::CvSolvePnP::CvSolvePnP_Processor, Common::Panel_Empty)

#endif /* CVSOLVEPNP_PROCESSOR_HPP_ */
