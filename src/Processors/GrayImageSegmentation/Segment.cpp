/*
 * Segment.cpp
 *
 *  Created on: 04-12-2010
 *      Author: mateusz
 */

#include <stdexcept>

#include "Segment.hpp"
#include "Logger.hpp"

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
	segmentImageSet = o.segmentImageSet;
	segmentImage = o.segmentImage.clone();
	areaComputed = o.areaComputed;
	area = o.area;
	contoursComputed = o.contoursComputed;
	contours = o.contours;
	lineSegmentsComputed = o.lineSegmentsComputed;
	lineSegments = o.lineSegments;
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

void Segment::computeLineSegments()
{
	getContours();
	lineSegments.clear();
	std::vector <std::vector <cv::Point> >::const_iterator contourIt;
	LOG(LDEBUG) << "Segment::computeLineSegments() contours.size(): " << contours.size();
	for (contourIt = contours.begin(); contourIt != contours.end(); ++contourIt) {
		LOG(LDEBUG) << "Segment::computeLineSegments() contour length: " << contourIt->size() << endl;
		computeLineSegmentsForSingleContour(contourIt, 0, contourIt->size() - 1);
	}
	lineSegmentsComputed = true;
}

void Segment::computeLineSegmentsForSingleContour(std::vector <std::vector <cv::Point> >::const_iterator contourIt, int p1Idx, int p2Idx)
{
	LOG(LDEBUG) << "Segment::computeLineSegmentsForSingleContour: (" << p1Idx << ", " << p2Idx << ")\n";
	//	if (!(p1Idx < p2Idx)) {
	//
	//	}

	Point p1 = (*contourIt)[p1Idx];
	Point p2 = (*contourIt)[p2Idx];

	Line line(p1, p2);

	double maxDistance = 0;
	int maxDistancePointIdx = p1Idx;
	for (int i = p1Idx + 1; i < p2Idx - 1; ++i) {
		Point p3 = (*contourIt)[i];
		double dist = line.getDistanceFromPoint(p3);
		if (maxDistance < dist) {
			maxDistance = dist;
			maxDistancePointIdx = i;
		}
	}
	if (maxDistance > splitDistance) {
		computeLineSegmentsForSingleContour(contourIt, p1Idx, maxDistancePointIdx);
		computeLineSegmentsForSingleContour(contourIt, maxDistancePointIdx, p2Idx);
	} else {
		lineSegments.push_back(line);
	}
}

std::vector <Types::Line>* Segment::getLineSegments()
{
	if (!lineSegmentsComputed) {
		computeLineSegments();
	}
	return &lineSegments;
}

} // namespace Segmentation
} // namespace Types
