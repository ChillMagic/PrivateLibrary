#include "convert.h"
#include "charptr.h"
#include "record.h"
#include "range.h"
#include <algorithm>
#include <cerrno>

PRILIB_BEGIN
namespace Convert
{
	std::string to_hex(const void *src, size_t size)
	{
		charptr result(3 * size + 10);

		const byte *begin = reinterpret_cast<const byte*>(src);
		const byte *end = begin + size;
		char *rp = result;

		for (auto ptr : range(begin, end)) {
			int v;
			v = *ptr / 0x10;
			rp[0] = (v < 10) ? ('0' + v) : ('A' + v - 10);
			v = *ptr % 0x10;
			rp[1] = (v < 10) ? ('0' + v) : ('A' + v - 10);
			rp[2] = ' ';
			rp += 3;
		}

		if (size > 0)
			rp[-1] = '\0';

		return result.to_string();
	}
}

namespace Convert
{
	template <typename T, T f(const char *str, char **str_end, int base)>
	bool to_num_base(const std::string &str, T &result, int base)
	{
		Record<int> recerr(errno, 0);

		char *end;
		result = f(str.data(), &end, base);

		if (str.data() + str.size() != end)
			return false;

		if (errno == ERANGE)
			return false;

		if (result == 0)
			return str[0] == '0';

		return true;
	}

	bool to_ll(const std::string &str, long long &result, int base)
	{
		return to_num_base<long long, std::strtoll>(str, result, base);
	}
	bool to_ull(const std::string &str, unsigned long long &result, int base)
	{
		return to_num_base<unsigned long long, std::strtoull>(str, result, base);
	}

	static int to_integer(char c);

	bool is_integer(char c, int base)
	{
		int r = to_integer(c);
		return r >= 0 && r < base;
	}

	bool is_integer(const std::string &str, int base)
	{
		for (auto &c : str) {
			if (!is_integer(c, base))
				return false;
		}
		return true;
	}

	static int to_integer(char c)
	{
		if (isdigit(c))
			return c - '0';
		else if (islower(c))
			return c - 'a' + 0xa;
		else if (isupper(c))
			return c - 'A' + 0xa;
		else
			return -1;
	}

	bool to_integer(char c, int &result, int base)
	{
		if (is_integer(c, base)) {
			result = to_integer(c);
			return true;
		}

		return false;
	}

	static bool to_base16(char higher, char lower, uint8_t &result) {
		uint8_t v1, v2;
		if (to_integer(higher, v1, 16) && to_integer(lower, v2, 16)) {
			result = v1 * 0x10 + v2;
			return true;
		}
		return false;
	}

	bool to_base16(const std::string &str, void *dst, size_t size, size_t msize) {
		if (str.empty())
			return false;

		bool islsb = msize == 0;
		bool isresize = size == 0;

		const size_t strsize = str.size();
		if (isresize)
			size = strsize / 2 + strsize % 2;

		if (!islsb && (size > msize))
			size = msize;

		size_t i;
		for (i = 0; i < std::min(strsize / 2, size); ++i) {
			auto &d = reinterpret_cast<uint8_t*>(dst)[islsb ? i : (msize - i - 1)];
			size_t si = strsize - i * 2 - 2;
			char higher = str[si];
			char lower = str[si + 1];
			if (!to_base16(higher, lower, d))
				return false;
		}

		if ((isresize || (!isresize && size > strsize / 2)) && (strsize % 2 != 0 && (islsb || (!islsb && msize > i)))) {
			if (!to_integer(str[0], reinterpret_cast<uint8_t*>(dst)[islsb ? i : (msize - i - 1)], 16))
				return false;
		}
		return true;
	}
}

void Convert::split(const std::string &token, const std::string &delimit, std::function<void(const char *)> yield, bool remain, bool lastremain)
{
	charptr buffer(token + (lastremain ? "," : ""));
	if (remain) {
		char *p = buffer.get();
		char *saveptr = p;

		while (*p) {
			if (delimit.find(*p) != delimit.npos) {
				*p = '\0';
				yield(saveptr);
				saveptr = p + 1;
			}
			p++;
		}
		if (saveptr != p) {
			yield(saveptr);
		}
	}
	else {
#if (defined(_MSC_VER))
		auto _strtok = strtok_s;
#else
		auto _strtok = strtok_r;
#endif
		char *saveptr;
		char *p;
		charptr buffer(token);

		p = _strtok(buffer, delimit.c_str(), &saveptr);
		while (p) {
			yield(p);
			p = _strtok(nullptr, delimit.c_str(), &saveptr);
		}
	}
}
PRILIB_END
