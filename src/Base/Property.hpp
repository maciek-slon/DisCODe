/*!
 * \file Property.hpp
 * \brief
 */

#ifndef PROPERTY_HPP_
#define PROPERTY_HPP_

#include <string>

#include <boost/lexical_cast.hpp>
#include <boost/function.hpp>

#include <typeinfo>
#include <vector>

namespace Base {

class PropertyInterface {
public:

	PropertyInterface(const std::string & n) : name_(n), persistent(true), m_tool_tip(n) {}

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

	virtual std::string type() const = 0;

	std::string toolTip() {
		return m_tool_tip;
	}

	void setToolTip(const std::string & tool_tip) {
		m_tool_tip = tool_tip;
	}

	void addConstraint(const std::string & str) {
		m_constraints.push_back(str);
	}

	int countConstraints() const {
		return m_constraints.size();
	}

	std::string getConstraint(int i) const {
		return m_constraints[i];
	}

private:
	/// property name
	std::string name_;

	bool persistent;

	std::string m_tool_tip;

	std::vector<std::string> m_constraints;
};

template <typename T>
class LexicalTranslator {
public:
	static std::string toStr(const T & val) {
		return boost::lexical_cast<std::string>(val);
	}

	static T fromStr(const std::string & str) {
		return boost::lexical_cast<T>(str);
	}
};

/*!
 *
 */
template < class T , class Translator = LexicalTranslator<T> >
class Property : public PropertyInterface
{
public:
	Property(const std::string& name, const T & initializer = T(), std::string type = typeid(T).name() ) : PropertyInterface(name), data(initializer), m_type(type)
	{
	}

	Property(const std::string& name, boost::function<void(T, T)> callback, const T & initializer = T(), std::string type = typeid(T).name()) : PropertyInterface(name), data(initializer), m_onChange(callback),  m_type(type) {
	}

	void setCallback(boost::function<void(T, T)> callback) {
		m_onChange = callback;
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
		return Translator::toStr(data);
	}

	/*!
	 * Retrieve value from it's string representation.
	 *
	 * @param str string to retrieve value from.
	 */
	virtual void retrieve(const std::string & str) {
		T old = data;
		data = Translator::fromStr(str);
		if (m_onChange)
			m_onChange(old, data);
	}

	std::string type() const {
		return m_type;
	}

	/// Might be useful for template deductions
	typedef T value_type;

protected:
	/// actual data
	T data;

	/// callback on data change
	boost::function<void(T, T)> m_onChange;

	std::string m_type;

};


}

#endif /* PROPERTY_HPP_ */
