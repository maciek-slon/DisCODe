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

	PropertyInterface(const std::string & n) : name(n), stored(true) {}

	template <typename T>
	T get() {
		return &((T*)internalGet());
	}

	template <typename T>
	T set(const T & t) {
		internalSet(&t);
		return t;
	}



	bool isStored() {
		return stored;
	}

	virtual std::string store() = 0;

	virtual void retrieve(const std::string & str) = 0;



protected:
	virtual void internalSet(void * data) = 0;

	virtual void* internalGet() = 0;

private:
	/// property name
	std::string name;

	bool stored;
};

/*!
 *
 */
template < class T >
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
