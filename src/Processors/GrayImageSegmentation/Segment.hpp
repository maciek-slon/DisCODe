/*
 * Segment.hpp
 *
 *  Created on: 04-12-2010
 *      Author: mateusz
 */

#ifndef SEGMENT_HPP_
#define SEGMENT_HPP_

#include <cv.h>

namespace Types {
namespace Segmentation {

typedef int32_t MaskType;
#define MaskType_CV CV_32S

class Segment
{
public:
	Segment(cv::Point startingPoint, MaskType segmentClass);
	Segment(cv::Point startingPoint, MaskType segmentClass, int area);
	Segment(const Segment& o);
	virtual ~Segment();

	cv::Point getStartingPoint() const;
	MaskType getSegmentClass() const;
	void setSegmentImage(cv::Mat& segmentImage);
	void setSegmentImageFromSegmentedImage(cv::Mat& segmentedImage);
	cv::Mat getSegmentImage();
	std::vector <std::vector <cv::Point> >* getContours();
private:
	cv::Point startingPoint;
	MaskType segmentClass;

	bool segmentImageSet;
	cv::Mat segmentImage;

	bool areaComputed;
	int area;

	bool contoursComputed;
	std::vector <std::vector <cv::Point> > contours;
	void computeContours();
};

} // namespace Segmentation
} // namespace Types

#endif /* SEGMENT_HPP_ */
