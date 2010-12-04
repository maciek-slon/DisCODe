/*
 * SegmentExtractor.cpp
 *
 *  Created on: 04-12-2010
 *      Author: mateusz
 */

#include <stdexcept>

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
	}
	currentHighestClass = 1;
	segmentedImage.image.setTo(currentHighestClass);
	segmentedImage.segments.clear();

	segmentRecursive(currentHighestClass);

	return segmentedImage;
}

void SegmentExtractor::segmentRecursive(MaskType whichClass)
{
	computeHistogram(whichClass);
	if (checkTerminationCondition()) {
		LOG(LTRACE) << "Termination condition on histogram has been satisfied.\n";
		return;
	}

	int th = findOptimalThreshold();
	MaskType classBelowOrEqual = ++currentHighestClass;
	MaskType classAbove = ++currentHighestClass;

	int firstSegmentIdx = segmentedImage.segments.size();

	thresholdImage(th, whichClass, classBelowOrEqual, classAbove);

	int lastSegmentIdx = segmentedImage.segments.size();

	for (int i = firstSegmentIdx; i < lastSegmentIdx; ++i) {
		segmentRecursive(segmentedImage.segments[i].getSegmentClass());
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
			if (segmentedImage.image.at <MaskType> (y, x) != 0) {
				u_int8_t brightness = originalImage->at <u_int8_t> (y, x);
				histogram[brightness]++;
			}
		}
	}
}

bool SegmentExtractor::checkTerminationCondition()
{
	// TODO: policzyc wariancje
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

	// TODO: porwonac wariancje
	if (variance < minVariance) {
		return true;
	}
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

void SegmentExtractor::extractHomogRegions(MaskType mask1, MaskType mask2)
{
	int w = segmentedImage.image.size().width;
	int h = segmentedImage.image.size().height;
	for (int y = 0; y < h; ++y) {
		for (int x = 0; x < w; ++x) {
			MaskType originalClass = segmentedImage.image.at <MaskType> (y, x);
			if (originalClass == mask1 || originalClass == mask2) {
				int area = 0;
				MaskType newSegmentClass = ++currentHighestClass;

				extractHomogRegion(y, x, originalClass, newSegmentClass, area);
				if (area >= minSegmentArea) {
					Segment s(Point(x, y), newSegmentClass, area);
					segmentedImage.segments.push_back(s);
				}
			}
		}
	}
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
