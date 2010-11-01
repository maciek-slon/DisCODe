/*!
 * \file CvPixelOperator_Processor.hpp
 * \brief
 * \author mstefanc
 * \date 2010-07-05
 */

#ifndef CVPIXELOPERATOR_PROCESSOR_HPP_
#define CVPIXELOPERATOR_PROCESSOR_HPP_

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "Panel_Empty.hpp"
#include "DataStream.hpp"
#include "Props.hpp"

#include <Types/stream_OpenCV.hpp>

#include <cv.h>
#include <highgui.h>

#include "PixelOperators.hpp"

#include <sstream>



/**
 * \defgroup CvPixelOperator CvPixelOperator
 * \ingroup Processors
 *
 * \brief Per pixel operations on image.
 *
 * This processor allows to do bitwise operations with constant,
 * non-linear functions etc.
 *
 *
 *
 * \par Data streams:
 *
 * \streamin{in_img,cv::Mat}
 * Input image
 * \streamout{out_img,cv::Mat}
 * Output image
 *
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
 *
 * \par Properties:
 *
 * \prop{operator,string,"unit"}
 * Operator to be used. Possible values (with corresponding settings) are listed below.
 *
 * \par Possible operators:
 *
 * - \b unit - identity operator: \f$ val_{out} = val_{in} \f$
 * - \b tanh - hiperbolic tangent \f$ val_{out} = tanh(\sigma \cdot (val_{in}-mean)) \f$
 * \prop{sigma,double,0.1}
 * \prop{mean,double,0.0}
 * - \b and - bitwise conjunction \f$ val_{out} = val_{in} \wedge mask \f$
 * \prop{mask,int,0xFF}
 * - \b or - bitwise alternative  \f$ val_{out} = val_{in} \vee mask \f$
 * \prop{mask,int,0xFF}
 *
 * @{
 *
 * @}
 */



namespace Processors {
namespace CvPixelOperator {

using namespace cv;

/*!
 * \brief Properties
 */
struct Props: public Base::Props
{
	PixelOperator<uint8_t> * op;
	std::string type;

	void load(const ptree & pt)
	{
		op = str2operator<uint8_t>(pt.get("operator", "unit"));
		if (op)
			op->load(pt);
	}

	/*!
	 * \copydoc Base::Props::save
	 */
	void save(ptree & pt)
	{
	}

	Props() {
		op = NULL;
	}

	virtual ~Props() {
		delete op;
	}

private:
	template<typename T>
	PixelOperator<T> * str2operator(const std::string & name) {
		if (name == "tanh")
			return new TanhOperator<uint8_t>;
		if (name == "and")
			return new AndOperator<uint8_t>;
		if (name == "or")
			return new OrOperator<uint8_t>;
		if (name == "unit")
			return new UnitOperator<uint8_t>;

		return NULL;
	}

};

/*!
 * \class CvPixelOperator_Processor
 * \brief Convolves an image with the kernel
 */
class CvPixelOperator_Processor: public Base::Component
{
public:
	/*!
	 * Constructor.
	 */
	CvPixelOperator_Processor(const std::string & name = "");

	/*!
	 * Destructor
	 */
	virtual ~CvPixelOperator_Processor();

	/*!
	 * Return window properties
	 */
	Base::Props * getProperties()
	{
		return &props;
	}

protected:

	/*!
	 * Connects source to given device.
	 */
	bool onInit();

	/*!
	 * Disconnect source from device, closes streams, etc.
	 */
	bool onFinish();

	/*!
	 * Retrieves data from device.
	 */
	bool onStep();

	/*!
	 * Start component
	 */
	bool onStart();

	/*!
	 * Stop component
	 */
	bool onStop();


	/*!
	 * Event handler function.
	 */
	void onNewImage();

	/// Event handler.
	Base::EventHandler <CvPixelOperator_Processor> h_onNewImage;

	/// Input data stream
	Base::DataStreamIn <Mat> in_img;

	/// Event raised, when image is processed
	Base::Event * newImage;

	/// Output data stream - processed image
	Base::DataStreamOut <Mat> out_img;

	/// Threshold properties
	Props props;

	cv::Mat img;

};

}//: namespace CvPixelOperator
}//: namespace Processors


/*
 * Register processor component.
 */
REGISTER_PROCESSOR_COMPONENT("CvPixelOperator", Processors::CvPixelOperator::CvPixelOperator_Processor, Common::Panel_Empty)

#endif /* CVPIXELOPERATOR_PROCESSOR_HPP_ */

