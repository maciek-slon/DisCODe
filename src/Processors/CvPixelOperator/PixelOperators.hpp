/*!
 * \file PixelOperators.hpp
 * \brief Different pixel operators
 * \date Aug 27, 2010
 */

#ifndef PIXELOPERATORS_HPP_
#define PIXELOPERATORS_HPP_

#include "Props.hpp"

template <typename T>
class PixelOperator : public Base::Props{
public:
	virtual T operator()(const T& val) = 0;

	void save(ptree & pt) {

	}
};

template <typename T>
class UnitOperator : public PixelOperator<T> {
public:
	virtual T operator()(const T& val) {
		return val;
	}

	virtual void load(const ptree & pt) {

	}
};

template <typename T>
class TanhOperator : public PixelOperator<T> {
public:
	virtual T operator()(const T& val) {
		return fabs( tanh( sigma * (val - mean) ) );
	}

	virtual void load(const ptree & pt) {
		sigma = pt.get("sigma", 0.1);
		mean = pt.get("mean", 0.0);
	}

private:
	double sigma;
	double mean;
};


template <typename T>
class AndOperator : public PixelOperator<T> {
public:
	virtual T operator()(const T& val) {
		return val & mask;
	}

	virtual void load(const ptree & pt) {
		mask = pt.get("mask", 0xFF);
	}

private:
	T mask;
};

template <typename T>
class OrOperator : public PixelOperator<T> {
public:
	virtual T operator()(const T& val) {
		return val | mask;
	}

	virtual void load(const ptree & pt) {
		mask = pt.get("mask", 0xFF);
	}

private:
	T mask;
};

#endif /* PIXELOPERATORS_HPP_ */
