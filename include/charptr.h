// charptr.h
// * PrivateLibrary

#pragma once
#ifndef _PRILIB_CHARPTR_H_
#define _PRILIB_CHARPTR_H_
#include "macro.h"
#include "memory.h"
#include <string>
#include <memory>

PRILIB_BEGIN
class charptr
{
public:
	explicit charptr() : _capacity(0), _data(new char('\0')) {}
	explicit charptr(size_t len) : _capacity(len + 1), _data(Memory::new_<char>(_capacity), Memory::delete_<char>) {}
	explicit charptr(const char *ptr, size_t len) : _capacity(len + 1), _data(Memory::copyOf(ptr, _capacity), Memory::delete_<char>) {}
	explicit charptr(const char *begin, const char *end) : charptr(begin, end - begin) {}
	explicit charptr(const std::string &str) : charptr(str.c_str(), str.length()) {}

	operator char*() { return _data.get(); }
	operator const char*() const { return _data.get(); }
	char& operator[](size_t id) { return _data.get()[id]; }
	const char& operator[](size_t id) const { return _data.get()[id]; }
	char* get() const { return _data.get(); }
	void set(size_t begin, const char *str) { Memory::copyTo(_data.get() + begin, str, strlen(str)); } // Unsafe
	void set(size_t begin, const std::string &str) { Memory::copyTo(_data.get() + begin, str.data(), str.size()); } // Unsafe
	std::string str() const { return _data.get(); }
	size_t capacity() const { return _capacity; }
	size_t length() const { size_t len = strlen(_data.get()); return len < _capacity ? len : _capacity; }

	std::string to_string() const { return str(); }

private:
	size_t _capacity;
	std::shared_ptr<char> _data;
};
PRILIB_END

#endif
