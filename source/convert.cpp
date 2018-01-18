#include "convert.h"
#include "charptr.h"

PRILIB_BEGIN
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
