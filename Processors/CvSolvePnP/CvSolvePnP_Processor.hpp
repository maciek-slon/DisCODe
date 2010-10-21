/*
 * CvSolvePnP_Processor.hpp
 *
 *  Created on: 18-10-2010
 *      Author: mateusz
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
		LOG(TRACE) << "loading camera parameters.\n";
		boost::numeric::ublas::matrix <double> cameraMatrixUblas = str2mat(pt.get <std::string> ("cameraMatrix"), 3, 3);
		cameraMatrix = cv::Mat_<double>(3, 3);
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				cameraMatrix(i, j) = cameraMatrixUblas(i, j);
				LOG(DEBUG) << "cameraMatrix(" << i << ", " << j << "): " << cameraMatrix(i, j) << endl;
			}
		}

		boost::numeric::ublas::matrix <double> distCoeffsUblas = str2mat(pt.get <std::string> ("distCoeffs"), 1, 5);
		distCoeffs = cv::Mat_<double>(1, 5);
		for (int j = 0; j < 5; ++j) {
			distCoeffs(0, j) = distCoeffsUblas(0, j);
			LOG(DEBUG) << "distCoeffs(" << 0 << ", " << j << "): " << distCoeffs(0, j) << endl;
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
	Base::DataStreamOut <Types::Objects3D::Object3D> out_object3d;

	Base::EventHandler <CvSolvePnP_Processor> h_onNewObject3D;
	Base::Event *objectLocated;
};

} // namespace CvSolvePnP

} // namespace Processors

REGISTER_PROCESSOR_COMPONENT("CvSolvePnP", Processors::CvSolvePnP::CvSolvePnP_Processor, Common::Panel_Empty)

#endif /* CVSOLVEPNP_PROCESSOR_HPP_ */
