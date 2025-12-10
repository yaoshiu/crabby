#include <stdio.h>

#include "crabby/numbers.h"

#define T i32
#include "crabby/vec.t.h"
#undef T

#define T Vec(i32)
#define T_DROP(self) vec_drop(i32, self)
#define T_CLONE(self) vec_clone(i32, self)
#include "crabby/option.t.h"
#include "crabby/vec.t.h"
#undef T
#undef T_DROP
#undef T_CLONE

i32 main() {
  Vec(i32) a = vec_new(i32);
  Vec(i32) b = vec_new(i32);
  Vec(Vec(i32)) c = vec_new(Vec(i32));

  vec_push(i32, &a, 1);
  vec_push(i32, &b, 2);
  vec_push(i32, &b, 3);

  vec_push(Vec(i32), &c, vec_clone(i32, &a));
  vec_push(Vec(i32), &c, b);
  vec_push(Vec(i32), &c, a);

  for (usize i = 0; i < c.len; i++) {
    Vec(i32) *a = &c.data[i];
    printf("%zu: ", a->len);
    for (usize i = 0; i < a->len; i++) {
      printf("%d ", a->data[i]);
    }
    printf("\n");
  }

  vec_drop(Vec(i32), c);

  return 0;
}
