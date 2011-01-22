/*!
 * \file JunctionConstraint.hpp
 * \brief
 * \author mboryn
 * \date 2011-01-22
 */

#ifndef JUNCTIONCONSTRAINT_HPP_
#define JUNCTIONCONSTRAINT_HPP_

#include <boost/property_tree/ptree.hpp>

#include "AbstractConstraint.hpp"

namespace Processors {
namespace CspObjectRecognize {

/**
 * Represents constraint for which is satisfied for connected line segments.
 * Line segments are considered connected if one end is close enough to the other.
 *
 * Property <b>maxDistance</b> (=5) defines max. distance between ends to consider them connected.
 */
class JunctionConstraint: public Processors::CspObjectRecognize::AbstractConstraint
{
public:
	/**
	 * Create constraint.
	 * @param node property maxDistance may exist.
	 * @return
	 */
	JunctionConstraint(const boost::property_tree::ptree& node);
	virtual ~JunctionConstraint();

	/**
	 * Check if line segments have common ends.
	 * @param first
	 * @param second
	 * @return true if ends are close enough.
	 */
	virtual bool isSatisifed(boost::shared_ptr<AbstractShape> first, boost::shared_ptr<AbstractShape> second);
private:
	double maxDistance;
};

} // namespace CspObjectRecognize
} // namespace Processors

#endif /* JUNCTIONCONSTRAINT_HPP_ */
