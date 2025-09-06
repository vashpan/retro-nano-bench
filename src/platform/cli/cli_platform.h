#ifndef CLI_PLATFORM_H
#define CLI_PLATFORM_H

/* Windows */
#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
#  define CLI_PLATFORM_WINDOWS 1
#else
#  define CLI_PLATFORM_WINDOWS 0
#endif

/* Unix/POSIX */
#if defined(__linux__) || defined(__unix__) || defined(__unix) || \
    defined(__APPLE__) || defined(__MACH__) || defined(__FreeBSD__) || \
    defined(__OpenBSD__) || defined(__NetBSD__)
#  define CLI_PLATFORM_POSIX 1
#else
#  define CLI_PLATFORM_POSIX 0
#endif

/* Not found */
#if !(CLI_PLATFORM_WINDOWS || CLI_PLATFORM_POSIX)
#  error "Unsupported platform: define a platform layer or extend detection."
#endif

/* Feature test macros for POSIX timing APIs (Linux glibc) */
#if CLI_PLATFORM_POSIX && defined(__linux__)
#  ifndef _POSIX_C_SOURCE
#    define _POSIX_C_SOURCE 199309L
#  endif
#endif

#endif /* CLI_PLATFORM_H */
