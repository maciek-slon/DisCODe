/*!
 * \file Sequence.hpp
 * \brief Class responsible for retrieving images from image sequences - class declaration.
 */


#ifndef CAMERAGIGE_HPP_
#define CAMERAGIGE_HPP_

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "Panel_Empty.hpp"
#include "DataStream.hpp"
#include "Props.hpp"

#include <vector>
#include <string>

#include <cv.h>

#ifdef _WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Winsock2.h>
#endif

//#if defined(_LINUX) || defined(_QNX) || defined(_OSX)
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <sys/times.h>
#include <arpa/inet.h>
//#endif

#define _LINUX
#define _x64

#include <PvApi.h>

/**
 * \defgroup Sequence Sequence
 * \ingroup Sources
 *
 * \brief Image sequence provider.
 *
 * At the moment there is only one type of sequences
 * available, based on image filename pattern (regular expression) and directory,
 * in which files will be searched.
 *
 *
 * \par Data streams:
 *
 * \streamout{out_img,cv::Mat}
 * Output image
 *
 *
 * \par Events:
 *
 * \event{newImage}
 * New image is ready
 * \event{endOfSequence}
 * Sequence has ended
 *
 *
 * \par Event handlers:
 *
 * \handler{onTrigger}
 * Trigger new frame
 *
 *
 * \par Properties:
 *
 * \prop{directory,string,"."}
 * Directory, where fils will be searched
 * \prop{pattern,string,".*\.jpg"}
 * Regex pattern used for searching files
 * \prop{sort,bool,true}
 * If set, then found siles will be sorted in ascending order
 * \prop{prefetch,bool,false}
 * If set, all files will be loaded beforehand, otherwise images are loaded just before they are needed.
 * \prop{triggered,bool,false}
 * If set, new frames will be produced only after onTrigger event
 *
 *
 * \see \ref regex_basics
 *
 *
 * @{
 *
 * @}
 */

namespace Sources {
namespace CameraGigE {

struct Props : public Base::Props {

	std::string address;
	uint32_t uid;

//	bool sort;
//	bool prefetch;

//	bool triggered;


	void load(const ptree & pt) {
		address = pt.get("address", "");
		uid = pt.get("uid", 0);

	//	sort = pt.get("sort", true);
	//	prefetch = pt.get("prefetch", false);

	//	triggered = pt.get("triggered", false);
	}

	void save(ptree & pt) {
		pt.put("address", address);
		pt.put("uid", uid);
		//pt.put("sort", sort);
		//pt.put("prefetch", prefetch);
		//pt.put("triggered", triggered);
	}
};

/*!
 * \class Sequence
 * \brief Class responsible for retrieving images from image sequences.
 */
class CameraGigE : public Base::Component {

public:
	/*!
	 * Constructor. Sets ID and startup variables.
	 */
	CameraGigE(const std::string & name = "");

	/*!
	 * Destructor.
	 */
	virtual ~CameraGigE();

	/*!
	 * Return sequence properties
	 */
	Base::Props * getProperties() {
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


	/// Event signaling that new image was retrieved.
	Base::Event * newImage;

	/// Sequence has ended
	Base::Event * endOfSequence;

	/// Output data stream
	Base::DataStreamOut<cv::Mat> out_img;


	/*!
	 * Event handler function.
	 */
	void onTrigger();

	/// Event handler.
	Base::EventHandler<CameraGigE> h_onTrigger;

private:
	/// Camera handle
	tPvHandle 	cHandle;
	tPvFrame	frame;

	/// current frame
	cv::Mat img;

	/// sequence properties
	Props props;

};

}//: namespace Sequence
}//: namespace Sources

/*
 * Register source component.
 */
REGISTER_SOURCE_COMPONENT("CameraGigE", Sources::CameraGigE::CameraGigE, Common::Panel_Empty)

#endif /* CAMERAGIGE_HPP_ */
