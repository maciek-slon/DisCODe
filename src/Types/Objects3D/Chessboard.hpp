/*
 * Chessboard.hpp
 *
 *  Created on: 17-10-2010
 *      Author: mateusz
 */

#ifndef CHESSBOARD_HPP_
#define CHESSBOARD_HPP_

#include <cv.h>

#include "Object3D.hpp"

namespace Types {

namespace Objects3D {

class Chessboard : public Object3D
{
public:
	Chessboard(cv::Size patternSize) :
		patternSize(patternSize)
	{
	}
	Chessboard(const Chessboard& o) :
		Object3D(o),
		patternSize(o.patternSize)
	{
	}

	virtual ~Chessboard()
	{
	}

	virtual Chessboard* clone()
	{
		return new Chessboard(*this);
	}

	virtual void draw(cv::Mat& image, CvScalar color, int offsetX = 0, int offsetY = 0)
	{
		cv::Mat ip(imagePoints);
		cv::drawChessboardCorners(image, patternSize, ip, true);
	}
private:
	const cv::Size patternSize;
};

}

}

#endif /* CHESSBOARD_HPP_ */
