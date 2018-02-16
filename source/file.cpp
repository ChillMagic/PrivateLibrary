#include "file.h"
#include "charptr.h"
#include <list>
#include <algorithm>
#include <cassert>

PRILIB_BEGIN
#ifdef _MSC_VER
inline static FILE* fopen(const char *filename, const char *mode)
{
	FILE *file;
	fopen_s(&file, filename, mode);
	return file;
}
#endif

//========================
// * File
//========================

class FileRecordPost
{
public:
	explicit FileRecordPost(FILE *file) : file(file) {
		fgetpos(file, &recpost);
	}
	~FileRecordPost() {
		fsetpos(file, &recpost);
	}
private:
	fpos_t recpost;
	FILE *file;
};


File::File(FILE *fp, bool exclusive) {
	if (exclusive) {
		_file = FilePtr(fp, fclose);
	}
	else {
		_file = FilePtr(fp, [](FILE*) {});
	}
}

size_t File::size() const {
	return _size;
}
bool File::eof() const {
	assert(!bad());
	return feof(_file.get()) != 0;
}

void File::_priOpen(const std::string &filename, TBMode tbmode, RWMode rwmode) {
	char mode[4];
	_getMode(mode, tbmode, rwmode);
	FILE *fp = fopen(filename.c_str(), mode);
	if (fp) {
		_file = FilePtr(fp, fclose);
		_setSize();
	}
}

void File::_setSize() {
	long size;
	if (bad()) {
		size = 0;
	}
	else {
		FileRecordPost frecpost(_file.get());
		_setPostEnd();
		size = ftell(_file.get());
	}
	this->_size = static_cast<size_t>(size);
}

inline void File::_setPostBegin() const {
	fseek(_file.get(), 0, SEEK_SET);
}

inline void File::_setPostEnd() const {
	fseek(_file.get(), 0, SEEK_END);
}

void File::_getMode(char mode[4], TBMode tbmode, RWMode rwmode) {
	// mode : 0 rw, 1 tb, 2 +\0, 3 \0
	char &rwchar = mode[0];
	char &tbchar = mode[1];
	char &apchar = mode[2];
	char &edchar = mode[3];

	tbchar = tbmode == Text ? 't' : 'b';
	apchar = '\0';
	switch (rwmode)
	{
	case ReadWrite: apchar = '+';
	case Read: rwchar = 'r'; break;

	case ReadOverWrite: apchar = '+';
	case Write: rwchar = 'w'; break;

	case ReadAppend: apchar = '+';
	case Append: rwchar = 'a'; break;

	default:
		fputs("No corresponding file mode.\n", stderr);
		exit(1);
	}
	edchar = '\0';
}

//========================
// * TextFile
//========================

std::string TextFile::getline() {
	std::list<charptr> strlist;
	const size_t size = 0x100;
	size_t length = 0;

	while (true) {
		strlist.push_back(charptr(size));
		charptr &buffer = strlist.back();

		fgets(buffer.get(), size + 1, _file.get());

		// Line:
		//  Linux:   \n
		//  Windows: \r\n

		char c = buffer[size - 1];
		if (c == '\0' || c == '\n') {
			length += buffer.length();
			break;
		}
		else {
			length += size;
		}
	}

	if (length == 0)
		return "";

	charptr result(length); // The std::string end without '\0'
	size_t reclen = length;

	auto iter = result.get();

	for (auto &str : strlist) {
		size_t step = reclen < size ? reclen : size;
		std::memcpy(iter, str.get(), step);
		iter += step;
		reclen -= step;
	}

	// Remove \n, \r
	if (result[length - 1] == '\n')
		result[length - 1] = '\0';
	if (length >= 2 && result[length - 2] == '\r')
		result[length - 2] = '\0';

	return result.get(); // Remove '\0
}

std::string TextFile::getText() const {
	if (bad()) return "";
	FileRecordPost frecpost(_file.get());
	charptr tmp(size());
	_setPostBegin();
	fread(tmp, sizeof(char), size(), _file.get());
	return tmp.to_string();
}
bool TextFile::getfmt(char *dst, size_t len) {
#if defined(_MSC_VER)
	assert(len <= UINT8_MAX);
	int s = fscanf_s(_file.get(), "%s", dst, static_cast<unsigned>(len));
	return s != EOF && s != 0;
#else
	return _getfmt("%s", dst);
#endif
}
inline bool TextFile::_getfmt(const char *fmt, void *dst) {
#if defined(_MSC_VER)
	int s = fscanf_s(_file.get(), fmt, dst);
#else
	int s = fscanf(_file.get(), fmt, dst);
#endif
	return s != EOF && s != 0;
}

//========================
// * BinaryFile
//========================

inline void BinaryFile::write(const void * buffer, size_t elsize, size_t elcount) {
	fwrite(buffer, elsize, elcount, _file.get());
}
inline void BinaryFile::read(void * buffer, size_t elsize, size_t elcount) {
	fread(buffer, elsize, elcount, _file.get());
}
PRILIB_END
