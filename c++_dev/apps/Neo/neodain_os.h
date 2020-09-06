#ifndef __NEODAIN_NEO_OS_H__

#include "neodain_neo.h"

namespace neodain {
namespace neo {
namespace os {

#ifdef __helper__
std::chrono::nanoseconds  duration</*signed integer type of at least 64 bits*/, std::nano>  
std::chrono::microseconds  duration</*signed integer type of at least 55 bits*/, std::micro>  
std::chrono::milliseconds  duration</*signed integer type of at least 45 bits*/, std::milli>  
std::chrono::seconds  duration</*signed integer type of at least 35 bits*/>  
std::chrono::minutes  duration</*signed integer type of at least 29 bits*/, std::ratio<60>>  
std::chrono::hours  duration</*signed integer type of at least 23 bits*/, std::ratio<3600>> 
#endif

#define sys_clock 	std::chrono::system_clock
#define time_point 	sys_clock::time_point

inline time_point now() 
{
    return sys_clock::now();
} 

inline uint64_t nowtime_msec()
{
    std::chrono::milliseconds ms =
    std::chrono::duration_cast<std::chrono::milliseconds>(
    now().time_since_epoch());
    return ms.count();
}

inline uint64_t nowtime_sec()
{
    std::chrono::seconds s =
    std::chrono::duration_cast<std::chrono::seconds>(
    now().time_since_epoch());
    return s.count();
}

inline std::time_t nowtime_t()
{
    time_point n = now();
    return sys_clock::to_time_t(n);
}


inline std::tm localtime(const std::time_t& time_tt)
{
#ifdef _WIN32
    std::tm tm;
    localtime_s(&tm, &time_tt);
#else
    std::tm tm;
    localtime_r(&time_tt, &tm);
#endif
    return tm;
}

inline std::tm localtime()
{
    std::time_t now_t = time(nullptr);
    return localtime(now_t);
}

inline std::tm gmtime(const std::time_t& time_tt)
{
#ifdef _WIN32
    std::tm tm;
    gmtime_s(&tm, &time_tt);
#else
    std::tm tm;
    gmtime_r(&time_tt, &tm);
#endif
    return tm;
}

inline std::tm gmtime()
{
    std::time_t now_t = time(nullptr);
    return gmtime(now_t);
}

inline bool operator==(const std::tm& tm1, const std::tm& tm2)
{
    return (tm1.tm_sec == tm2.tm_sec &&
            tm1.tm_min == tm2.tm_min &&
            tm1.tm_hour == tm2.tm_hour &&
            tm1.tm_mday == tm2.tm_mday &&
            tm1.tm_mon == tm2.tm_mon &&
            tm1.tm_year == tm2.tm_year &&
            tm1.tm_isdst == tm2.tm_isdst);
}

inline bool operator!=(const std::tm& tm1, const std::tm& tm2)
{
    return !(tm1 == tm2);
}

// eol definition
#if !defined (NEO_EOL)
#ifdef _WIN32
#define NEO_EOL "\r\n"
#else
#define NEO_EOL "\n"
#endif
#endif

constexpr static const char* eol = NEO_EOL;
constexpr static int eol_size = sizeof(NEO_EOL) - 1;


inline void prevent_child_fd(FILE* f) // To fd prevent generater to child 
{
#ifdef _WIN32
	auto file_handle = (HANDLE)_get_osfhandle(_fileno(f));
	if(!::SetHandleInformation(file_handle, HANDLE_FALG_INHERIT, 0))
		throw neo_ex("SetHandleInformation failed", errno);
#else
	auto fd = fileno(f);
	if(fcntl(fd, F_SETFD, FD_CLOEXEC) == -1) // 
		throw neo_ex("fcntl with FD_CLOEXEC failed", errno);
#endif
}


inline int32_t fopen_s(FILE** fp, const filename_t& filename, const filename_t& mode)
{
#ifdef _WIN32 
#ifdef NEO_WCHAR_FILENAMES
	*fp = _wfsopen((filename.c_str()), mode.c_str(), _SH_DENYWR);
#else
	*fp = _fsopen((filename.c_str()), mode.c_str(), _SH_DENYWR);
#endif
#else // unix
	*fp = fopen((filename.c_str()), mode.c_str());
#endif

#ifdef NEO_PREVENT_CHILD_FD
	if(*fp != nullptr)
		prevent_child_fd(*fp);
#endif
	return *fp == nullptr;
}


inline int remove(const filename_t &filename)
{
#if defined(_WIN32) && defined(NEO_WCHAR_FILENAMES)
    return _wremove(filename.c_str());
#else
    return std::remove(filename.c_str());
#endif
}

inline int rename(const filename_t& filename1, const filename_t& filename2)
{
#if defined(_WIN32) && defined(NEO_WCHAR_FILENAMES)
    return _wrename(filename1.c_str(), filename2.c_str());
#else
    return std::rename(filename1.c_str(), filename2.c_str());
#endif
}


//Return if file exists
inline bool file_exists(const filename_t& filename)
{
#ifdef _WIN32
#ifdef NEO_WCHAR_FILENAMES
    auto attribs = GetFileAttributesW(filename.c_str());
#else
    auto attribs = GetFileAttributesA(filename.c_str());
#endif
    return (attribs != INVALID_FILE_ATTRIBUTES && !(attribs & FILE_ATTRIBUTE_DIRECTORY));
#else //common linux/unix all have the stat system call
    struct stat buffer;
    return (stat(filename.c_str(), &buffer) == 0);
#endif
}




//Return file size according to open FILE* object
inline size_t filesize(FILE *f)
{
    if (f == nullptr)
        throw neodain::neo_ex("Failed getting file size. fd is null");
#ifdef _WIN32
    int fd = _fileno(f);
#if _WIN64 //64 bits
    struct _stat64 st;
    if (_fstat64(fd, &st) == 0)
        return st.st_size;

#else //windows 32 bits
    long ret = _filelength(fd);

    if (ret >= 0)
        return static_cast<size_t>(ret);
#endif

#else // unix
    int fd = fileno(f);
    //64 bits(but not in osx, where fstat64 is deprecated)
#if !defined(__FreeBSD__) && !defined(__APPLE__) && (defined(__x86_64__) || defined(__ppc64__))
    struct stat64 st;
    if (fstat64(fd, &st) == 0)
        return static_cast<size_t>(st.st_size);
#else // unix 32 bits or osx
    struct stat st;
    if (fstat(fd, &st) == 0)
        return static_cast<size_t>(st.st_size);
#endif
#endif
    throw neodain::neo_ex("Failed getting file size from fd", errno);
}

#define NEO_FILE_NFS_SUPER_MAGIC 0x6969
inline bool checkMounted(char *path)
{
	if (path == nullptr)
		throw neodain::neo_ex("Failed getting path name. path is null");

#if defined(__x86_64) || defined(__ppc64__)
	struct statfs64 st_buf;
	if (statfs64(path, &st_buf) == -1)
	{
		std::cout << "Failed statfs64(). result is -1 : " << path << std::endl;
		return false;
	}
#else
	struct statfs st_buf;
	if (statfs(path, &st_buf) == -1)
	{
		std::cout << "Failed statfs64(). result is -1 : " << path << std::endl;
		return false;
	}
#endif
	if (st_buf.f_type == NEO_FILE_NFS_SUPER_MAGIC)
	{
		std::cout << "NFS FILE Mounted : " << st_buf.f_type << std::endl;
	}
	else
	{
		std::cout << "REGULAR FILE Mounted : " << st_buf.f_type << std::endl;
	}
	return true;
}


inline uint32_t storeSizeRatio(char *path)
{
	if (path == nullptr)
		throw neodain::neo_ex("Failed getting path name. path is null");

#if defined(__x86_64) || defined(__ppc64__)
	std::cout << "statfs64(). check : " << path << std::endl;
	struct statfs64 st_buf;
	if (statfs64(path, &st_buf) == -1)
	{
		throw neodain::neo_ex("Failed statfs64(). result is -1");
	}
#else
	std::cout << "statfs(). check : " << path << std::endl;
	struct statfs st_buf;
	if (statfs(path, &st_buf) == -1)
	{
		throw neodain::neo_ex("Failed statfs(). result is -1");
	}
#endif

	uint64_t totalSize = st_buf.f_blocks * (st_buf.f_bsize / 1024);
	uint64_t freeSize = st_buf.f_bfree * (st_buf.f_bsize / 1024);
	uint64_t availSize = st_buf.f_bavail * (st_buf.f_bsize / 1024);
	uint64_t storeSize = totalSize - availSize;
	uint64_t storeSizeRatio = ((totalSize - availSize) * 100) / totalSize;

	std::cout << "totalSize : " << totalSize << std::endl; 
	std::cout << "freeSize  : " << freeSize << std::endl; 
	std::cout << "availSize : " << availSize << std::endl; 
	std::cout << "storeSize : " << storeSize << " ratio : " << storeSizeRatio << " %%" << std::endl; 

	return storeSizeRatio;
}


//Return utc offset in minutes or throw spdlog_ex on failure
inline int utc_minutes_offset(const std::tm& tm = neo::os::localtime())
{

#ifdef _WIN32
#if _WIN32_WINNT < _WIN32_WINNT_WS08
    TIME_ZONE_INFORMATION tzinfo;
    auto rv = GetTimeZoneInformation(&tzinfo);
#else
    DYNAMIC_TIME_ZONE_INFORMATION tzinfo;
    auto rv = GetDynamicTimeZoneInformation(&tzinfo);
#endif
    if (rv == TIME_ZONE_ID_INVALID)
        throw neodain::neo_ex("Failed getting timezone info. ", errno);

    int offset = -tzinfo.Bias;
    if (tm.tm_isdst)
        offset -= tzinfo.DaylightBias;
    else
        offset -= tzinfo.StandardBias;
    return offset;
#else

#if defined(sun) || defined(__sun)
    // 'tm_gmtoff' field is BSD extension and it's missing on SunOS/Solaris
    struct helper
    {
        static long int calculate_gmt_offset(const std::tm & localtm = neo::os::localtime(), const std::tm & gmtm = neo::os::gmtime())
        {
            int local_year = localtm.tm_year + (1900 - 1);
            int gmt_year = gmtm.tm_year + (1900 - 1);

            long int days = (
                                // difference in day of year
                                localtm.tm_yday - gmtm.tm_yday

                                // + intervening leap days
                                + ((local_year >> 2) - (gmt_year >> 2))
                                - (local_year / 100 - gmt_year / 100)
                                + ((local_year / 100 >> 2) - (gmt_year / 100 >> 2))

                                // + difference in years * 365 */
                                + (long int)(local_year - gmt_year) * 365
                            );

            long int hours = (24 * days) + (localtm.tm_hour - gmtm.tm_hour);
            long int mins = (60 * hours) + (localtm.tm_min - gmtm.tm_min);
            long int secs = (60 * mins) + (localtm.tm_sec - gmtm.tm_sec);

            return secs;
        }
    };

    long int offset_seconds = helper::calculate_gmt_offset(tm);
#else
    long int offset_seconds = tm.tm_gmtoff;
#endif

    return static_cast<int>(offset_seconds / 60);
#endif
}

//Return current thread id as size_t
//It exists because the std::this_thread::get_id() is much slower(espcially under VS 2013)
inline size_t _thread_id()
{
#ifdef _WIN32
    return  static_cast<size_t>(::GetCurrentThreadId());
#elif __linux__
# if defined(__ANDROID__) && defined(__ANDROID_API__) && (__ANDROID_API__ < 21)
#  define SYS_gettid __NR_gettid
# endif
    return  static_cast<size_t>(syscall(SYS_gettid));
#elif __FreeBSD__
    long tid;
    thr_self(&tid);
    return static_cast<size_t>(tid);
#elif __APPLE__
    uint64_t tid;
    pthread_threadid_np(nullptr, &tid);
    return static_cast<size_t>(tid);
#else //Default to standard C++11 (other Unix)
    return static_cast<size_t>(std::hash<std::thread::id>()(std::this_thread::get_id()));
#endif
}

//Return current thread id as size_t (from thread local storage)
inline size_t thread_id()
{
#if defined(_MSC_VER) && (_MSC_VER < 1900) || defined(__clang__) && !__has_feature(cxx_thread_local)
    return _thread_id();
#else
    static thread_local const size_t tid = _thread_id();
    return tid;
#endif
}




// wchar support for windows file names (NEO_WCHAR_FILENAMES must be defined)
#if defined(_WIN32) && defined(NEO_WCHAR_FILENAMES)
#define NEO_FILENAME_T(s) L ## s
inline std::string filename_to_str(const filename_t& filename)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> c;
    return c.to_bytes(filename);
}
#else
#define NEO_FILENAME_T(s) s
inline std::string filename_to_str(const filename_t& filename)
{
    return filename;
}
#endif

inline std::string errno_to_string(char buf[256], char* res)
{
    return std::string(res);
}

inline std::string errno_to_string(char buf[256], int res)
{
    if (res == 0)
    {
        return std::string(buf);
    }
    else
    {
        return "Unknown error";
    }
}

// Return errno string (thread safe)
inline std::string errno_str(int err_num)
{
    char buf[256];
    NEO_CONSTEXPR auto buf_size = sizeof(buf);

#ifdef _WIN32
    if (strerror_s(buf, buf_size, err_num) == 0)
        return std::string(buf);
    else
        return "Unknown error";

#elif defined(__FreeBSD__) || defined(__APPLE__) || defined(ANDROID) || defined(__SUNPRO_CC) || \
      ((_POSIX_C_SOURCE >= 200112L) && ! defined(_GNU_SOURCE)) // posix version

    if (strerror_r(err_num, buf, buf_size) == 0)
        return std::string(buf);
    else
        return "Unknown error";

#else  // gnu version (might not use the given buf, so its retval pointer must be used)
    auto err = strerror_r(err_num, buf, buf_size); // let compiler choose type
    return errno_to_string(buf, err); // use overloading to select correct stringify function
#endif
}

inline int pid()
{

#ifdef _WIN32
    return ::_getpid();
#else
    return static_cast<int>(::getpid());
#endif

}


// Detrmine if the terminal supports colors
// Source: https://github.com/agauniyal/rang/
inline bool is_color_terminal()
{
#ifdef _WIN32
    return true;
#else
    static constexpr const char* Terms[] =
    {
        "ansi", "color", "console", "cygwin", "gnome", "konsole", "kterm",
        "linux", "msys", "putty", "rxvt", "screen", "vt100", "xterm"
    };

    const char *env_p = std::getenv("TERM");
    if (env_p == nullptr)
    {
        return false;
    }

    static const bool result = std::any_of(
                                   std::begin(Terms), std::end(Terms), [&](const char* term)
    {
        return std::strstr(env_p, term) != nullptr;
    });
    return result;
#endif
}


// Detrmine if the terminal attached
// Source: https://github.com/agauniyal/rang/
inline bool in_terminal(FILE* file)
{

#ifdef _WIN32
    return _isatty(_fileno(file)) ? true : false;
#else
    return isatty(fileno(file)) ? true : false;
#endif
}


} // namespace os
} // namespace neo
} // namespace neodain

#endif // __NEODAIN_NEO_OS_H__

