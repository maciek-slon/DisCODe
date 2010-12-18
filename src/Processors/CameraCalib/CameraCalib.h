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
#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>

#include "Component_Aux.hpp"
#include "Panel_Empty.hpp"
#include "Objects3D/Chessboard.hpp"

namespace Processors {
namespace CameraCalib {

struct CameraCalib_Props: public Base::Props
{
	/**
	 * Specifies whether imagesBaseDirectory is valid.
	 */
	bool saveImages;

	/**
	 * If specified, store all calibration images to this imagesBaseDirectory + "/CameraCalib_DATETIME/".
	 */
	std::string imagesBaseDirectory;

	/**
	 * Where to put calibration results.
	 */
	std::string resultsFilename;

	/**
	 * If true, store every image read from data stream.
	 */
	bool storeOnNewImage;

	/**
	 *
	 */
	cv::Size patternSize;
	/**
	 *
	 */
	float squareSize;
	/**
	 *
	 */
	bool findSubpix;

	virtual void load(const ptree & pt)
	{
		imagesBaseDirectory = pt.get <std::string> ("imagesBaseDirectory");
		saveImages = imagesBaseDirectory.size() > 0;
		resultsFilename = pt.get <std::string> ("resultsFilename");
		patternSize.width = pt.get <int> ("width");
		patternSize.height = pt.get <int> ("height");
		squareSize = pt.get <float> ("squareSize");
		findSubpix = pt.get <bool> ("findSubpix");
		storeOnNewImage = pt.get <bool> ("storeOnNewImage");
	}
	virtual void save(ptree & pt)
	{
		pt.put("imagesBaseDirectory", imagesBaseDirectory);
		pt.put("resultsFilename", resultsFilename);
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
	bool lastImageAlreadySaved;

	std::vector <cv::Point3f> chessboardModelPoints;

	cv::Size imageSize;
	CameraCalib_Props props;

	int findChessboardCornersFlags;

	std::string currentDirectory;

	cv::Mat image;


	std::string getTimeAsString();

	void saveResults(cv::Mat cameraMatrix, cv::Mat distCoeffs, double reprojectionError);

	boost::interprocess::interprocess_mutex eventsMutex;
};

} // namespace CameraCalib

} // namespace Processors

REGISTER_PROCESSOR_COMPONENT("CameraCalib", Processors::CameraCalib::CameraCalib_Processor, Common::Panel_Empty)

#endif /* CAMERACALIB_H_ */
