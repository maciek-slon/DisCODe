/*
 * Segment.cpp
 *
 *  Created on: 04-12-2010
 *      Author: mateusz
 */

#include <stdexcept>

#include "Segment.hpp"

namespace Types {
namespace Segmentation {

using namespace cv;
using namespace std;

Segment::Segment(cv::Point startingPoint, MaskType segmentClass) :
	startingPoint(startingPoint), segmentClass(segmentClass), segmentImageSet(false), areaComputed(false),
			contoursComputed(false)
{
}

Segment::Segment(cv::Point startingPoint, MaskType segmentClass, int area) :
	startingPoint(startingPoint), segmentClass(segmentClass), areaComputed(false), area(area)
{

}

Segment::Segment(const Segment& o)
{
	startingPoint = o.startingPoint;
	segmentClass = o.segmentClass;
	areaComputed = o.areaComputed;
	area = o.area;
}

Segment::~Segment()
{
}

cv::Point Segment::getStartingPoint() const
{
	return startingPoint;
}
MaskType Segment::getSegmentClass() const
{
	return segmentClass;
}

cv::Mat Segment::getSegmentImage()
{
	return segmentImage;
}

void Segment::setSegmentImage(cv::Mat& segmentImage)
{
	this->segmentImage = segmentImage;
	segmentImageSet = true;
}

void Segment::setSegmentImageFromSegmentedImage(cv::Mat& segmentedImage)
{
	if (segmentImage.size() != segmentedImage.size() || segmentImage.type() != CV_8U) {
		segmentImage = Mat(segmentedImage.size(), CV_8U);
	}

	int w = segmentedImage.size().width;
	int h = segmentedImage.size().height;
	for (int y = 0; y < h; ++y) {
		for (int x = 0; x < w; ++x) {
			segmentImage.at <u_int8_t> (y, x) = segmentedImage.at <MaskType> (y, x) == segmentClass ? 255 : 0;
		}
	}

	segmentImageSet = true;
}

void Segment::computeContours()
{
	if (!segmentImageSet) {
		throw logic_error("Segment::computeContours() called, but segmentImage hasn't been set.");
	}
	Mat clonedImage = segmentImage.clone();
	contours.clear();
	findContours(clonedImage, contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);
	contoursComputed = true;
}

std::vector <std::vector <cv::Point> >* Segment::getContours()
{
	if (!contoursComputed) {
		computeContours();
	}
	return &contours;
}

} // namespace Segmentation
} // namespace Types
