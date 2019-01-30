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

class StringViewRange
{
public:
	using SizeType = size_t;
	using OffsetType = ptrdiff_t;
public:
	explicit StringViewRange(const std::string &begin)
		: _data(begin), _size(begin.size()) {}

	explicit StringViewRange(const std::string &begin, SizeType length)
		: _data(begin), _size(length) { assert(length <= begin.size()); }

	explicit StringViewRange(const char *begin)
		: _data(begin), _size(std::strlen(begin)) {}

	explicit StringViewRange(const char *begin, SizeType length)
		: _data(begin), _size(length) {}

	explicit StringViewRange(const StringView &strview, SizeType length)
		: _data(strview), _size(length) {}

	explicit StringViewRange(const StringView &begin, const StringView &end)
		: _data(begin), _size(end.get() - begin.get()) { assert(end.get() >= begin.get()); }

	std::string toString() const {
		return _data.toSubString(_size);
	}

	const char* begin() const {
		return _data.get();
	}
	const char* end() const {
		return _data.get(_size);
	}

	SizeType length() const {
		return _size;
	}

	SizeType size() const {
		return _size;
	}

	char operator[](OffsetType offset) const {
		assert((offset >= 0 && offset < _size) || (offset < 0 && -offset <= _size));
		return (offset >= 0) ? (_data[offset]) : (_data[_size + offset]);
	}

private:
	StringView _data;
	SizeType _size = 0;
};
PRILIB_END

#endif
