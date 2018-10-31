// csvloader.h
// * PrivateLibrary

#pragma once
#ifndef _PRILIB_CSVLOADER_H_
#define _PRILIB_CSVLOADER_H_
#include <cassert>
#include <cctype>
#include <cmath>
#include <string>
#include <vector>
#include <algorithm>
#include "file.h"

PRILIB_BEGIN
class CSVLoader
{
public:
	struct Size {
		size_t row, column;
	};
public:
	explicit CSVLoader(TextFile &file) {
		_load(file);
	}
	explicit CSVLoader(TextFile &&file) {
		_load(file);
	}

	std::string& at(size_t row, size_t column) {
		return const_cast<std::string&>(const_cast<const CSVLoader*>(this)->at(row, column));
	}
	const std::string& at(size_t row, size_t column) const {
		return _data.at(row - 1).at(column - 1);
	}

	// word : A1, AA11, ...
	std::string& at(const std::string &word) {
		return const_cast<std::string&>(const_cast<const CSVLoader*>(this)->at(word));
	}
	const std::string& at(const std::string &word) const {
		auto post = _parse_post(word);
		return at(post.row, post.column);
	}

	Size size() const {
		return Size { row(), column() };
	}

	size_t row() const {
		return _data.size();
	}
	size_t column() const {
		return _data.empty() ? 0 : _data.at(0).size();
	}

	auto data() {
		return _data;
	}
	auto data() const {
		return _data;
	}

	auto begin() {
		return _data.begin();
	}
	auto begin() const {
		return _data.begin();
	}

	auto end() {
		return _data.end();
	}
	auto end() const {
		return _data.end();
	}

	void save(TextFile &output) const {
		for (auto &dat : _data) {
			for (size_t i = 0; i != dat.size(); ++i) {
				if (i != 0)
					output.write(",");
				output.write(dat[i]);
			}
			output.write("\n");
		}
	}

private:
	std::vector<std::vector<std::string>> _data;

	void _load(TextFile &file) {
		size_t recsize = 0;
		while (!file.eof()) {
			std::string line = file.getline();
			if (line.empty())
				continue;
			std::vector<std::string> dat;
			Convert::split(line, ",", [&](const char *s) { dat.push_back(s); }, true, true);
			recsize = std::max(dat.size(), recsize);
			_data.push_back(dat);
		}
		for (auto &dat : _data) {
			while (recsize > dat.size())
				dat.push_back("");
		}
	}

	static Size _parse_post(const std::string &word) {
		std::string wcolumn;
		std::string wrow;
		bool parse_row = false;
		for (char c : word) {
			if (!parse_row) {
				if (std::isalpha(c)) {
					wcolumn.push_back(std::toupper(c));
				}
				else if (std::isdigit(c)) {
					parse_row = true;
					wrow.push_back(c);
				}
				else {
					assert(false);
				}
			}
			else {
				if (std::isdigit(c)) {
					wrow.push_back(c);
				}
				else {
					assert(false);
				}
			}
		}
		size_t column = 0;
		size_t row = 0;
		bool v = Convert::to_integer(wrow, row);
		assert(v);
		assert(wrow.size() <= log(std::numeric_limits<size_t>::max()) / log(26));
		size_t weights = 1;
		for (auto iter = wcolumn.rbegin(); iter != wcolumn.rend(); ++iter) {
			column += weights * ((*iter) - 'A' + 1);
			weights *= 26;
		}

		return Size { row, column };
	}
};
PRILIB_END

#endif
