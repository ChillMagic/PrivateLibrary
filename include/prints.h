// prints.h
// * PrivateLibrary
// * Description:  Basic on 'to_string', using 'printf'.

#pragma once
#ifndef _PRILIB_PRINTS_H_
#define _PRILIB_PRINTS_H_
#include "macro.h"
#include "convert.h"
#include <cstdio>
#include <string>

PRILIB_BEGIN
namespace Output
{
	// Print
	template <typename T>
	inline void print(const T &obj)
	{
		using Convert::to_string;
		using std::to_string;
		std::printf("%s", to_string(obj).c_str());
	}
	inline void print(const char *str)
	{
		std::printf("%s", str);
	}
	template <typename First, typename... Rest>
	inline void print(const First& first, const Rest&... rest)
	{
		print(first);
		print(rest...);
	}

	// Pintln
	inline void println()
	{
		std::printf("\n");
	}
	template <typename T>
	inline void println(const T &obj)
	{
		print<T>(obj);
		println();
	}
	template <typename First, typename... Rest>
	inline void println(const First& first, const Rest&... rest)
	{
		print(first);
		print(rest...);
		println();
	}

	// PutError
	inline void putError(const char *msg, int errcode = -1)
	{
		std::fprintf(stderr, "%s\n", msg);
		exit(errcode);
	}
}
PRILIB_END

#endif
