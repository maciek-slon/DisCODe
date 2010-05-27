/*!
 * \file CameraV4L_Source.hpp
 * \brief
 * \author mstefanc
 * \date 2010-05-27
 */

#ifndef CAMERAV4L_SOURCE_HPP_
#define CAMERAV4L_SOURCE_HPP_

#include "Kernel_Aux.hpp"
#include "Kernel.hpp"
#include "Panel_Empty.hpp"
#include "StringState.hpp"
#include "DataStream.hpp"

namespace Sources {
namespace CameraV4L {

/*!
 * \class CameraV4L_Source
 * \brief
 */
class CameraV4L_Source : public Base::Kernel {

public:
	/*!
	 * Constructor. Sets ID and startup variables.
	 */
	CameraV4L_Source();

	/*!
	 * Destructor.
	 */
	~CameraV4L_Source();

	/*!
	 * Connects source to given device.
	 */
	void initialize();

	/*!
	 * Disconnect source from device, closes streams, etc.
	 */
	void finish();

	/*!
	 * Retrieves data from device.
	 */
	int step();

protected:
};

}//: namespace CameraV4L
}//: namespace Sources

/*
 * Register source kernel.
 */
REGISTER_SOURCE_KERNEL("CameraV4L", Sources::CameraV4L::CameraV4L_Source, Common::Panel_Empty, Common::StringState::instance())

#endif /* CAMERAV4L_SOURCE_HPP_ */
