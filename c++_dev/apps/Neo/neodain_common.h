#ifndef __NEODAIN_COMMON_H__
#define __NEODAIN_COMMON_H__

#include <cstdarg>
#include <cassert>

int32_t neo_sprintf(char *pstr, size_t size, const char* format, ...)
{
    if(pstr == NULL)
    {
        assert(!"Error : neo_snprintf() is trying to access NULL pointer");
        return 0;
    }

    int32_t ns=0;
    va_list ptr; 
    va_start(ptr, format);
    ns = std::vsnprintf(pstr, size, format, ptr);
    va_end(ptr);
    return ns; 
}


#endif
