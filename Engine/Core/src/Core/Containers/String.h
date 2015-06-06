#pragma once

#include <string>
#include <cstdio>
#include <stdarg.h>

template<int TLength>
std::string gCreateStringF(const char* inFormat, ...)
{
	char buffer[TLength];
	
	va_list argp;
	va_start(argp, inFormat);
	vsprintf_s(buffer, TLength, inFormat, argp);
	va_end(argp);

	return std::string(buffer);
}
