/*
 * CvFindChessboardCorners_Processor.hpp
 *
 *  Created on: 16-10-2010
 *      Author: mateusz
 */

#ifndef CVFINDCHESSBOARDCORNERS_PROCESSOR_HPP_
#define CVFINDCHESSBOARDCORNERS_PROCESSOR_HPP_

#include <cv.h>
#include <boost/variant.hpp>


#include "Component_Aux.hpp"
#include "Component.hpp"
#include "Panel_Empty.hpp"

namespace Processors {

namespace CvFindChessboardCorners {

using namespace cv;
using boost::variant;

class CvFindChessboardCorners_Processor: public Base::Component
{
public:
	CvFindChessboardCorners_Processor(const std::string & name = "");
	virtual ~CvFindChessboardCorners_Processor();
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
	void onNewImage();

	Base::EventHandler<CvFindChessboardCorners_Processor> h_onNewImage;
	Base::DataStreamIn<Mat> in_img;
	Base::DataStreamOut<> in_img;

	Size patternSize;
	int findChessboardCornersFlags;
};

} // namespace CvFindChessboardCorners {

} // namespace Processors {

REGISTER_PROCESSOR_COMPONENT("CvFindChessboardCorners", Processors::CvFindChessboardCorners::CvFindChessboardCorners_Processor, Common::Panel_Empty)

#endif /* CVFINDCHESSBOARDCORNERS_PROCESSOR_HPP_ */
