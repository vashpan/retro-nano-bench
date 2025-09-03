#ifdef __linux__ 
#define _POSIX_C_SOURCE 199309L /* to tell Linux Glibc to use proper POSIX standard */
#endif

#include <stdio.h>
#include <stdarg.h>

#include <sys/time.h>

#if defined(_WIN32)
#include <windows.h>
#endif

#include "platform.h"

void platform_log(const char *format, ...) {
    va_list arglist;

    va_start(arglist, format);
    vprintf(format, arglist);
    va_end(arglist);
}

#if defined(_WIN32)
static double win32_get_time() {
    LARGE_INTEGER freq, counter;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&counter);
    return (double)counter.QuadPart / (double)freq.QuadPart;
}
#else
static double posix_get_time() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec + (double)ts.tv_nsec * 1e-9;
}
#endif

double platform_get_time() {
#if defined(_WIN32)
    return win32_get_time();
#else
    return posix_get_time();
#endif
}