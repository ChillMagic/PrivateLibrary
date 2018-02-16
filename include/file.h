// file.h
// * PrivateLibrary

#pragma once
#ifndef _PRILIB_FILE_H_
#define _PRILIB_FILE_H_
#include "macro.h"
#include "convert.h"
#include <string>
#include <memory>

PRILIB_BEGIN
class File
{
	using FilePtr = std::shared_ptr<FILE>;
public:
	enum TBMode {
		Text,      // t
		Binary,    // b
	};
	enum RWMode {
		Read,          // r
		Write,         // w
		Append,        // a
		ReadWrite,     // r+
		ReadOverWrite, // w+
		ReadAppend,    // a+
	};

public:
	explicit File()
		: _file(nullptr), _size(0) {}

	explicit File(FILE *fp, bool exclusive = false);

	explicit File(const std::string &filename, TBMode tbmode, RWMode rwmode) {
		_priOpen(filename, tbmode, rwmode);
	}

	File& open(const std::string &filename, TBMode tbmode, RWMode rwmode) {
		_priOpen(filename, tbmode, rwmode);
		return *this;
	}
	File& close() {
		_file = nullptr;
		return *this;
	}

	operator FILE*() const {
		return _file.get();
	}
	bool bad() const {
		return _file == nullptr;
	}
	FILE* c_ptr() const {
		return _file.get();
	}

	size_t size() const;
	bool eof() const;

protected:
	FilePtr _file;
	size_t _size;

	void _priOpen(const std::string &filename, TBMode tbmode, RWMode rwmode);
	void _setSize();
	void _setPostBegin() const;
	void _setPostEnd() const;

	static void _getMode(char mode[4], TBMode tbmode, RWMode rwmode);
};

class TextFile : public File
{
public:
	explicit TextFile() = default;

	explicit TextFile(FILE *fp, bool exclusive = false)
		: File(fp, exclusive) {}

	explicit TextFile(const std::string &filename, File::RWMode rwmode = Read)
		: File(filename, File::Text, rwmode) {}

	explicit TextFile(const File &file)
		: File(file) {}

	TextFile& open(const std::string &filename, RWMode rwmode) {
		File::open(filename, File::Text, rwmode);
		return *this;
	}
	TextFile& close() {
		File::close();
		return *this;
	}

	std::string getline();

	std::string getText() const;

	template <typename T>
	bool getfmt(T &element) {
		return _getfmt(Convert::format<T>(), &element);
	}
	bool getfmt(char *dst, size_t len);

	void write(const std::string &str);

private:
	bool _getfmt(const char *fmt, void *dst);
};

class BinaryFile : public File
{
public:
	explicit BinaryFile() = default;

	explicit BinaryFile(FILE *fp, bool exclusive = false)
		: File(fp, exclusive) {}

	explicit BinaryFile(const std::string &filename, File::RWMode rwmode = Read)
		: File(filename, File::Binary, rwmode) {}
	explicit BinaryFile(const File &file)
		: File(file) {}

	BinaryFile& open(const std::string &filename, RWMode rwmode) {
		File::open(filename, File::Binary, rwmode);
		return *this;
	}
	BinaryFile& close() {
		File::close();
		return *this;
	}

	void write(const void *buffer, size_t elsize, size_t elcount);
	void read(void *buffer, size_t elsize, size_t elcount);

	template <typename T>
	void write(const T *buffer, size_t elcount) {
		write(buffer, sizeof(T), elcount);
	}
	template <typename T>
	void write(const T &element) {
		write(&element, 1);
	}

	template <typename T>
	void read(T *buffer, size_t elcount) {
		read(buffer, sizeof(T), elcount);
	}
	template <typename T>
	void read(T &element) {
		read(&element, 1);
	}
};
PRILIB_END

#endif
