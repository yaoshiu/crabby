#include "utilities.h"
#include <stdbool.h>

#ifndef T
#define T int
#endif

#define CRABBY_OPTION_NONE                                                     \
  "crabby/option: called `option_unwrap` on a `None` value"

#ifndef CRABBY_OPTION_TAG
#define CRABBY_OPTION_TAG
typedef enum {
  Some,
  None,
} OptionTag;
#endif

#define Option(T) CONCAT(Option, T)
typedef struct Option(T) {
  OptionTag tag;
  T val;
} Option(T);

#define Self Option(T)

#define option_some(T, value) TEMPLATE(option_some, T, value)
static inline Self option_some(T, T value) {
  return (Self){.tag = Some, .val = value};
}

#define option_none(T) TEMPLATE(option_none, T)
static inline Self option_none(T) { return (Self){.tag = None}; }

#define option_unwrap(T, self) TRACK_CALLER(_option_unwrap, T, self)
#define _option_unwrap(T, self) TRACK(_option_unwrap, T, self)
static inline T _option_unwrap(T, Self self) {
  if (self.tag == None) {
    panic_track(CRABBY_OPTION_NONE);
  }

  return self.val;
}

#define option_unwrap_or(T, self, value)                                       \
  TEMPLATE(option_unwrap_or, T, self, value)
static inline T option_unwrap_or(T, Self self, T value) {
  return self.tag == Some ? self.val : value;
}

#define option_is_some(T, self) TEMPLATE(option_is_some, T, self)
static inline bool option_is_some(T, const Self *self) {
  return self->tag == Some;
}

#define option_is_none(T, self) TEMPLATE(option_is_none, T, self)
static inline bool option_is_none(T, const Self *self) {
  return self->tag == None;
}

#define option_drop(T, self) TEMPLATE(option_drop, T, self)
static inline void option_drop(T, Self self) {
#ifdef T_DROP
  if (self.tag == Some) {
    T_DROP(self.val);
  }
#endif
}

#define option_expect(T, self, message)                                        \
  TRACK_CALLER(_option_expect, T, self, message)
#define _option_expect(T, self, message) TRACK(_option_expect, T, self, message)
static inline T _option_expect(T, Self self, const char *message) {
  if (self.tag == None) {
    panic_track(message);
  }

  return self.val;
}

#define option_clone(T, self) TEMPLATE(option_clone, T, self)
#ifdef T_DROP
#ifdef T_CLONE
static inline Self option_clone(T, const Self *self) {
  return self->tag == Some ? option_some(T, T_CLONE(&self->val))
                           : option_none(T);
}
#endif
#else
static inline Self option_clone(T, const Self *self) { return *self; }
#endif

#undef Self
