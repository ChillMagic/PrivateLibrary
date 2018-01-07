// Name:    file.h
// Date:    08/25/2016
// Version: 2.0.0.0

#pragma once
#ifndef _SYSTEM_FILE_H_
#define _SYSTEM_FILE_H_
#include "macro.h"
#include "charptr.h"
#include "prints.h"
#include <cstdio>
#include <cstring>
#include <string>
#include <list>
#include <algorithm>

SYSTEM_BEGIN
class File
{
	using FilePtr = std::shared_ptr<FILE>;
public:
	enum TBMode {
		Text,      // t
		Binary,    // b
	};
	enum RWMode {
		Read,      // r
		ReadWrite, // r+
		OverWrite, // w
		Create,    // w+
		WAppend,   // a
		RWAppend,  // a+
	};

public:
	explicit File(TBMode tbmode = Text, RWMode rwmode = ReadWrite)
		: tbmode(tbmode), rwmode(rwmode) {}
	explicit File(const std::string &filename, TBMode tbmode = Text, RWMode rwmode = ReadWrite)
		: filename(filename), tbmode(tbmode), rwmode(rwmode) {
		priOpen();
	}
	explicit File(FILE *fp, TBMode tbmode = Text, RWMode rwmode = ReadWrite)
		: file(fp, [](FILE*) {}), tbmode(tbmode), rwmode(rwmode) {}

	File& open(const std::string &filename) {
		this->filename = filename;
		priOpen();
		return *this;
	}
	File& close() {
		file = nullptr;
		return *this;
	}
	File& reopen() {
		close();
		priOpen();
		return *this;
	}
	File& reopen(RWMode rwmode) {
		close();
		this->rwmode = rwmode;
		priOpen();
		return *this;
	}
	void write(const void *buffer, size_t elsize, size_t elcount) {
		fwrite(buffer, elsize, elcount, file.get());
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
		fread(buffer, elsize, elcount, file.get());
	}
	template <typename T>
	void read(T *buffer, size_t elcount) {
		read(buffer, sizeof(T), elcount);
	}
	template <typename T>
	void read(T &element) {
		read(&element, 1);
	}

	template <typename T>
	bool getft(T &element) {
		int s = fscanf(file.get(), Convert::format<T>(), &element);
		return s != EOF && s != 0;
	}
	bool getft(char *dst, size_t len) {
		int s = fscanf(file.get(), "%s", dst);
		return s != EOF && s != 0;
	}

	operator FILE*() const {
		return file.get();
	}
	bool bad() const {
		return file == nullptr;
	}
	size_t size() const {
		return _size;
	}
	bool eof() const {
		return feof(file.get()) != 0;
	}
	std::string getText() const {
		if (bad()) return "";
		FileRecordPost frecpost(file);
		charptr tmp(size());
		setPostBegin();
		fread(tmp, sizeof(char), size(), file.get());
		return tmp.to_string();
	}
	FILE* c_ptr() const {
		return file.get();
	}
	std::string getline() {
		std::list<std::unique_ptr<char>> strlist;
		const size_t size = 0x100;
		size_t length = 0;

		while (true) {
			strlist.push_back(std::unique_ptr<char>(new char[size]()));
			char *buffer = strlist.back().get();

			fgets(buffer, size, file.get());

			char c = buffer[size - 2];
			if (c == '\x00' || c == '\x0A') {
				size_t i = strlen(buffer) - 1;
				if (buffer[i] == '\n')
					buffer[i] = '\0';
				length += i;
				break;
			}
			else {
				length += size;
			}
		}

		std::string result(length + 1, '\0');

		if (result.empty())
			return "";

		auto iter = result.begin();

		for (auto &str : strlist) {
			size_t step = length < size ? length : size;
			std::copy_n(str.get(), step, iter);
			iter += step;
			length -= step;
		}

		return result;
	}

private:
	FilePtr file;
	std::string filename;
	TBMode tbmode;
	RWMode rwmode;
	size_t _size;

	void priOpen() {
		const std::string &mode = getMode(tbmode, rwmode);
		FILE *fp = fopen(filename.c_str(), mode.c_str());
		if (fp) {
			file = FilePtr(fp, fclose);
			setSize();
		}
	}
	void setSize() {
		long size;
		if (bad()) {
			size = -1;
		}
		else {
			FileRecordPost frecpost(file);
			setPostEnd();
			size = ftell(file.get());
		}
		this->_size = static_cast<size_t>(size);
	}
	void setPostBegin() const {
		fseek(file.get(), 0, SEEK_SET);
	}
	void setPostEnd() const {
		fseek(file.get(), 0, SEEK_END);
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

	static std::string getMode(TBMode tbmode, RWMode rwmode) {
		std::string str;
		char tbchar = tbmode == Text ? 't' : 'b';
		char rwchar;
		bool appchar = false;
		switch (rwmode)
		{
		case Read: rwchar = 'r'; break;
		case ReadWrite: rwchar = 'r'; appchar = true; break;
		case OverWrite: rwchar = 'w'; break;
		case Create: rwchar = 'w'; appchar = true; break;
		case WAppend: rwchar = 'a'; break;
		case RWAppend: rwchar = 'a'; appchar = true; break;
		}
		str.push_back(rwchar);
		str.push_back(tbchar);
		if (appchar)
			str.push_back('+');
		return str;
	}

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
SYSTEM_END

#endif