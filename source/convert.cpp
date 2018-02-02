#include "convert.h"
#include "charptr.h"
#include "record.h"
#include <algorithm>

PRILIB_BEGIN
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

	static bool to_base16(char c, uint8_t &result) {
		if (isxdigit(c)) {
			result = (isdigit(c)) ? (c - '0') : (c - 'a' + 0xa);
			return true;
		}
		return false;
	}

	static bool to_base16(char higher, char lower, uint8_t &result) {
		uint8_t v1, v2;
		if (to_base16(higher, v1) && to_base16(lower, v2)) {
			result = v1 * 0x10 + v2;
			return true;
		}
		return false;
	}

	bool to_base16(const std::string &str, void *dst, size_t size) {
		if (str.empty())
			return false;

		bool isresize = size == 0;

		const size_t strsize = str.size();
		if (isresize)
			size = strsize / 2 + strsize % 2;

		for (size_t i = 0; i < std::min(strsize / 2, size); ++i) {
			auto &d = reinterpret_cast<uint8_t*>(dst)[i];
			size_t si = strsize - i * 2 - 2;
			char higher = str[si];
			char lower = str[si + 1];
			if (!to_base16(higher, lower, d))
				return false;
		}

		if ((isresize || (!isresize && size > strsize / 2)) && (strsize % 2 != 0)) {
			if (!to_base16(str[0], reinterpret_cast<uint8_t*>(dst)[strsize / 2]))
				return false;
		}
		return true;
	}
}

void Convert::split(const std::string & token, const std::string & delimit, std::function<void(const char*)> yield)
{
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
PRILIB_END
