/*
 * CvMatTranslator.h
 *
 *  Created on: 19-10-2010
 *      Author: mateusz
 */

#ifndef CVMATTRANSLATOR_H_
#define CVMATTRANSLATOR_H_

#include <boost/property_tree/ptree.hpp>
#include <cv.h>

namespace Types {

class CvMatTranslator
{
public:
	CvMatTranslator(cv::Size size) :
		size(size)
	{
	}

	virtual ~CvMatTranslator()
	{
	}

	template <class Ptree, class T>
	bool get_value(const Ptree &pt, T &value) const
	{
//		std::string s = pt.data();
//
//		std::stringstream ss;
//		cv::Mat mat = cv::Mat::eye(size, CV_32F);
//		double val;
//
//		ss << s;
//
//		for (int i = 0; i < size.height; ++i) {
//			for (int j = 0; j < size.width; ++j) {
//				ss >> val;
//				mat.at<float>(i,j) = val;
//			}
//		}
//
//		return mat;

		return true; // Success
	}

	// Custom inserter - converts data from T to boost::any
	template <class Ptree, class T>
	bool put_value(Ptree &pt, const T &value) const
	{
		pt.data() = value;
		return true;
	}
private:
	const cv::Size size;
};

} // namespace Types {

#endif /* CVMATTRANSLATOR_H_ */
