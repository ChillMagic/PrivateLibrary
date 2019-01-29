// stringview.h
// * PrivateLibrary

#pragma once
#ifndef _PRILIB_STRINGVIEW_H_
#define _PRILIB_STRINGVIEW_H_
#include "macro.h"
#include "charptr.h"
#include <cassert>

PRILIB_BEGIN
class StringView
{
public:
	using OffsetType = size_t;
public:
	StringView(const char *str, OffsetType offset = 0) : _pointer(str + offset) {}
	StringView(const std::string &str, OffsetType offset = 0) : _pointer(str.data() + offset) {}
	StringView(const PriLib::charptr &cp, OffsetType offset = 0) : _pointer(cp.get() + offset) {}

	const char* get(OffsetType offset = 0) const {
		assert(_pointer);
	    return _pointer + offset;
	}
	char operator[](OffsetType offset) const {
		assert(_pointer);
	    return _pointer[offset];
	}
	StringView& operator+=(OffsetType offset) {
		assert(_pointer);
		this->_pointer += offset;
		return *this;
	}
	std::string toString() const {
		return std::string(_pointer);
	}
	std::string toSubString(OffsetType offset = 0) const {
		return std::string(_pointer, _pointer + offset);
	}

private:
	const char *_pointer = nullptr;
};

inline std::string toSubString(const StringView &lhs, const StringView &rhs) {
	assert(lhs.get() < rhs.get());
	return lhs.toSubString(rhs.get() - lhs.get());
}
PRILIB_END

#endif
