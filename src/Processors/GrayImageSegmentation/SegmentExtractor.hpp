/*
 * SegmentExtractor.hpp
 *
 *  Created on: 04-12-2010
 *      Author: mateusz
 */

#ifndef SEGMENTEXTRACTOR_HPP_
#define SEGMENTEXTRACTOR_HPP_

#include <stack>

#include "SegmentedImage.hpp"

namespace Processors {
namespace GrayImageSegmentation {

using Types::Segmentation::MaskType;

class SegmentExtractor
{
public:
	SegmentExtractor();
	virtual ~SegmentExtractor();

	Types::Segmentation::SegmentedImage segmentImage(const cv::Mat& originalImage);

	void setMinVariance(double minVariance);
	void setMinSegmentArea(int minSegmentArea);
private:
	Types::Segmentation::SegmentedImage segmentedImage;
	const cv::Mat* originalImage;
	static const int histogramSize = 256;
	int histogram[histogramSize];
	double minVariance;
	MaskType currentHighestClass;
	int minSegmentArea;
	std::stack<cv::Point> points;

	void segmentRecursive(MaskType whichClass);
	void computeHistogram(MaskType whichClass);
	bool checkTerminationCondition();
	int findOptimalThreshold();
	void thresholdImage(int th, MaskType whichClass, MaskType classBelowOrEqual, MaskType classAbove);
	void extractHomogRegions(MaskType mask1, MaskType mask2);
	void extractHomogRegion(int y, int x, MaskType originalClass, MaskType newSegmentClass, int &area);
};

} // namespace GrayImageSegmentation
} // namespace Processors

#endif /* SEGMENTEXTRACTOR_HPP_ */
