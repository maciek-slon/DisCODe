/*!
 * \file Executor.cpp
 * \brief
 */

#include "Executor.hpp"
#include "Component.hpp"
#include "Timer.hpp"

#include <boost/foreach.hpp>

namespace Core {

std::vector<std::string> Executor::listComponents() {
	std::vector<std::string> ret;
	BOOST_FOREACH(ComponentPair cp, components) {
		ret.push_back(cp.first);
	}
	return ret;
}

}//: namespace Core

