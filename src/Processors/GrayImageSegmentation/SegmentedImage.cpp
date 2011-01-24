/*
 * SegmentedImage.cpp
 *
 *  Created on: 04-12-2010
 *      Author: mateusz
 */

#include "SegmentedImage.hpp"

namespace Types {
namespace Segmentation {

void SegmentedImage::detectEdges()
{
	int w = edgeImage.size().width;
	int h = edgeImage.size().height;

	for (int y = 0; y < h; ++y) {
		for (int x = 0; x < w; ++x) {
			// check neighbors, if they are different, there is edge.
			MaskType m = image.at <MaskType> (y, x);
			MaskType dstMask = 0;
			if ((y > 0 && image.at <MaskType> (y - 1, x) != m) || // upper
					(y < h - 1 && image.at <MaskType> (y + 1, x) != m) || // lower
					(x > 0 && image.at <MaskType> (y, x - 1) != m) || // left
					(x < w - 1 && image.at <MaskType> (y, x + 1) != m) // right
			) {
				dstMask = m;
			}
			edgeImage.at <MaskType> (y, x) = dstMask;
		}
	}
}

boost::shared_ptr<SegmentedImage> SegmentedImage::clone(){
	boost::shared_ptr<SegmentedImage> si = boost::shared_ptr<SegmentedImage>(new SegmentedImage);
	si->image = image.clone();
	si->edgeImage = edgeImage.clone();
	si->segments = segments;
	return si;
}

} // namespace Segmentation
} // namespace Types
