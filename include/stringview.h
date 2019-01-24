// stringview.h
// * PrivateLibrary

#pragma once
#ifndef _PRILIB_STRINGVIEW_H_
#define _PRILIB_STRINGVIEW_H_
#include "macro.h"
#include "charptr.h"

PRILIB_BEGIN
class StringView
{
public:
	StringView(const char *str, size_t length = 0) : _pointer(str), _length(length) {}
	StringView(const std::string &str) : _pointer(str.data()), _length(str.length()) {}
	StringView(const PriLib::charptr &cp) : _pointer(cp.get()), _length(cp.length()) {}

	size_t length() const {
		if (_length == 0 && _pointer) {
			return std::strlen(_pointer);
		}
		return _length;
	}
	size_t size() const {
		return length();
	}
	const char* get(size_t offset = 0) const {
	    return _pointer + offset;
	}
	char operator[](size_t offset) {
	    return _pointer[offset];
	}

private:
	const char *_pointer = nullptr;
	mutable size_t _length = 0;
};
PRILIB_END

#endif
