/*!
 * \file CameraProps.hpp
 * \brief
 * \author mstefanc
 * \date 2010-05-28
 */

#ifndef CAMERAPROPS_HPP_
#define CAMERAPROPS_HPP_

#include "Props.hpp"

namespace Sources {
namespace CameraV4L {

static int convStandard(string palette) {
	if (palette.compare("PAL") == 0)
		return 0;
	if (palette.compare("NTSC") == 0)
		return 1;
	if (palette.compare("SECAM") == 0)
		return 2;
}

static int convChannel(string palette) {
	vector<string> channels = getChannels();
	vector<string>::iterator it;
	int i_chan = 0;
	for (it = channels.begin(); it != channels.end(); it++) {
		if (it->compare(palette) == 0)
			return i_chan;
		i_chan++;
	}
	return 0;
}

struct CameraProps : public Common::Props {
	// framegrabber settings
	/// Video standard
	int standard;
	/// frame width
	int width;
	/// frame height
	int height;
	/// selected channel
	int channel;
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
	void load(const ptree & pt) {
		standard = convStandard(pt.get("input_device.video_standard", "PAL"));
		width = pt.get("input_device.width", 640);
		height = pt.get("input_device.height", 480);
		channel = convChannel(pt.get("input_device.channel", "Composite"));
	}

	/*!
	 * \copydoc Common::Props::save
	 */
	void save(ptree & pt) {

	}

};

}//: namespace CameraV4L
}//: namespace Sources

#endif /* CAMERAPROPS_HPP_ */
