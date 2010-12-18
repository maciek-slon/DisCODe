/*
 * SegmentExtractor.cpp
 *
 *  Created on: 04-12-2010
 *      Author: mateusz
 */

#include <stdexcept>
#include <boost/foreach.hpp>
#include "SegmentExtractor.hpp"

#include "Logger.hpp"

namespace Processors {
namespace GrayImageSegmentation {

using namespace cv;
using namespace std;
using Types::Segmentation::Segment;

SegmentExtractor::SegmentExtractor()
{
}

SegmentExtractor::~SegmentExtractor()
{
}

Types::Segmentation::SegmentedImage SegmentExtractor::segmentImage(const cv::Mat& originalImage)
{
	this->originalImage = &originalImage;
	if (segmentedImage.image.size() != originalImage.size()) {
		LOG(LTRACE) << "SegmentExtractor::segmentImage() changing image size.";
		segmentedImage.image = Mat(originalImage.size(), MaskType_CV);
		segmentedImage.edgeImage = Mat(originalImage.size(), MaskType_CV);
	}

	// initialize result
	currentHighestClass = 1;
	segmentedImage.image.setTo(currentHighestClass);
	segmentedImage.segments.clear();

	Segment s(Point(0, 0), currentHighestClass);

	// perform segmentation
	segmentRecursive(s);

	// clear mask for unclassified pixels
	int w = segmentedImage.image.size().width;
	int h = segmentedImage.image.size().height;
	for (int y = 0; y < h; ++y) {
		for (int x = 0; x < w; ++x) {
			MaskType m = segmentedImage.image.at <MaskType> (y, x);
			bool found = false;
			for (size_t i = 0; i < segmentedImage.segments.size(); ++i) {
				if (segmentedImage.segments[i].getSegmentClass() == m) {
					found = true;
					break;
				}
			}
			if (!found) {
				segmentedImage.image.at <MaskType> (y, x) = 0;
			}
		}
	}

	return segmentedImage;
}

void SegmentExtractor::segmentRecursive(const Segment& s)
{
	MaskType whichClass = s.getSegmentClass();
	computeHistogram(whichClass);
	if (checkTerminationCondition()) { // this segment has met termination condition
//		LOG(LFATAL) << "Termination condition on histogram has been satisfied.\n";
		segmentedImage.segments.push_back(s);
		return;
	}

	int th = findOptimalThreshold();
//	LOG(LFATAL) << "findOptimalThreshold(): " << th;
	// add new pixel classes
	MaskType classBelowOrEqual = ++currentHighestClass;
	MaskType classAbove = ++currentHighestClass;

//	LOG(LFATAL) << "currentHighestClass: " << currentHighestClass;
//	LOG(LFATAL) << "classBelowOrEqual: " << classBelowOrEqual;
//	LOG(LFATAL) << "classAbove: " << classAbove;

	// threshold with these classes
	thresholdImage(th, whichClass, classBelowOrEqual, classAbove);
	// extract homog regions from thresholded image
	vector <Segment> segments = extractHomogRegions(classBelowOrEqual, classAbove);
//	LOG(LFATAL) << "extractHomogRegions returned " << segments.size() << " segments";

	// segment each new region found
	BOOST_FOREACH(Segment s, segments)
				{
					//LOG(LFATAL) << "segmenting recursive...";
					segmentRecursive(s);
				}
}

void SegmentExtractor::computeHistogram(MaskType whichClass)
{
	for (int i = 0; i < histogramSize; ++i) {
		histogram[i] = 0;
	}
	Size imageSize = originalImage->size();
	for (int y = 0; y < imageSize.height; ++y) {
		for (int x = 0; x < imageSize.width; ++x) {
			if (segmentedImage.image.at <MaskType> (y, x) == whichClass) {
				u_int8_t brightness = originalImage->at <u_int8_t> (y, x);
				histogram[brightness]++;
			}
		}
	}
}

bool SegmentExtractor::checkTerminationCondition()
{
	// compute variance
	double sum = 0;
	double cnt = 0;
	for (int i = 0; i < histogramSize; ++i) {
		sum += i * histogram[i];
		cnt += histogram[i];
	}
	double mean = sum / cnt;

	sum = 0;
	cnt = 0;
	for (int i = 0; i < histogramSize; ++i) {
		sum += (i - mean) * (i - mean) * histogram[i];
		cnt += histogram[i];
	}
	double variance = sum / (cnt * histogramSize * histogramSize);

	//	cout << "Segmentation::checkTerminationCondition(): sum = " << sum << endl;
	//	cout << "Segmentation::checkTerminationCondition(): cnt = " << cnt << endl;
	//	cout << "Segmentation::checkTerminationCondition(): Threshold::histogramSize = " << Threshold::histogramSize << endl;
	//	cout << "Segmentation::checkTerminationCondition(): variance = " << variance << endl;

	// compare variance
	if (variance < minVariance) {
//		LOG(LFATAL) << "SegmentExtractor::checkTerminationCondition(): variance < minVariance";
		return true;
	}
//	LOG(LFATAL) << "SegmentExtractor::checkTerminationCondition(): variance >= minVariance";
	return false;
}

void SegmentExtractor::setMinVariance(double minVariance)
{
	this->minVariance = minVariance;
}

int SegmentExtractor::findOptimalThreshold()
{
	double sum = 0;
	int cnt = 0;
	for (int i = 0; i < histogramSize; ++i) {
		sum += histogram[i] * i;
		cnt += histogram[i];
	}
	if (cnt == 0) {
		throw logic_error("empty histogram");
	}
	int currentThreshold = sum / cnt;
	int prevThreshold = currentThreshold;

	int iterations = 0;
	do {
		iterations++;
		double sumBelowOrEqual = 0;
		int cntBelowOrEqual = 0;
		for (int i = 0; i <= currentThreshold; ++i) {
			cntBelowOrEqual += histogram[i];
			sumBelowOrEqual += histogram[i] * i;
		}

		double sumAbove = 0;
		int cntAbove = 0;
		for (int i = currentThreshold + 1; i < histogramSize; ++i) {
			cntAbove += histogram[i];
			sumAbove += histogram[i] * i;
		}

		//		double meanBelowOrEqual = cntBelowOrEqual == 0 ? 0 : sumBelowOrEqual / cntBelowOrEqual;
		double meanBelowOrEqual = sumBelowOrEqual / cntBelowOrEqual; //cntBelowOrEqual > 0
		double meanAbove = cntAbove == 0 ? histogramSize - 1 : sumAbove / cntAbove;

		prevThreshold = currentThreshold;
		currentThreshold = (meanBelowOrEqual + meanAbove) / 2;
	} while (abs(prevThreshold - currentThreshold) > 0);

	LOG(LTRACE) << "Threshold::findOptimalThreshold() iterations = " << iterations << " threshold found: "
			<< currentThreshold << endl;
	return currentThreshold;
}

void SegmentExtractor::thresholdImage(int th, MaskType whichClass, MaskType classBelowOrEqual, MaskType classAbove)
{
	Size imageSize = originalImage->size();
	for (int y = 0; y < imageSize.height; ++y) {
		for (int x = 0; x < imageSize.width; ++x) {
			if (segmentedImage.image.at <MaskType> (y, x) == whichClass) {
				if (originalImage->at <u_int8_t> (y, x) > th) {
					segmentedImage.image.at <MaskType> (y, x) = classAbove;
				} else {
					segmentedImage.image.at <MaskType> (y, x) = classBelowOrEqual;
				}
			}
		}
	}
}

std::vector <Types::Segmentation::Segment> SegmentExtractor::extractHomogRegions(MaskType mask1, MaskType mask2)
{
	vector <Segment> segments;
	int w = segmentedImage.image.size().width;
	int h = segmentedImage.image.size().height;
	// check every pixel if it has mask1 or mask2
	for (int y = 0; y < h; ++y) {
		for (int x = 0; x < w; ++x) {
			MaskType originalClass = segmentedImage.image.at <MaskType> (y, x);
			if (originalClass == mask1 || originalClass == mask2) { // first pixel of new segment found
				int area = 0;
				MaskType newSegmentClass = ++currentHighestClass;
				// extract single region
				extractHomogRegion(y, x, originalClass, newSegmentClass, area);
				if (area >= minSegmentArea) { // add only if area is greater than desired
					Segment s(Point(x, y), newSegmentClass, area);
					segments.push_back(s);
				}
			}
		}
	}
	return segments;
}

void SegmentExtractor::setMinSegmentArea(int minSegmentArea)
{
	this->minSegmentArea = minSegmentArea;
}

void SegmentExtractor::extractHomogRegion(int y, int x, MaskType originalClass, MaskType newSegmentClass, int &area)
{
	int w = segmentedImage.image.size().width - 1, h = segmentedImage.image.size().height - 1;
	area = 0;

	//	points.clear();
	while (!points.empty()) {
		points.pop();
	}

	points.push(Point(x, y));
	segmentedImage.image.at <MaskType> (points.top()) = newSegmentClass;

	// for each point check it's neighbors pixels and if they are the same as first segment pixel put them on the stack
	while (!points.empty()) {
		Point p = points.top();
		points.pop();

		area++;
		if (p.x > 0) { // left
			Point newP(p.x - 1, p.y);
			if (segmentedImage.image.at <MaskType> (newP) == originalClass) {
				segmentedImage.image.at <MaskType> (newP) = newSegmentClass;
				points.push(newP);
			}
		}
		if (p.y > 0) { // upper
			Point newP(p.x, p.y - 1);
			if (segmentedImage.image.at <MaskType> (newP) == originalClass) {
				segmentedImage.image.at <MaskType> (newP) = newSegmentClass;
				points.push(newP);
			}
		}
		if (p.x < w) { // right
			Point newP(p.x + 1, p.y);
			if (segmentedImage.image.at <MaskType> (newP) == originalClass) {
				segmentedImage.image.at <MaskType> (newP) = newSegmentClass;
				points.push(newP);
			}
		}
		if (p.y < h) { // lower
			Point newP(p.x, p.y + 1);
			if (segmentedImage.image.at <MaskType> (newP) == originalClass) {
				segmentedImage.image.at <MaskType> (newP) = newSegmentClass;
				points.push(newP);
			}
		}
	}
}

} // namespace GrayImageSegmentation
} // namespace Processors
