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
	cv::Mat cameraMatrix;
	cv::Mat distCoeffs;

	/*!
	 * Load settings
	 *
	 * @param pt root property tree to load settings from
	 */
	virtual void load(const ptree & pt)
	{
		cameraMatrix = str2mat(cv::Size(3, 3), pt.get<std::string>("cameraMatrix"));
		distCoeffs = str2mat(cv::Size(1, 5), pt.get<std::string>("distCoeffs"));
	}

	/*!
	 * Save settings
	 *
	 * @param pt root property tree to save settings
	 */
	virtual void save(ptree & pt)
	{
	}

private:
	cv::Mat str2mat(cv::Size size, std::string s)
	{
		std::stringstream ss;
		cv::Mat mat = cv::Mat::eye(size, CV_32F);
		double val;

		ss << s;

		for (int i = 0; i < size.height; ++i) {
			for (int j = 0; j < size.width; ++j) {
				ss >> val;
				mat.at <float> (i, j) = val;
			}
		}

		return mat;
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

	Base::DataStreamInPtr <Types::Drawable> in_object3d;
	Base::DataStreamOut <Types::Drawable> out_object3d;

	Base::EventHandler <CvSolvePnP_Processor> h_onNewObject3D;
};

} // namespace CvSolvePnP

} // namespace Processors

REGISTER_PROCESSOR_COMPONENT("CvSolvePnP", Processors::CvSolvePnP::CvSolvePnP_Processor, Common::Panel_Empty)

#endif /* CVSOLVEPNP_PROCESSOR_HPP_ */
