#include "utilities.h"
#include <stdckdint.h>
#include <stdbool.h>

#ifndef T
#define T int
#endif

#define CRABBY_OPTION_NONE "crabby/option: unexpected 'None'"

#ifndef CRABBY_OPTION_TAG
#define CRABBY_OPTION_TAG
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

#define option_unwrap(T, self) TRACK_CALL(_option_unwrap_track, T, self)
#define _option_unwrap_track(T, self) TRACK(_option_unwrap_track, T, self)
static inline T _option_unwrap_track(T, Option(T) self) {
  if (self.tag == None) {
    panic_track(CRABBY_OPTION_NONE);
  }

  return self.val;
}

#define option_unwrap_or(T, self, val) TEMPLATE(option_unwrap_or, T, self, val)
static inline T option_unwrap_or(T, Option(T) self, T val) {
  return self.tag == Some ? self.val : val;
}

#define option_is_some(T, self) TEMPLATE(option_is_some, T, self)
static inline bool option_is_some(T, const Option(T)* self) {
  return self->tag == Some;
}

#define option_is_none(T, self) TEMPLATE(option_is_none, T, self)
static inline bool option_is_none(T, const Option(T) *self) {
  return self->tag == None;
}

#define option_drop(T, self) TEMPLATE(option_drop, T, self)
static inline void option_drop(T, Option(T) self) {
#ifdef T_DROP
  if (self.tag == Some) {
    T_DROP(self.val);
  }
#endif
}

#define option_expect(T, self, message) TRACK_CALL(_option_expect_track, T, self, message)
#define _option_expect_track(T, self, message) TRACK(_option_expect_track, T, self, message)
static inline T _option_expect_track(T, Option(T) self, const char *message) {
  if (self.tag == None) {
    panic_track(message);
  }

  return self.val;
}

#define option_clone(T, self) TEMPLATE(option_clone, T, self)
#ifdef T_DROP
#ifdef T_CLONE
static inline Option(T) option_clone(T, const Option(T) * self) {
  return self->tag == Some ? (Option(T)) {
    .tag = Some,
    .val = T_CLONE(self->val),
  } : option_none(T);
}
#endif
#else
static inline Option(T) option_clone(T, const Option(T) * self) { return *self; }
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
