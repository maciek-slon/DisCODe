#include "CameraProps.hpp"
#include "VL.hpp"

namespace Sources {
namespace CameraV4L {

void CameraProps::load(const ptree & pt) {
	device   =                  pt.get("device.device", "video0");
	io       = VL->convIOMethod(pt.get("device.io", "MMAP"));
	standard = VL->convStandard(pt.get("device.video_standard", "PAL"));
	width    =                  pt.get("device.width", 640);
	height   =                  pt.get("device.height", 480);
	channel  = VL->convChannel( pt.get("device.channel", "Composite"));
}

void CameraProps::save(ptree & pt) {

}

}//: namespace CameraV4L
}//: namespace Sources
