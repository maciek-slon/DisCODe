/*
 * AbstractConstraint.hpp
 *
 *  Created on: 09-01-2011
 *      Author: mateusz
 */

#ifndef ABSTRACTCONSTRAINT_HPP_
#define ABSTRACTCONSTRAINT_HPP_

#include "AbstractShape.hpp"

namespace Processors {
namespace CspObjectRecognize {

class AbstractShape;

class AbstractConstraint
{
public:
	AbstractConstraint();
	virtual ~AbstractConstraint();
	bool isSatisifed(AbstractShape *first,AbstractShape *second);
};

} // namespace Processors
} // namespace CspObjectRecognize

#endif /* ABSTRACTCONSTRAINT_HPP_ */
