/*
 * SampleClass.hpp
 *
 *  Created on: 31-10-2010
 *      Author: mateusz
 */

#ifndef SAMPLECLASS_HPP_
#define SAMPLECLASS_HPP_

#include <string>

#include <boost/serialization/string.hpp>
#include "xdr/xdr_oarchive.hpp"


namespace Proxies {

namespace Mrrocpp {

class SampleClass
{
public:
	SampleClass();
	SampleClass(int numberOne, int numberTwo, const char *text, const std::string &str);
	virtual ~SampleClass();

	int numberOne;
	int numberTwo;
	char text[16];
	std::string str;
};

} // namespace Mrrocpp

} // namespace Proxies

namespace boost {
namespace serialization {

template<class Archive>
void serialize(Archive & ar, Proxies::Mrrocpp::SampleClass & g, const unsigned int version)
{
    ar & g.numberOne;
    ar & g.numberTwo;
    ar & g.text;
    ar & g.str;
}

} // namespace serialization
} // namespace boost


#endif /* SAMPLECLASS_HPP_ */
