/*
 * CameraCalib.h
 *
 *  Created on: 2010-06-21
 *      Author: konradb3
 */

#ifndef CAMERACALIB_H_
#define CAMERACALIB_H_

#include <cv.h>
#include <boost/shared_ptr.hpp>

#include "Component_Aux.hpp"
#include "Panel_Empty.hpp"
#include "Objects3D/Chessboard.hpp"

namespace Processors {
namespace CameraCalib {

struct CameraCalib_Props: public Base::Props
{
	std::string outputFilename;

	cv::Size patternSize;
	float squareSize;
	bool findSubpix;

	bool storeOnNewImage;

	virtual void load(const ptree & pt)
	{
		outputFilename = pt.get <std::string> ("outputFilename");
		patternSize.width = pt.get <int> ("width");
		patternSize.height = pt.get <int> ("height");
		squareSize = pt.get <float> ("squareSize");
		findSubpix = pt.get <bool> ("findSubpix");
		storeOnNewImage = pt.get<bool>("storeOnNewImage");
	}
	virtual void save(ptree & pt)
	{
		pt.put("outputFilename", outputFilename);
		pt.put("width", patternSize.width);
		pt.put("height", patternSize.height);
		pt.put("squareSize", squareSize);
		pt.put("findSubpix", findSubpix);
		pt.put("storeOnNewImage", storeOnNewImage);
	}
};

class CameraCalib_Processor: public Base::Component
{
public:
	CameraCalib_Processor(const std::string & name);
	virtual ~CameraCalib_Processor();

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
	void onStoreLastImage();
	void onSequenceEnd();

	void addImageToSet();

	Base::EventHandler <CameraCalib_Processor> h_onNewImage;
	Base::EventHandler <CameraCalib_Processor> h_onStoreLastImage;
	Base::EventHandler <CameraCalib_Processor> h_onSequenceEnd;

	Base::DataStreamIn <cv::Mat> in_img;
	Base::DataStreamOut <Types::Objects3D::Chessboard> out_chessboard;

	/** Raised when chessboard has been located on the image. */
	Base::Event *chessboardFound;
	/** Raised when chessboard has not been located on the image. */
	Base::Event *chessboardNotFound;

	std::vector <std::vector <cv::Point3f> > objectPoints;
	std::vector <std::vector <cv::Point2f> > imagePoints;
	std::vector <cv::Point2f> lastImagePoints;
	std::vector<cv::Point3f> chessboardModelPoints;
	cv::Size imageSize;
	CameraCalib_Props props;

	int findChessboardCornersFlags;

};

} // namespace CameraCalib

} // namespace Processors

REGISTER_PROCESSOR_COMPONENT("CameraCalib", Processors::CameraCalib::CameraCalib_Processor, Common::Panel_Empty)

#endif /* CAMERACALIB_H_ */
