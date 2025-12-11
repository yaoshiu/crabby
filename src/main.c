#include <stdio.h>

#include <crabby/numbers.h>

#define T i32
#include <crabby/vec.t.h>
#undef T

#define T Vec(i32)
#define T_DROP(self) vec_drop(i32, self)
#include <crabby/option.t.h>
#include <crabby/vec.t.h>
#undef T
#undef T_DROP

i32 main() {
  Vec(i32) a = vec_new(i32);
  Vec(i32) b = vec_new(i32);

  vec_push(i32, &a, 1);
  vec_push(i32, &b, 2);
  vec_push(i32, &b, 3);

  Vec(Vec(i32)) c =
      vec_from(Vec(i32), ((Vec(i32)[]){vec_clone(i32, &a), b, a}), 3);

  usize len = c.len;
  for (usize i = 0; i < len + 1; i++) {
    Vec(i32) a = option_unwrap(Vec(i32), vec_pop(Vec(i32), &c));
    while (a.len != 0) {
      i32 val = option_unwrap(i32, vec_pop(i32, &a));
      printf("%d ", val);
    }
    vec_drop(i32, a);
    printf("\n");
  }

  vec_drop(Vec(i32), c);

  return 0;
}
