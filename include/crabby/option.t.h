#include "utilities.h"
#include <stdckdint.h>

#ifndef T
#define T int
#endif

#define CRABBY_OPTION_NONE "crabby/option: unexpected 'None'"

#ifndef OPTION_TAG
typedef enum {
  Some,
  None,
} OptionTag;
#endif

#define Option(T) CONCAT(Option, T)
typedef struct {
  OptionTag tag;
  T val;
} Option(T);

#define option_some(T, val) TEMPLATE(option_some, T, val)
static inline Option(T) option_some(T, T val) {
  return (Option(T)){.tag = Some, .val = val};
}

#define option_none(T) TEMPLATE(option_none, T)
static inline Option(T) option_none(T) { return (Option(T)){.tag = None}; }

#define option_unwrap(T, self) TRACK(option_unwrap_, T, self)
#define option_unwrap_(...) TEMPLATE(option_unwrap_, __VA_ARGS__)
static inline T option_unwrap_(T, Option(T) self, const char *file, int line) {
  if (self.tag == None) {
    panic_track(CRABBY_OPTION_NONE, file, line);
  }

  return self.val;
}

#define option_unwrap_or(T, self, val) TEMPLATE(option_unwrap_or, T, self, val)
static inline T option_unwrap_or(T, Option(T) self, T val) {
  return self.tag == Some ? self.val : val;
}

#define option_drop(T, self) TEMPLATE(option_drop, T, self)
static inline void option_drop(T, Option(T) self){
#ifdef T_DROP
    T_DROP(self.val)
#endif
}

#define option_clone(T, self) TEMPLATE(option_drop, T, self)
#ifdef T_DROP
#ifdef T_CLONE
static inline Option(T) option_clone(T, Option(T) * self) {
  return self->tag == Some ? (Option(T)) {
    .tag = Some,
    .val = T_CLONE(val),
  } : option_none(T);
}
#endif
#else
static inline Option(T) option_clone(T, Option(T) * self) {
  return *self;
}
#endif

#ifndef T_DROP
#define checked_add(T, a, b) TEMPLATE(checked_add, T, a, b)
static inline Option(T) checked_add(T, T a, T b) {
  T res;
  if (ckd_add(&res, a, b)) {
    return option_none(T);
  }
  return option_some(T, res);
}
#endif

#ifndef CRABBY_NO_UNDEF_T

#undef T

#ifdef T_DROP
#undef T_DROP
#endif

#ifdef T_CLONE
#undef T_CLONE
#endif

#endif
