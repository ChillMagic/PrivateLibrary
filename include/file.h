// Name:    file.h
// Date:    08/25/2016
// Version: 2.0.0.0

#pragma once
#ifndef _PRILIB_FILE_H_
#define _PRILIB_FILE_H_
#include "macro.h"
#include "charptr.h"
#include "prints.h"
#include <cstdio>
#include <cstring>
#include <string>
#include <list>
#include <algorithm>

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

	explicit File(FILE *fp)
		: _file(fp, [](FILE*) {}) {}

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
	size_t size() const {
		return _size;
	}
	bool eof() const {
		return feof(_file.get()) != 0;
	}
	FILE* c_ptr() const {
		return _file.get();
	}

protected:
	FilePtr _file;
	size_t _size;

	void _priOpen(const std::string &filename, TBMode tbmode, RWMode rwmode);
	void _setSize();
	void _setPostBegin() const {
		fseek(_file.get(), 0, SEEK_SET);
	}
	void _setPostEnd() const {
		fseek(_file.get(), 0, SEEK_END);
	}

	class FileRecordPost
	{
	public:
		FileRecordPost(const FilePtr &file) : file(file) {
			fgetpos(file.get(), &recpost);
		}
		~FileRecordPost() {
			fsetpos(file.get(), &recpost);
		}
	private:
		fpos_t recpost;
		const FilePtr &file;
	};

	static void _getMode(char mode[4], TBMode tbmode, RWMode rwmode);

#ifdef _MSC_VER
	inline static FILE* fopen(const char *filename, const char *mode)
	{
		FILE *file;
		fopen_s(&file, filename, mode);
		return file;
	}
	template <typename... Args>
	inline static int fscanf(FILE *file, const char *format, Args... args)
	{
		return fscanf_s(file, format, args...);
	}
#endif
};

class TextFile : public File
{
public:
	explicit TextFile(const std::string &filename, File::RWMode rwmode = ReadWrite)
		: File(filename, File::Text, rwmode) {}

	explicit TextFile(const File &file)
		: File(file) {}

	std::string getline();

	std::string getText() const;

	template <typename T>
	bool getft(T &element) {
		int s = fscanf(_file.get(), Convert::format<T>(), &element);
		return s != EOF && s != 0;
	}
	bool getft(char *dst, size_t len) {
		int s = fscanf(_file.get(), "%s", dst);
		return s != EOF && s != 0;
	}
};

class BinaryFile : public File
{
public:
	explicit BinaryFile(const std::string &filename, File::RWMode rwmode = ReadWrite)
		: File(filename, File::Binary, rwmode) {}
	explicit BinaryFile(const File &file)
		: File(file) {}

	void write(const void *buffer, size_t elsize, size_t elcount) {
		fwrite(buffer, elsize, elcount, _file.get());
	}
	template <typename T>
	void write(const T *buffer, size_t elcount) {
		write(buffer, sizeof(T), elcount);
	}
	template <typename T>
	void write(const T &element) {
		write(&element, 1);
	}
	void read(void *buffer, size_t elsize, size_t elcount) {
		fread(buffer, elsize, elcount, _file.get());
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
