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

	/**
	 * Segment image.
	 * @param originalImage Grayscale image
	 * @return
	 */
	Types::Segmentation::SegmentedImage segmentImage(const cv::Mat& originalImage);

	void setMinVariance(double minVariance);
	void setMinSegmentArea(int minSegmentArea);
private:
	/**
	 * Segmented image.
	 */
	Types::Segmentation::SegmentedImage segmentedImage;
	/**
	 * Keep original image.
	 */
	const cv::Mat* originalImage;
	/**
	 * Histogram for 8-bit grayscale image has 256 values.
	 */
	static const int histogramSize = 256;
	/**
	 * Histogram
	 */
	int histogram[histogramSize];

	double minVariance;
	/**
	 * Value increased when new segment is found.
	 */
	MaskType currentHighestClass;

	int minSegmentArea;

	/**
	 * Stack of points for homogeneous region extraction.
	 */
	std::stack<cv::Point> points;

	/**
	 * Performs thresholding and homogeneous region extraction from given segment.
	 * @param s
	 */
	void segmentRecursive(const Types::Segmentation::Segment& s);
	/**
	 * Computes histogram only for given class. Updates histogram field.
	 * @param whichClass
	 */
	void computeHistogram(MaskType whichClass);
	/**
	 * @return true if pixels value variance is lower than minVariance
	 */
	bool checkTerminationCondition();

	/**
	 * Finds optimal threshold for thresholding from field histogram.
	 * @return
	 */
	int findOptimalThreshold();
	/**
	 * Performs thresholding only for pixels of class whichClass.
	 * If pixel value is below or equal that threshold, then it is marked with classBelowOrEqual.
	 * @param th
	 * @param whichClass
	 * @param classBelowOrEqual
	 * @param classAbove
	 */
	void thresholdImage(int th, MaskType whichClass, MaskType classBelowOrEqual, MaskType classAbove);

	/**
	 * Extracts homogeneous regions from pixels of mask1 and mask2.
	 * @param mask1
	 * @param mask2
	 * @return
	 */
	std::vector<Types::Segmentation::Segment> extractHomogRegions(MaskType mask1, MaskType mask2);

	/**
	 * Extracts single region starting from given point
	 * @param y
	 * @param x
	 * @param originalClass
	 * @param newSegmentClass
	 * @param area found region area
	 */
	void extractHomogRegion(int y, int x, MaskType originalClass, MaskType newSegmentClass, int &area);
};

} // namespace GrayImageSegmentation
} // namespace Processors

#endif /* SEGMENTEXTRACTOR_HPP_ */
