#ifndef T
#define T int
#endif

#ifdef CRABBY_NUMBER

#include "ordering.h"
#include "utilities.h"

#define number_cmp(T, a, b) TEMPLATE(number_cmp, T, a, b)
static inline Ordering number_cmp(T, T *a, T *b) {
  return *a < *b ? Less : (*a == *b ? Equal : Greater);
}
#define T_ORD CONCAT(number_cmp, T)

#endif

#ifdef T_ORD

#include "ordering.h"
#include "utilities.h"

#define max(T, a, b) TEMPLATE(max, T, a, b)
static inline T max(T, T a, T b) {
  if (T_ORD(&a, &b) == Less) {
#ifdef T_DROP
    T_DROP(a);
#endif
    return b;
  } else {
#ifdef T_DROP
    T_DROP(b);
#endif
    return a;
  }
}

#define min(T, a, b) TEMPLATE(min, T, a, b)
static inline T min(T, T a, T b) {
  if (T_ORD(&a, &b) == Greater) {
#ifdef T_DROP
    T_DROP(a);
#endif
    return b;
  } else {
#ifdef T_DROP
    T_DROP(b);
#endif
    return a;
  }
}

#endif

#ifndef CRABBY_NO_UNDEF_T

#undef T

#ifdef T_DROP
#undef T_DROP
#endif

#endif
