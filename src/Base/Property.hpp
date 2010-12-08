/*!
 * \file Property.hpp
 * \brief
 * \author mstefanc
 * \date Jul 27, 2010
 */

#ifndef PROPERTY_HPP_
#define PROPERTY_HPP_

#include <string>

namespace Base {

class PropertyInterface {
public:

	PropertyInterface(const std::string & n) : name(n) {}

	template <typename T>
	T get() {
		return &((T*)internalGet());
	}

	template <typename T>
	T set(const T & t) {
		internalSet(&t);
		return t;
	}

protected:
	virtual void internalSet(void * data) = 0;

	virtual void* internalGet() = 0;

private:
	/// property name
	std::string name;
};

/*!
 * Each property has three sets of redundant accessors:
 * 1. function call syntax
 * 2. get() and set() functions
 * 3. overloaded operator =
 * a read-write property with data store and
 * automatically generated get/set functions.
 */
template <class T>
class Property : public PropertyInterface
{
public:
	Property(const std::string& name) : PropertyInterface(name), data()
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

	operator T() const
	{
		return data;
	}

	T operator= (T const & value)
	{
		data = value;
		return data;
	}

	/// Might be useful for template deductions
	typedef T value_type;

protected:
	void internalSet(void * t) {
		data = *((T*)t);
	}

	void* internalGet() {
		return &data;
	}

private:
	/// actual data
	T data;

};

}

#endif /* PROPERTY_HPP_ */
