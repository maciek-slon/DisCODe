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
	/**
	 * Class image of format MaskType_CV (single-channel integer)
	 */
	cv::Mat image;

	/**
	 * Edge image of format MaskType_CV (single-channel integer)
	 */
	cv::Mat edgeImage;

	/*
	 * Segments collection
	 */
	std::vector <Segment> segments;

	/**
	 * Detecte edges on image.
	 */
	void detectEdges();
};

} // namespace Segmentation
} // namespace Types

#endif /* SEGMENTEDIMAGE_HPP_ */
