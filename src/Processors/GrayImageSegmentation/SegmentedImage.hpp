/*
 * SegmentedImage.hpp
 *
 *  Created on: 04-12-2010
 *      Author: mateusz
 */

#ifndef SEGMENTEDIMAGE_HPP_
#define SEGMENTEDIMAGE_HPP_

#include <vector>
#include <cv.h>

#include "Segment.hpp"

namespace Types {
namespace Segmentation {

struct SegmentedImage
{
	cv::Mat image;
	std::vector <Segment> segments;
};

} // namespace Segmentation
} // namespace Types

#endif /* SEGMENTEDIMAGE_HPP_ */
