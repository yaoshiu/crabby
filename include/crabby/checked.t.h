#ifdef CRABBY_NUMBER

#include "utilities.h"
#include <stdckdint.h>

#define checked_add(T, a, b) TEMPLATE(checked_add, T, a, b)
static inline Option(T) checked_add(T, T a, T b) {
  T res;
  if (ckd_add(&res, a, b)) {
    return option_none(T);
  }
  return option_some(T, res);
}

#define checked_mul(T, a, b) TEMPLATE(checked_mul, T, a, b)
static inline Option(T) checked_mul(T, T a, T b) {
  T res;
  if (ckd_mul(&res, a, b)) {
    return option_none(T);
  }
  return option_some(T, res);
}

#define checked_sub(T, a, b) TEMPLATE(checked_sub, T, a, b)
static inline Option(T) checked_sub(T, T a, T b) {
  T res;
  if (ckd_sub(&res, a, b)) {
    return option_none(T);
  }
  return option_some(T, res);
}

#endif
