#include "CameraProps.hpp"
#include "VL.hpp"
#include "Logger.hpp"

namespace Sources {
namespace CameraV4L {

void CameraProps::load(const ptree & pt) {
	LOG(LINFO) << "Loading settings for CameraV4L\n";
	device   =              pt.get("device.device", "/dev/video0");
	io       = convIOMethod(pt.get("device.io", "MMAP"));
	standard = convStandard(pt.get("device.video_standard", "PAL"));
	width    =              pt.get("device.width", 640);
	height   =              pt.get("device.height", 480);
	channel  =              pt.get("device.channel", "Composite");
}

void CameraProps::save(ptree & pt) {
	pt.put("device.device", device);
	pt.put("device.io", convIOMethod(io));
	pt.put("device.video_standard", convStandard(standard));
	pt.put("device.width", width);
	pt.put("device.height", height);
	pt.put("device.channel", channel);
}

}//: namespace CameraV4L
}//: namespace Sources
