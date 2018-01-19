#include "convert.h"
#include "charptr.h"
#include "record.h"

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
