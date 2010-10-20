/*
 * CvFindChessboardCorners_Processor.hpp
 *
 *  Created on: 16-10-2010
 *      Author: mateusz
 */

#ifndef CVFINDCHESSBOARDCORNERS_PROCESSOR_HPP_
#define CVFINDCHESSBOARDCORNERS_PROCESSOR_HPP_

#define CV_NO_BACKWARD_COMPATIBILITY
#include <cv.h>

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "Panel_Empty.hpp"
#include "Objects3D/Chessboard.hpp"
#include "Drawable.hpp"
#include "Timer.hpp"

namespace Processors {

namespace CvFindChessboardCorners {

using namespace cv;

struct CvFindChessboardCornersProps: public Base::Props
{
	Size patternSize;
	float squareSize;
	void load(const ptree & pt)
	{
		LOG(TRACE) << "CvFindChessboardCornersProps::load()\n";
		patternSize.width = pt.get<int>("width");
		patternSize.height = pt.get<int>("height");
		squareSize = pt.get<float>("squareSize");
	}
	void save(ptree & pt)
	{
		LOG(TRACE) << "CvFindChessboardCornersProps::save()\n";
		pt.put("width", patternSize.width);
		pt.put("height", patternSize.height);
		pt.put("squareSize", squareSize);
	}
};

class CvFindChessboardCorners_Processor: public Base::Component
{
public:
	CvFindChessboardCorners_Processor(const std::string & name = "");
	virtual ~CvFindChessboardCorners_Processor();

	Base::Props * getProperties()
	{
		return &props;
	}
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

	Base::EventHandler <CvFindChessboardCorners_Processor> h_onNewImage;
	Base::DataStreamIn <Mat> in_img;
	Base::DataStreamOut <Types::Drawable> out_chessboard;
	Base::Event *chessboardFound;
	Base::Event *chessboardNotFound;

	cv::vector<Point2f> corners;

	int findChessboardCornersFlags;

	Common::Timer timer;

	CvFindChessboardCornersProps props;
};

} // namespace CvFindChessboardCorners {

} // namespace Processors {

REGISTER_PROCESSOR_COMPONENT("CvFindChessboardCorners", Processors::CvFindChessboardCorners::CvFindChessboardCorners_Processor, Common::Panel_Empty)

#endif /* CVFINDCHESSBOARDCORNERS_PROCESSOR_HPP_ */
