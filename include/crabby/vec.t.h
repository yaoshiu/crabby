#include "numbers.h"
#include "utilities.h"

#ifndef T
#define T int
#endif

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

#define vec_with_capacity(T, capacity)                                         \
  TRACK_CALL(_vec_with_capacity_track, T, capacity)
#define _vec_with_capacity_track(T, capacity)                                  \
  TRACK(_vec_with_capacity_track, T, capacity)
static inline Vec(T) _vec_with_capacity_track(T, usize cap) {
  if (cap == 0) {
    return vec_new(T);
  }

  Option(usize) cap_byte = checked_mul(usize, cap, sizeof(T));
  if (option_is_none(usize, &cap_byte)) {
    panic_track(CRABBY_VEC_CAP_OVERFLOW);
  }

  T *data = (T *)malloc(cap_byte.val);
  if (data == NULL) {
    panic_track(CRABBY_VEC_OOM);
  }

  return (Vec(T)){.len = 0, .cap = cap, .data = data};
}

#define vec_drop(T, self) TEMPLATE(vec_drop, T, self)
static inline void vec_drop(T, Vec(T) self) {
#ifdef T_DROP
  for (usize i = 0; i < self.len; i++) {
    T_DROP(self.data[i]);
  }
#endif
  free(self.data);
}

#define vec_reserve_exact(T, self, additional)                                 \
  TRACK_CALL(_vec_reserve_exact_track, T, self, additional)
#define _vec_reserve_exact_track(T, self, additional)                          \
  TRACK(_vec_reserve_exact_track, T, self, additional)
static inline void _vec_reserve_exact_track(T, Vec(T) * self,
                                            usize additional) {
  Option(usize) required = checked_add(usize, self->len, additional);
  if (option_is_none(usize, &required)) {
    panic_track(CRABBY_VEC_CAP_OVERFLOW);
  }

  if (required.val <= self->cap) {
    return;
  }

  Option(usize) required_byte = checked_mul(usize, required.val, sizeof(T));
  if (option_is_none(usize, &required_byte)) {
    panic_track(CRABBY_VEC_CAP_OVERFLOW);
  }

  T *data = realloc(self->data, required_byte.val);
  if (data == NULL) {
    panic_track(CRABBY_VEC_OOM);
  }

  self->cap = required.val;
  self->data = data;
}

#define vec_reserve(T, self, additional)                                       \
  TRACK_CALL(_vec_reserve_track, T, self, additional)
#define _vec_reserve_track(T, self, additional)                                \
  TRACK(_vec_reserve_track, T, self, additional)
static inline void _vec_reserve_track(T, Vec(T) * self, usize additional) {
  Option(usize) required = checked_add(usize, self->len, additional);
  if (option_is_none(usize, &required)) {
    panic_track(CRABBY_VEC_CAP_OVERFLOW);
  }

  if (required.val <= self->cap) {
    return;
  }

  Option(usize) double_cap = checked_mul(usize, self->cap, 2);
  if (option_is_none(usize, &double_cap)) {
    panic_track(CRABBY_VEC_CAP_OVERFLOW);
  }

  usize add = max(usize, double_cap.val, required.val) - self->len;

  CONCAT(_vec_reserve_exact_track, T)(self, add, file, line);
}

#ifndef CRABBY_NO_UNDEF_T

#undef T

#ifdef T_DROP
#undef T_DROP
#endif

#endif
