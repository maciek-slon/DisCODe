/*!
 * \file stream_OpenCV.hpp
 * \brief stream operators for OpenCV types
 */

#include <cv.h>

/*
 * Operators for standard streams
 */

template <typename Ch, typename Traits, typename t>
std::basic_ostream<Ch, Traits> & operator<<(std::basic_ostream<Ch, Traits> & out, const cv::Size_<t> & s) {
	out << s.width << " " << s.height;
	return out;
}

template <typename Ch, typename Traits, typename t>
std::basic_istream<Ch, Traits> & operator>>(std::basic_istream<Ch, Traits> & in, cv::Size_<t> & s) {
	t w,h;
	in >> w >> h;
	s.width = w;
	s.height = h;
	return in;
}

/*
 * Operators for Boost::PropertyTree
 */

namespace boost {
namespace property_tree {

template <typename Ch, typename Traits, typename t>
struct customize_stream<Ch, Traits, cv::Size_<t>, void>
{

	static void insert(std::basic_ostream<Ch, Traits>& s, const cv::Size_<t>& e) {
		s << e.width << " " << e.height;
	}

	static void extract(std::basic_istream<Ch, Traits>& s, cv::Size_<t>& e) {
		t w,h;
		s >> w >> h;
		e.width = w;
		e.height = h;
	}
};

}
}
