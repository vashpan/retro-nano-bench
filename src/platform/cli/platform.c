#include "cli_platforms.h"

#include <stdio.h>
#include <stdarg.h>
#include <time.h>

#if CLI_PLATFORM_WINDOWS
#  include <windows.h>
#endif

#if CLI_PLATFORM_POSIX
#  include <sys/time.h>
#endif

#include "platform.h"

void platform_log(const char *format, ...) {
    va_list arglist;

    va_start(arglist, format);
    vprintf(format, arglist);
    va_end(arglist);
}

#if CLI_PLATFORM_WINDOWS
static double win32_get_time() {
    LARGE_INTEGER freq, counter;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&counter);
    return (double)counter.QuadPart / (double)freq.QuadPart;
}
#endif

#if CLI_PLATFORM_POSIX
static double posix_get_time() {
#if defined(_POSIX_TIMERS) && defined(CLOCK_REALTIME)
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    return (double)ts.tv_sec + (double)ts.tv_nsec / 1.0e9;
#else
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (double)(tv.tv_sec + (tv.tv_usec / 1E6));
#endif
}
#endif

double platform_get_time() {
#if CLI_PLATFORM_WINDOWS
    return win32_get_time();
#elif CLI_PLATFORM_POSIX
    return posix_get_time();
#else
    return 0.0;
#endif
}
