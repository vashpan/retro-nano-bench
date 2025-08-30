#ifdef __linux__ 
#define _POSIX_C_SOURCE 199309L /* to tell Linux Glibc to use proper POSIX standard */
#endif

#include <stdio.h>
#include <stdarg.h>

#include <sys/time.h>

#include "platform.h"

void platform_log(const char *format, ...) {
    va_list arglist;

    va_start(arglist, format);
    vprintf(format, arglist);
    va_end(arglist);
}

double platform_get_time() {
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