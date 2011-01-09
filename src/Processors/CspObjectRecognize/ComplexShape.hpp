/*
 * ComplexShape.hpp
 *
 *  Created on: 09-01-2011
 *      Author: mateusz
 */

#ifndef COMPLEXSHAPE_HPP_
#define COMPLEXSHAPE_HPP_

#include "AbstractShape.hpp"

namespace Processors {
namespace CspObjectRecognize {

class ComplexShape: public Processors::CspObjectRecognize::AbstractShape
{
public:
	ComplexShape();
	virtual ~ComplexShape();
};

} // namespace CspObjectRecognize
} // namespace Processors

#endif /* COMPLEXSHAPE_HPP_ */
