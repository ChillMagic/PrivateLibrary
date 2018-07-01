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

	//--------------------
	// * Class Rational
	//--------------------
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
			this->numerator = r.numerator;
			this->denominator = r.denominator;
			return *this;
		}

		Rational& operator+=(const Rational &r) {
			return initialize(numerator * r.denominator + r.numerator * denominator, denominator * r.denominator);
		}

		Rational& operator-=(const Rational &r) {
			return initialize(numerator * r.denominator - r.numerator * denominator, denominator * r.denominator);
		}

		Rational& operator*=(const Rational &r) {
			return initialize(numerator * r.numerator, denominator * r.denominator);
		}

		Rational& operator/=(const Rational &r) {
			return initialize(numerator * r.denominator, denominator * r.numerator);
		}

		friend const Rational operator+(const Rational &lhs, const Rational &rhs) { return Rational(lhs) += rhs; }
		friend const Rational operator-(const Rational &lhs, const Rational &rhs) { return Rational(lhs) -= rhs; }
		friend const Rational operator*(const Rational &lhs, const Rational &rhs) { return Rational(lhs) *= rhs; }
		friend const Rational operator/(const Rational &lhs, const Rational &rhs) { return Rational(lhs) /= rhs; }

		friend bool operator==(const Rational &lhs, const Rational &rhs) { return lhs.equal(rhs); }
		friend bool operator!=(const Rational &lhs, const Rational &rhs) { return !lhs.equal(rhs); }
		friend bool operator>(const Rational &lhs, const Rational &rhs) { return lhs.compare(rhs) > 0; }
		friend bool operator>=(const Rational &lhs, const Rational &rhs) { return lhs.compare(rhs) >= 0; }
		friend bool operator<(const Rational &lhs, const Rational &rhs) { return lhs.compare(rhs) < 0; }
		friend bool operator<=(const Rational &lhs, const Rational &rhs) { return lhs.compare(rhs) <= 0; }

		friend const Rational operator+(const Rational &r) { return r; }
		friend const Rational operator-(const Rational &r) { return Rational(-r.numerator, r.denominator); }
		friend const Rational operator~(const Rational &r) { return Rational(r.denominator, r.numerator); }

	public:
		friend Type floor(const Rational &r) { return r.floor(); }
		friend Type ceil(const Rational &r) { return r.ceil(); }
		friend Type fix(const Rational &r) { return r.fix(); }

		friend const Rational abs(const Rational &a) {
			using std::abs;

			return Rational(abs(a.getNum()), a.getDen());
		}

		friend const Rational mod(const Rational &lhs, const Rational &rhs) {
			return lhs - (rhs * (lhs / rhs).floor());
		}

		friend const Rational rem(const Rational &lhs, const Rational &rhs) {
			return  lhs - (rhs * (lhs / rhs).fix());
		}

	public:
		const Type& getNum() const { return numerator; }
		const Type& getDen() const { return denominator; }

	public:
		bool isNaN() const {
			return (denominator == 0 && numerator == 0) || (denominator < 0);
		}
		bool isInf() const {
			return (denominator == 0 && numerator != 0);
		}
		bool isInt() const {
			return denominator == 1;
		}
		bool isPositive() const {
			return numerator > 0;
		}
		bool isZero() const {
			return numerator == 0;
		}
		bool isNegative() const {
			return numerator < 0;
		}

	private:
		Type numerator;
		Type denominator;

	private:
		Rational & initialize(const Type &num, const Type &den) {
			Type g = gcd(num, den);
			numerator = num / g;
			denominator = den / g;
			adjust(numerator, denominator);
			return *this;
		}

		static Type gcd(Type a, Type b) {
			while (b != 0) {
				a = a % b;
				std::swap(a, b);
			}
			return a;
		}

		static void adjust(Type &num, Type &den) {
			bool _isnegative = false;
			if (num < 0) {
				num = -num;
				_isnegative = !_isnegative;
			}
			if (den < 0) {
				den = -den;
				_isnegative = !_isnegative;
			}

			if (_isnegative) {
				num = -num;
			}
		}

	private:
		bool equal(const Rational &r) const {
			return r.numerator == this->numerator && r.denominator == this->denominator;
		}
		Type compare(const Rational &r) const {
			return this->numerator * r.denominator - r.numerator * this->denominator;
		}

		Type floor() const {
			if (isInf())
				return std::numeric_limits<Type>::max();
			else if (isNaN())
				return Type(0);
			else if (isZero())
				return Type(0);
			else if (isInt())
				return numerator;
			else
				return isPositive() ? (numerator / denominator) : (numerator / denominator - 1);
		}

		Type ceil() const {
			if (isInf())
				return std::numeric_limits<Type>::max();
			else if (isNaN())
				return Type(0);
			else if (isZero())
				return Type(0);
			else if (isInt())
				return numerator;
			else
				return isPositive() ? (numerator / denominator + 1) : (numerator / denominator);
		}

		Type fix() const {
			return isPositive() ? floor() : ceil();
		}
	};

	template <typename T>
	inline std::string to_string(const Rational<T> &a) {
		using std::to_string;

		if (a.isInt())
			return to_string(a.getNum());
		else if (a.isInf())
			return "Inf";
		else if (a.isNaN())
			return "NaN";
		else
			return to_string(a.getNum()) + "/" + to_string(a.getDen());
	}

	//--------------------
	// * Convert
	//--------------------
	
	template <typename T>
	inline Rational<T> get_num(const std::string &str)
	{
		if (str.size() > 18)
			return Rational<T>(0, 0);
		else
			return Rational<T>(std::stoll(str));
	}

	template <typename T>
	inline Rational<T> to_rational(const std::string &);

	template <>
	inline Rational<long long> to_rational(const std::string &str)
	{
		using Type = long long;

		size_t i1 = str.find('/');
		size_t i2 = str.find('.');

		if (i1 != std::string::npos) {
			if (i1 == str.rfind('/') && i2 == std::string::npos)
				return Rational<Type>(std::stoi(str.substr(0, i1)), std::stoi(str.substr(i1 + 1, std::string::npos)));
		}
		else {
			if (i2 != std::string::npos) {
				if (i2 == str.rfind('.')) {
					Rational<Type> n = get_num<Type>(str.substr(0, i2));
					std::string substr(str.substr(i2 + 1, std::string::npos));
					Rational<Type> r = get_num<Type>(substr);
					Type m = (Type)powl((long double)10, (long double)substr.size());
					return r / m + n;
				}
			}
			else {
				return get_num<Type>(str);
			}
		}
		return Rational<Type>(0, 0);
	}
}
PRILIB_END

#endif
