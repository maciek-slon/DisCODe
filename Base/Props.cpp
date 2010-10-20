/*
 * Props.cpp
 *
 *  Created on: 20-10-2010
 *      Author: mateusz
 */

#include "Props.hpp"

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <sstream>

namespace Base {

using namespace std;
using namespace boost::numeric::ublas;

matrix <double> Props::str2mat(const std::string& str, int rows, int cols) const
{
	matrix <double> m (rows, cols);

	stringstream ss(str);

	ss>>m;

	return m;
}

} // namespace Base
