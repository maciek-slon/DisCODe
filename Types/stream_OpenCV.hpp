/*!
 * \file stream_OpenCV.hpp
 * \brief stream operators for OpenCV types
 */

#include <cv.h>

/*
 * Operators for standard streams
 */

/*!
 * Stream operator printing cv::Size_<T>
 * @param s output stream
 * @param e size to be printed
 * @return output stream
 */
template <typename Ch, typename Traits, typename T>
std::basic_ostream<Ch, Traits> & operator<<(std::basic_ostream<Ch, Traits> & s, const cv::Size_<T> & e) {
	s << e.width << " " << e.height;
	return s;
}

/*!
 * Stream operator reading cv::Size_<T>
 * @param s input stream
 * @param e size to be read
 * @return input stream
 */
template <typename Ch, typename Traits, typename T>
std::basic_istream<Ch, Traits> & operator>>(std::basic_istream<Ch, Traits> & s, cv::Size_<T> & e) {
	T w,h;
	s >> w >> h;
	e.width = w;
	e.height = h;
	return s;
}

/*!
 * Stream operator printing cv::Rect_<T>
 * @param s output stream
 * @param e rect to be printed
 * @return output stream
 */
template <typename Ch, typename Traits, typename T>
std::basic_ostream<Ch, Traits> & operator<<(std::basic_ostream<Ch, Traits> & s, const cv::Rect_<T> & e) {
	s << e.x << " " << e.y << " " << e.width << " " << e.height;
	return s;
}

/*!
 * Stream operator reading cv::Rect_<T>
 * @param s input stream
 * @param e rect to be read
 * @return input stream
 */
template <typename Ch, typename Traits, typename T>
std::basic_istream<Ch, Traits> & operator>>(std::basic_istream<Ch, Traits> & s, cv::Rect_<T> & e) {
	T x,y,w,h;
	s >> x >> y >> w >> h;

	e.x = x;
	e.y = y;
	e.width = w;
	e.height = h;
	return s;
}


/*
 * Operators for Boost::PropertyTree
 */

namespace boost {
namespace property_tree {

template <typename Ch, typename Traits, typename T>
struct customize_stream<Ch, Traits, cv::Size_<T>, void>
{

	static void insert(std::basic_ostream<Ch, Traits>& s, const cv::Size_<T>& e) {
		s << e.width << " " << e.height;
	}

	static void extract(std::basic_istream<Ch, Traits>& s, cv::Size_<T>& e) {
		T w,h;
		s >> w >> h;
		e.width = w;
		e.height = h;
	}
};


template <typename Ch, typename Traits, typename T>
struct customize_stream<Ch, Traits, cv::Rect_<T>, void>
{

	static void insert(std::basic_ostream<Ch, Traits>& s, const cv::Rect_<T>& e) {
		s << e.x << " " << e.y << " " << e.width << " " << e.height;
	}

	static void extract(std::basic_istream<Ch, Traits>& s, cv::Rect_<T>& e) {
		T x,y,w,h;
		s >> x >> y >> w >> h;

		e.x = x;
		e.y = y;
		e.width = w;
		e.height = h;
	}
};

}
}
