#ifndef CRABBY_NUMBERS_H_
#define CRABBY_NUMBERS_H_

#include <stddef.h>
#include <stdint.h>

#define CRABBY_NUMBER
#define CRABBY_NO_UNDEF_T

typedef int8_t i8;
#define T i8
#include "checked.t.h"
#include "cmp.t.h"
#undef T

typedef int16_t i16;
#define T i16
#include "checked.t.h"
#include "cmp.t.h"
#undef T

typedef int32_t i32;
#define T i32
#include "checked.t.h"
#include "cmp.t.h"
#undef T

typedef int64_t i64;
#define T i64
#include "checked.t.h"
#include "cmp.t.h"
#undef T

typedef ptrdiff_t isize;
#define T isize
#include "checked.t.h"
#include "cmp.t.h"
#undef T

typedef uint8_t u8;
#define T u8
#include "checked.t.h"
#include "cmp.t.h"
#undef T

typedef uint16_t u16;
#define T u16
#include "checked.t.h"
#include "cmp.t.h"
#undef T

typedef uint32_t u32;
#define T u32
#include "checked.t.h"
#include "cmp.t.h"
#undef T

typedef uint64_t u64;
#define T u64
#include "checked.t.h"
#include "cmp.t.h"
#undef T

typedef size_t usize;
#define T usize
#include "checked.t.h"
#include "cmp.t.h"
#undef T

#undef CRABBY_NO_UNDEF_T
#undef CRABBY_NUMBER

#endif
