#include <cstdio>
#include <cstdarg>

extern "C" int __cdecl __vsnprintf(char *buffer, size_t count, const char *format, va_list argptr)
{
    return _vsnprintf(buffer, count, format, argptr);
}