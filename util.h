#ifndef _UTIL_H_
#define _UTIL_H_

#define max(a, b) (a > b ? a : b)
#define min(a, b) (a < b ? a : b)
#define clamp(a, b, c) min(max(a, b), c)
#define sign(a) (a < 0 ? -1 : (a > 0 ? 1 : 0))

#endif
