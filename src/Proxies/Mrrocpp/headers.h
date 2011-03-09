/*
 * headers.h
 *
 *  Created on: Nov 5, 2010
 *      Author: mboryn
 */

#ifndef DISCODE_SENSOR_HEADERS_H_
#define DISCODE_SENSOR_HEADERS_H_

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

namespace Proxies {

namespace Mrrocpp {

struct initiate_message_header
{
	int data_size;
	bool is_rpc_call;

	template <class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & data_size;
		ar & is_rpc_call;
	}
};

struct reading_message_header
{
	int data_size;
	bool is_rpc_call;

	/** Time of reading. */
	uint64_t readingTimeSeconds;
	uint64_t readingTimeNanoseconds;

	/** Time, when reading is sent to mrrocpp */
	uint64_t sendTimeSeconds;
	uint64_t sendTimeNanoseconds;

	template <class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & data_size;
		ar & is_rpc_call;
		ar & readingTimeSeconds;
		ar & readingTimeNanoseconds;
		ar & sendTimeSeconds;
		ar & sendTimeNanoseconds;
	}
};

} // namespace Mrrocpp {

} // namespace Proxies

#endif /* DISCODE_SENSOR_HEADERS_H_ */
