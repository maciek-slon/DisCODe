/*!
 * \file CameraProps.hpp
 * \brief
 * \author mstefanc
 * \date 2010-05-28
 */

#ifndef CAMERAPROPS_HPP_
#define CAMERAPROPS_HPP_

#include "Props.hpp"
#include "VL_Common.hpp"

namespace Sources {
namespace CameraV4L {


class VL;

struct CameraProps : public Base::Props {
	// device settings
	/// device name
	std::string device;
	/// input method
	io_method io;
	/// Video standard
	int standard;
	/// frame width
	int width;
	/// frame height
	int height;
	/// selected channel
	std::string channel;
	/// interlace mode
	int interlace;

	// picture settings
	/// color palette
	int palette;
	/// picture whiteness
	int whiteness;
	/// picture brightness
	int brightness;
	/// picture contrast
	int contrast;
	/// picture hue
	int hue;

	/// Probably unused
	std::string min_settings;

	/*!
	 * \copydoc Common::Props::load
	 */
	void load(const ptree & pt);

	/*!
	 * \copydoc Common::Props::save
	 */
	void save(ptree & pt);

	void setDevice(VL * dev) {
		cam = dev;
	}

private:
	/// device to query some attributes from
	VL * cam;

};

}//: namespace CameraV4L
}//: namespace Sources

#endif /* CAMERAPROPS_HPP_ */
