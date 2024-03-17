#ifndef _BITMATH_H_
#define _BITMATH_H_

#define SETBIT(x, n)    x |= (1 << n)

#define CLEARBIT(x, n)  x &= ~(1 << n)

#define TOGGLEBIT(x, n) x ^= (1 << n)
#define GETBIT(x, n)  (x >> n) & 1
#endif
