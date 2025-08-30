#ifndef _PLAFTORM_H_
#define _PLAFTORM_H_

void platform_log(const char *format, ...); /* debug log */
double platform_get_time(); /* time since epoch, in seconds */

#endif  /* _PLAFTORM_H_ */