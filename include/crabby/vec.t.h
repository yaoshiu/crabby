#include "numbers.h"
#include "utilities.h"
#include <string.h>

#ifndef T
#define T int
#endif

typedef struct Option(T) Option(T);

static Option(T) option_some(T, T val);
static Option(T) option_none(T);

#define CRABBY_VEC_OOM "crabby/vec: out of memory"
#define CRABBY_VEC_CAP_OVERFLOW "crabby/vec: capacity overflow"

#define Vec(T) CONCAT(Vec, T)
typedef struct Vec(T) {
  size_t len;
  size_t cap;
  T *data;
} Vec(T);

#define Self Vec(T)

#define vec_new(T) TEMPLATE(vec_new, T)
static inline Self vec_new(T) {
  return (Self){.len = 0, .cap = 0, .data = NULL};
}

#define vec_with_capacity(T, capacity)                                         \
  TRACK_CALLER(_vec_with_capacity, T, capacity)
#define _vec_with_capacity(T, capacity) TRACK(_vec_with_capacity, T, capacity)
static inline Self _vec_with_capacity(T, usize capacity) {
  if (capacity == 0) {
    return vec_new(T);
  }

  Option(usize) cap_byte = checked_mul(usize, capacity, sizeof(T));
  if (option_is_none(usize, &cap_byte)) {
    panic_track(CRABBY_VEC_CAP_OVERFLOW);
  }

  T *data = (T *)malloc(cap_byte.val);
  if (data == NULL) {
    panic_track(CRABBY_VEC_OOM);
  }

  return (Self){.len = 0, .cap = capacity, .data = data};
}

#define vec_drop(T, self) TEMPLATE(vec_drop, T, self)
static inline void vec_drop(T, Self self) {
#ifdef T_DROP
  for (usize i = 0; i < self.len; i++) {
    T_DROP(self.data[i]);
  }
#endif
  free(self.data);
}

#define vec_reserve_exact(T, self, additional)                                 \
  TRACK_CALLER(_vec_reserve_exact, T, self, additional)
#define _vec_reserve_exact(T, self, additional)                                \
  TRACK(_vec_reserve_exact, T, self, additional)
static inline void _vec_reserve_exact(T, Self *self, usize additional) {
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

  T *data = (T *)realloc(self->data, required_byte.val);
  if (data == NULL) {
    panic_track(CRABBY_VEC_OOM);
  }

  self->cap = required.val;
  self->data = data;
}

#define vec_reserve(T, self, additional)                                       \
  TRACK_CALLER(_vec_reserve, T, self, additional)
#define _vec_reserve(T, self, additional)                                      \
  TRACK(_vec_reserve, T, self, additional)
static inline void _vec_reserve(T, Self *self, usize additional) {
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

  CONCAT(_vec_reserve_exact, T)(self, add, _file, _line);
}

#define vec_push(T, self, value) TRACK_CALLER(_vec_push, T, self, value)
#define _vec_push(T, self, value) TRACK(_vec_push, T, self, value)
static inline void _vec_push(T, Self *self, T value) {
  CONCAT(_vec_reserve, T)(self, 1, _file, _line);
  self->data[self->len++] = value;
}

#define vec_pop(T, self) TEMPLATE(vec_pop, T, self)
static inline Option(T) vec_pop(T, Self *self) {
  return self->len == 0 ? option_none(T)
                        : option_some(T, self->data[--self->len]);
}

#define vec_clone(T, self) TRACK_CALLER(_vec_clone, T, self)
#define _vec_clone(T, self) TRACK(_vec_clone, T, self)
#ifdef T_DROP
#ifdef T_CLONE
static inline Self _vec_clone(T, const Self *self) {
  Self clone = CONCAT(_vec_with_capacity, T)(self->cap, _file, _line);
  clone.len = self->len;
  for (usize i = 0; i < self->len; i++) {
    clone.data[i] = T_CLONE(&self->data[i]);
  }
  return clone;
}
#endif
#else
static inline Self _vec_clone(T, const Self *self) {
  Self clone = CONCAT(_vec_with_capacity, T)(self->cap, _file, _line);
  clone.len = self->len;
  // since `self->cap` does not overflow, we assume that this is okay
  memcpy(clone.data, self->data, self->len * sizeof(T));
  return clone;
}
#endif

#define vec_from(T, s, len) TRACK_CALLER(_vec_from, T, s, len)
#define _vec_from(T, s, len) TRACK(_vec_from, T, s, len)
static inline Self _vec_from(T, T *s, usize len) {
  Option(usize) capacity = checked_mul(usize, len, sizeof(T));
  if (option_is_none(usize, &capacity)) {
    panic_track(CRABBY_VEC_OOM);
  }

  T *data = (T *)malloc(capacity.val);
  if (data == NULL) {
    panic_track(CRABBY_VEC_OOM);
  }
  memcpy(data, s, capacity.val);

  return (Self){.len = len, .cap = len, .data = data};
}

#undef Self
