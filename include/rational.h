// rational.h
// * PrivateLibrary

#pragma once
#ifndef _PRILIB_RATIONAL_H_
#define _PRILIB_RATIONAL_H_
#include "macro.h"
#include <cmath>
#include <string>
#include <algorithm>

PRILIB_BEGIN
namespace Number
{
	template <typename T>
	class Rational
	{
	public:
		using Type = T;

	public:
		Rational() : Rational(0, 1) {};
		Rational(const Type &num) : Rational(num, 1) {};
		Rational(const Type &num, const Type &den) { initialize(num, den); };

	public:
		Rational& operator=(const Rational &r) {
			this->_numerator = r._numerator;
			this->_denominator = r._denominator;
			return *this;
		}

		Rational& operator+=(const Rational &r) {
			return initialize(_numerator * r._denominator + r._numerator * _denominator, _denominator * r._denominator);
		}
		Rational& operator-=(const Rational &r) {
			return initialize(_numerator * r._denominator - r._numerator * _denominator, _denominator * r._denominator);
		}
		Rational& operator*=(const Rational &r) {
			return initialize(_numerator * r._numerator, _denominator * r._denominator);
		}
		Rational& operator/=(const Rational &r) {
			return initialize(_numerator * r._denominator, _denominator * r._numerator);
		}

		friend const Rational operator+(const Rational &lhs, const Rational &rhs) { return Rational(lhs) += rhs; }
		friend const Rational operator-(const Rational &lhs, const Rational &rhs) { return Rational(lhs) -= rhs; }
		friend const Rational operator*(const Rational &lhs, const Rational &rhs) { return Rational(lhs) *= rhs; }
		friend const Rational operator/(const Rational &lhs, const Rational &rhs) { return Rational(lhs) /= rhs; }

		friend bool operator==(const Rational &lhs, const Rational &rhs) { return lhs._equal(rhs); }
		friend bool operator!=(const Rational &lhs, const Rational &rhs) { return !lhs._equal(rhs); }
		friend bool operator>(const Rational &lhs, const Rational &rhs) { return lhs._compare(rhs) > 0; }
		friend bool operator>=(const Rational &lhs, const Rational &rhs) { return lhs._compare(rhs) >= 0; }
		friend bool operator<(const Rational &lhs, const Rational &rhs) { return lhs._compare(rhs) < 0; }
		friend bool operator<=(const Rational &lhs, const Rational &rhs) { return lhs._compare(rhs) <= 0; }

		friend const Rational operator+(const Rational &r) { return r; }
		friend const Rational operator-(const Rational &r) { return Rational(-r._numerator, r._denominator); }
		friend const Rational operator~(const Rational &r) { return Rational(r._denominator, r._numerator); }

	public:
		friend Type floor(const Rational &r) { return r.floor(); }
		friend Type ceil(const Rational &r) { return r.ceil(); }
		friend Type fix(const Rational &r) { return r.fix(); }

		friend const Rational abs(const Rational &a) {
			using std::abs;

			return Rational(abs(a.num()), a.den());
		}

		friend const Rational mod(const Rational &lhs, const Rational &rhs) {
			return lhs - (rhs * (lhs / rhs).floor());
		}
		friend const Rational rem(const Rational &lhs, const Rational &rhs) {
			return  lhs - (rhs * (lhs / rhs).fix());
		}

	public:
		const Type& num() const { return _numerator; }
		const Type& den() const { return _denominator; }

	public:
		bool isNaN() const {
			return (_denominator == 0 && _numerator == 0) || (_denominator < 0);
		}
		bool isInf() const {
			return (_denominator == 0 && _numerator != 0);
		}
		bool isInt() const {
			return _denominator == 1;
		}
		bool isPositive() const {
			return _numerator > 0;
		}
		bool isZero() const {
			return _numerator == 0;
		}
		bool isNegative() const {
			return _numerator < 0;
		}

	public:
		Type floor() const {
			if (isInf())
				return std::numeric_limits<Type>::max();
			else if (isNaN())
				return Type(0);
			else if (isZero())
				return Type(0);
			else if (isInt())
				return _numerator;
			else
				return isPositive() ? (_numerator / _denominator) : (_numerator / _denominator - 1);
		}
		Type ceil() const {
			if (isInf())
				return std::numeric_limits<Type>::max();
			else if (isNaN())
				return Type(0);
			else if (isZero())
				return Type(0);
			else if (isInt())
				return _numerator;
			else
				return isPositive() ? (_numerator / _denominator + 1) : (_numerator / _denominator);
		}
		Type fix() const {
			return isPositive() ? floor() : ceil();
		}

	private:
		Type _numerator;
		Type _denominator;

	private:
		Rational& initialize(const Type &num, const Type &den) {
			Type g = gcd(num, den);
			_numerator = num / g;
			_denominator = den / g;
			if (_denominator < 0) {
				_denominator = -_denominator;
				_numerator = -_numerator;
			}
			return *this;
		}

		static Type gcd(Type a, Type b) {
			while (b != 0) {
				a = a % b;
				std::swap(a, b);
			}
			return a;
		}

	private:
		bool _equal(const Rational &r) const {
			return r._numerator == this->_numerator && r._denominator == this->_denominator;
		}
		Type _compare(const Rational &r) const {
			return this->_numerator * r._denominator - r._numerator * this->_denominator;
		}
	};

	template <typename T>
	inline std::string to_string(const Rational<T> &a) {
		using std::to_string;

		if (a.isInt())
			return to_string(a.num());
		else if (a.isInf())
			return "Inf";
		else if (a.isNaN())
			return "NaN";
		else
			return to_string(a.num()) + "/" + to_string(a.den());
	}
}
PRILIB_END

#endif
