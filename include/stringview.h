// stringview.h
// * PrivateLibrary

#pragma once
#ifndef _PRILIB_STRINGVIEW_H_
#define _PRILIB_STRINGVIEW_H_
#include "macro.h"
#include "charptr.h"
#include <cassert>
#include <cstddef>

PRILIB_BEGIN
class CharPtrView
{
public:
	using OffsetType = size_t;
public:
	CharPtrView(const char *str, OffsetType offset = 0) : _pointer(str + offset) {}
	CharPtrView(const std::string &str, OffsetType offset = 0) : _pointer(str.data() + offset) {}
	CharPtrView(const charptr &cp, OffsetType offset = 0) : _pointer(cp.get() + offset) {}

	const char* get(OffsetType offset = 0) const {
		assert(_pointer);
		return _pointer + offset;
	}
	char operator[](OffsetType offset) const {
		assert(_pointer);
	    return _pointer[offset];
	}
	CharPtrView& operator+=(OffsetType offset) {
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

class StringView
{
public:
	using SizeType = size_t;
	using OffsetType = ptrdiff_t;
public:
	explicit StringView(const std::string &begin)
		: _data(begin), _size(begin.size()) {}

	explicit StringView(const std::string &begin, SizeType length)
		: _data(begin), _size(length) { assert(length <= begin.size()); }

	explicit StringView(const char *begin)
		: _data(begin), _size(std::strlen(begin)) {}

	explicit StringView(const char *begin, SizeType length)
		: _data(begin), _size(length) {}

	explicit StringView(const CharPtrView &strview, SizeType length)
		: _data(strview), _size(length) {}

	explicit StringView(const CharPtrView &strview, SizeType begin, SizeType end)
		: _data(strview.get() + begin), _size(end - begin) {}

	explicit StringView(const CharPtrView &begin, const CharPtrView &end)
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
		assert((offset >= 0 && static_cast<SizeType>(offset) < _size) || (offset < 0 && static_cast<SizeType>(-offset) <= _size));
		return (offset >= 0) ? (_data[offset]) : (_data[_size + offset]);
	}

private:
	CharPtrView _data;
	SizeType _size = 0;
};
PRILIB_END

#endif
