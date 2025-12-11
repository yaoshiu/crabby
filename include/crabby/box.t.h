#include "utilities.h"

#ifndef T
#define T int
#endif

#define CRABBY_BOX_OOM "crabby/box: out of memory"

#define Box(T) CONCAT(Box, T)
typedef struct {
  T *ptr;
} Box(T);

#define Self Box(T)

#define box_new(T, value) TRACK_CALLER(_box_new, T, value)
#define _box_new(T, value) TRACK(_box_new, T, value)
static inline Self _box_new(T, T value) {
  T *ptr = (T *)malloc(sizeof(T));
  if (ptr == NULL) {
    panic_track(CRABBY_BOX_OOM);
  }
  *ptr = value;
  return (Self){.ptr = ptr};
}

#define box_drop(T, self) TEMPLATE(box_drop, T, self)
static inline void box_drop(T, Self self) {
#ifdef T_DROP
  T_DROP(*self.ptr);
#endif
  free(self.ptr);
}

#undef Self
