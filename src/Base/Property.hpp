/*!
 * \file Property.hpp
 * \brief
 */

#ifndef PROPERTY_HPP_
#define PROPERTY_HPP_

#include <string>

#include <boost/lexical_cast.hpp>

namespace Base {

class PropertyInterface {
public:

	PropertyInterface(const std::string & n) : name_(n), persistent(true) {}

	/*
	template <typename T>
	T get() {
		return &((T*)internalGet());
	}

	template <typename T>
	T set(const T & t) {
		internalSet(&t);
		return t;
	}*/



	bool isPersistent() {
		return persistent;
	}

	virtual std::string store() = 0;

	virtual void retrieve(const std::string & str) = 0;


	const std::string & name() const {
		return name_;
	}

private:
	/// property name
	std::string name_;

	bool persistent;
};

/*!
 *
 */
template < class T >
class Property : public PropertyInterface
{
public:
	Property(const std::string& name, const T & initializer = T()) : PropertyInterface(name), data(initializer)
	{
	}

	/*!
	 * Access with function call syntax
	 */
	T operator()() const
	{
		return data;
	}

	T operator()(T const & value)
	{
		data = value;
		return data;
	}

	/*!
	 * Return value
	 *
	 * @return current value
	 */
	operator T() const
	{
		return data;
	}

	/*!
	 * Set new value.
	 *
	 * @param value value to be set
	 * @return value
	 */
	T operator= (T const & value)
	{
		data = value;
		return data;
	}


	/*!
	 * Return string representation of current value
	 *
	 * @return string representation of current value.
	 */
	virtual std::string store() {
		return boost::lexical_cast<std::string>(data);
	}

	/*!
	 * Retrieve value from it's string representation.
	 *
	 * @param str string to retrieve value from.
	 */
	virtual void retrieve(const std::string & str) {
		data = boost::lexical_cast<T>(str);
	}



	/// Might be useful for template deductions
	typedef T value_type;

private:
	/// actual data
	T data;

};

}

#endif /* PROPERTY_HPP_ */
