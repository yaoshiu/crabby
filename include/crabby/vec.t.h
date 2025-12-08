#include "utilities.h"
#include <stdckdint.h>
#include <stddef.h>
#include <stdlib.h>

#ifndef T
#define T int
#endif // T

#define CRABBY_VEC_DEFAULT_CAP 4
#define CRABBY_VEC_OOM "crabby/vec: out of memory"
#define CRABBY_VEC_CAP_OVERFLOW "crabby/vec: capacity overflow"

#define Vec(T) CONCAT(Vec, T)
typedef struct {
  size_t len;
  size_t cap;
  T *data;
} Vec(T);

#define vec_new(T) TEMPLATE(vec_new, T)
static inline Vec(T) vec_new(T) {
  return (Vec(T)){.len = 0, .cap = 0, .data = NULL};
}

#define vec_with_capacity(T, cap) TRACK(vec_with_capacity, T, cap)
#define vec_with_capacity_(...) TEMPLATE(vec_with_capacity, __VA_ARGS__)
static inline Vec(T)
    vec_with_capacity_(T, size_t cap, const char *file, int line) {
  if (cap == 0) {
    return vec_new(T);
  }

  size_t cap_byte;
  if (ckd_mul(&cap_byte, cap, sizeof(T))) {
    panic_track(CRABBY_VEC_CAP_OVERFLOW, file, line);
  }

  T *data = (T *)malloc(cap_byte);
  if (data == NULL) {
    panic_track(CRABBY_VEC_OOM, file, line);
  }

  return (Vec(T)){.len = 0, .cap = cap, .data = data};
}

#define vec_drop(T, self) TEMPLATE(vec_drop, T, self)
static inline void vec_drop(T, Vec(T) self) {
#ifdef T_DROP
  for (size_t i = 0; i < self.len; i++) {
    T_DROP(self.data[i]);
  }
#endif

  free(self.data);
}
#define vec_reserve_exact(T, self, n) TEMPLATE(vec_reserve_exact, T, self, n)
static inline void vec_reserve_exact(T, Vec(T) * self, size_t n) {}

#ifndef CRABBY_NO_UNDEF_T

#undef T

#ifdef T_DROP
#undef T_DROP
#endif // T_DROP

#endif // CRABBY_NO_UNDEF_T
