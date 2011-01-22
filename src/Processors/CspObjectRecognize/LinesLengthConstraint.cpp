/*
 * LinesLengthConstraint.cpp
 *
 *  Created on: 09-01-2011
 *      Author: mateusz
 */

#include <limits>

#include "LinesLengthConstraint.hpp"

namespace Processors {
namespace CspObjectRecognize {

using namespace std;

LinesLengthConstraint::LinesLengthConstraint(const boost::property_tree::ptree& node)
{
	minLengthAbsolute = node.get<double>("minLengthAbsolute", numeric_limits<double>::infinity());
	maxLengthAbsolute = node.get<double>("maxLengthAbsolute", numeric_limits<double>::infinity());
	minLengthRelative = node.get<double>("minLengthRelative", numeric_limits<double>::infinity());
	maxLengthRelative = node.get<double>("maxLengthRelative", numeric_limits<double>::infinity());
}

LinesLengthConstraint::~LinesLengthConstraint()
{
}

bool LinesLengthConstraint::isSatisifed(boost::shared_ptr <AbstractShape> first, boost::shared_ptr <AbstractShape> second)
{
	return false;
}

}
}
