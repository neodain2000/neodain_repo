#ifndef __NEODAIN_NEO_H__

#include <cstdio>
#include <ctime>
#include <functional>
#include <string>
#include <chrono>
#include <algorithm>
#include <cstring>
#include <cstdlib>
#include <sys/stat.h>
#include <sys/types.h>

#include <stdexcept>
#include <iostream>
#include <typeinfo>

#ifdef _WIN32
#include <windows.h>
#include <process.h> // _get_pid support
#include <io.h> // _get_oshandle and _isatty support

#ifdef __MINGW32__
#include <share.h>
#endif

#else // unix

#include <unistd.h>
#include <fcntl.h>
#include <sys/vfs.h>

#ifdef __linux__
#include <sys/syscall.h> // use gettid() syscall under linux to get thread id
#elif __FreeBSD__
#include <sys/thr.h> // use thr_self() syscall under FreeBSD to get thread id
#endif

#endif // unix

#ifndef __has_feature       // Clang - feature checking macros.
#define __has_feature(x) 0  // Compatibility with non-clang compilers.
#endif


//visual studio upto 2013 does not support noexcept nor constexpr
#if defined(_MSC_VER) && (_MSC_VER < 1900)
#define NEO_NOEXCEPT throw()
#define NEO_CONSTEXPR
#else
#define NEO_NOEXCEPT noexcept
#define NEO_CONSTEXPR constexpr
#endif


/*
using log_clock = std::chrono::system_clock;
using sink_ptr = std::shared_ptr < sinks::sink >;
using sinks_init_list = std::initializer_list < sink_ptr >;
using formatter_ptr = std::shared_ptr<spdlog::formatter>;
#if defined(SPDLOG_NO_ATOMIC_LEVELS)
using level_t = details::null_atomic_int;
#else
using level_t = std::atomic<int>;
#endif

using log_err_handler = std::function<void(const std::string &err_msg)>;
*/


namespace neodain {

namespace neo {
namespace os {
std::string errno_str(int err_num);
} // namespace os
} // namespace neo

struct null_mutex
{
    void lock() {}
    void unlock() {}
    bool try_lock() { return true; }
};

using neo_clock = std::chrono::system_clock;

#if defined(_WIN32) && defined(NEO_WCHAR_FILENAMES)
using filename_t = std::wstring;
#else
using filename_t = std::string;
#endif

class neo_ex: public std::exception
{
  public:
    neo_ex(const std::string& msg) : msg_m(msg) { }
    neo_ex(const std::string& msg, int last_errno)
    {
        msg_m = msg + " : " + neo::os::errno_str(last_errno);
    }
    const char* what() const NEO_NOEXCEPT override
    {
        return msg_m.c_str();
    }
  private:
    std::string msg_m;
};

} // namespace neodain


namespace neodain {
namespace neo {
namespace app {

// void CopyObject(const char *original) NEO_NOEXCEPT {
void CopyObject(const char *original) {
    // std::cout << original << std::endl;
    if(nullptr == original) {
        std::cout << "original is nullptr" << std::endl;
        throw neo_ex("CopyObject() function : argument is NULL(nullptr).", errno);
    }
}

// void MyFunc(char *name) NEO_NOEXCEPT {
void MyFunc(char *name) {
    //check generate exception
    bool ok = NEO_NOEXCEPT(CopyObject(name));
    std::cout << std::boolalpha << "CopyObject() noexcept? " << ok << std::endl;
    if(ok) CopyObject(name);
}

} // namespace app 
} // namespace neo
} // namespace neodain


#endif // __NEODAIN_NEO_H__

