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
	StringView(const char *str) : _pointer(str) {}
	StringView(const std::string &str) : _pointer(str.data()) {}
	StringView(const PriLib::charptr &cp) : _pointer(cp.get()) {}

	const char* get(size_t offset = 0) const {
	    return _pointer + offset;
	}
	char operator[](size_t offset) {
	    return _pointer[offset];
	}
	StringView offset(size_t offset) {
		return StringView(_pointer + offset);
	}
	StringView offset(ptrdiff_t offset) {
		return StringView(_pointer + offset);
	}

private:
	const char *_pointer = nullptr;
};
PRILIB_END

#endif
