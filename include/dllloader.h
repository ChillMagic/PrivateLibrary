// dllloader.h
// * PrivateLibrary

#pragma once
#ifndef _PRILIB_DLLLOADER_H_
#define _PRILIB_DLLLOADER_H_
#include "macro.h"
#include <memory>

#define S_WINDOWS 1
#define S_LINUX   2

#if (defined _WIN32)
#	define SYSTEM_PLATFORM S_WINDOWS
#elif (defined __linux__)
#	define SYSTEM_PLATFORM S_LINUX
#endif

PRILIB_BEGIN
class DLLLoader
{
public:
#if SYSTEM_PLATFORM == S_WINDOWS
	using ErrorCode = unsigned long;
	using Mode = int;
#	ifdef UNICODE
	using Path = const wchar_t *;
#	else
	using Path = const char *;
#	endif
#elif SYSTEM_PLATFORM == S_LINUX
	using ErrorCode = const char *;
	using Path = const char *;
	enum Mode {
		Lazy,
		Now,
		Global,
		Local
	};
#else
#	error
#endif

public:
	class DLLPointer;

public:
	explicit DLLLoader();
#if SYSTEM_PLATFORM == S_WINDOWS
	explicit DLLLoader(Path path, Mode flag = 0);
	void open(Path name, Mode flag = 0);
#elif SYSTEM_PLATFORM == S_LINUX
	explicit DLLLoader(Path path, Mode flag = Lazy);
	void open(Path name, Mode flag = Lazy);
#endif

	void close();
	bool bad() const {
		return _data == nullptr;
	}

	template <typename T = void>
	T* get(const char *func_name) const {
		return reinterpret_cast<T*>(_get(func_name));
	}

	ErrorCode errcode() const {
		return _errcode;
	}

private:
	std::shared_ptr<DLLPointer> _data;
	ErrorCode _errcode;

	void* _get(const char *func_name) const;
};
PRILIB_END

#endif
