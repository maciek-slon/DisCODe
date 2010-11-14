/*!
 * \file
 * \brief
 */

#ifndef KW_COV_HPP_
#define KW_COV_HPP_

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "Panel_Empty.hpp"
#include "DataStream.hpp"
#include "Props.hpp"

#include <cv.h>
#include <highgui.h>

namespace Processors {
namespace KW_Cov_ {

using namespace cv;

/*!
 * \brief KW_Cov properties
 */
struct Props: public Base::Props
{
	int channels;
	bool normalize;

	/*!
	 * \copydoc Base::Props::load
	 */
	void load(const ptree & pt)
	{
		channels = pt.get("channels", 3);
		if (channels < 1)
			channels = 1;
		if (channels > 3)
			channels = 3;

		normalize = pt.get("normalize", true);
	}

	/*!
	 * \copydoc Base::Props::save
	 */
	void save(ptree & pt)
	{
	}
};

/*!
 * \class KW_Cov
 * \brief Example processor class.
 */
class KW_Cov: public Base::Component
{
public:
	/*!
	 * Constructor.
	 */
	KW_Cov(const std::string & name = "");

	/*!
	 * Destructor
	 */
	virtual ~KW_Cov();

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
	/// New image is waiting
	Base::EventHandler <KW_Cov> h_onNewImage;


	/*!
	 * Event handler function.
	 */
	void calculate();

	/// Event handler.
	/// New image is waiting
	Base::EventHandler <KW_Cov> h_calculate;




	/// Input image
	Base::DataStreamIn <Mat> in_img;


	/// Properties
	Props props;

private:
	std::vector<float> c[3];

	bool calculated;
};

}//: namespace KW_Cov_
}//: namespace Processors


/*
 * Register processor component.
 */
REGISTER_PROCESSOR_COMPONENT("KW_Cov", Processors::KW_Cov_::KW_Cov, Common::Panel_Empty)

#endif /* KW_COV_HPP_ */

