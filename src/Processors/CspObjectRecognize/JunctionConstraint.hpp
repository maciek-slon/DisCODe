/*
 * JunctionConstraint.hpp
 *
 *  Created on: 19-01-2011
 *      Author: mateusz
 */

#ifndef JUNCTIONCONSTRAINT_HPP_
#define JUNCTIONCONSTRAINT_HPP_

#include "AbstractConstraint.hpp"

namespace Processors {
namespace CspObjectRecognize {

class JunctionConstraint: public Processors::CspObjectRecognize::AbstractConstraint
{
public:
	JunctionConstraint();
	virtual ~JunctionConstraint();
	virtual bool isSatisifed(boost::shared_ptr<AbstractShape> first, boost::shared_ptr<AbstractShape> second);
};

} // namespace CspObjectRecognize
} // namespace Processors

#endif /* JUNCTIONCONSTRAINT_HPP_ */
