#include "file.h"

PRILIB_BEGIN
//========================
// * File
//========================

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
		FileRecordPost frecpost(_file);
		_setPostEnd();
		size = ftell(_file.get());
	}
	this->_size = static_cast<size_t>(size);
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
	std::list<std::unique_ptr<char>> strlist;
	const size_t size = 0x100;
	size_t length = 0;

	while (true) {
		strlist.push_back(std::unique_ptr<char>(new char[size]()));
		char *buffer = strlist.back().get();

		fgets(buffer, size, _file.get());

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

std::string TextFile::getText() const {
	if (bad()) return "";
	FileRecordPost frecpost(_file);
	charptr tmp(size());
	_setPostBegin();
	fread(tmp, sizeof(char), size(), _file.get());
	return tmp.to_string();
}
PRILIB_END
