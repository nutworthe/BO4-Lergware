#include "../includes.h"
#include "utils.hpp"

namespace utils
{
	std::string va(const char* fmt, ...)
	{
		char result[2048] = { 0 };

		va_list ap;
		va_start(ap, fmt);

		vsnprintf_s(result, sizeof(result), _TRUNCATE, fmt, ap);

		va_end(ap);
		return result;
	}
	
	std::string adr_to_string(const netadr_t* address)
	{
		XNADDR xn;
		dwNetadrToCommonAddr(*address, &xn, sizeof xn, nullptr);

		return va("%u.%u.%u.%u", xn.ip[0], xn.ip[1], xn.ip[2], xn.ip[3]);
	}
}