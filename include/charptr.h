// Name:	charptr.h
// Date:    06/15/2016
// Version: 2.1.0.0

#pragma once
#ifndef _SYSTEM_CHARPTR_H_
#define _SYSTEM_CHARPTR_H_
#include "macro.h"
#include "memory.h"
#include <string>
#include <memory>

SYSTEM_BEGIN
class charptr
{
public:
	charptr() : _capacity(0), data(new char('\0')) {}
	explicit charptr(size_t len) : _capacity(len + 1), data(Memory::new_<char>(_capacity), Memory::delete_<char>) {}
	explicit charptr(const char *ptr, size_t len) : _capacity(len + 1), data(Memory::copyOf(ptr, _capacity), Memory::delete_<char>) {}
	explicit charptr(const char *begin, const char *end) : charptr(begin, end - begin) {}
	explicit charptr(const std::string &str) : charptr(str.c_str(), str.length()) {}
	operator char*() { return data.get(); }
	operator const char*() const { return data.get(); }
	char& operator[](size_t id) { return data.get()[id]; }
	const char& operator[](size_t id) const { return data.get()[id]; }
	void set(size_t begin, const char *str) { Memory::copyTo(data.get() + begin, str, strlen(str)); } // Unsafe
	std::string to_string() const { return data.get(); }
	charptr* clone() const { return new charptr(data.get(), _capacity); }
	size_t capacity() const { return _capacity; }
	size_t length() const { size_t len = strlen(data.get()); return len < _capacity ? len : _capacity; }

private:
	size_t _capacity;
	std::shared_ptr<char> data;
};
SYSTEM_END

#endif