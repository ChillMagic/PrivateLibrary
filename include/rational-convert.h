// rational-convert.h
// * PrivateLibrary

#pragma once
#ifndef _PRILIB_RATIONAL_CONVERT_H_
#define _PRILIB_RATIONAL_CONVERT_H_
#include "macro.h"
#include "rational.h"

PRILIB_BEGIN
namespace Number
{
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
