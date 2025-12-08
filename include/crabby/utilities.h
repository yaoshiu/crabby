#ifndef CRABBY_UTILITIES_H_
#define CRABBY_UTILITIES_H_

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define CONCAT_RAW(a, b) a##b
#define CONCAT(a, b) CONCAT_RAW(a##_, b)

#define TEMPLATE(func, type, ...) CONCAT(func, type)(__VA_ARGS__)

#define TRACK(func, type, ...)                                                 \
  CONCAT(func, type)(__VA_ARGS__, __FILE__, __LINE__)

#define THREAD_NAME_BUFFER 64
static inline const char *current_thread_name() {
  static _Thread_local char name[THREAD_NAME_BUFFER];
  if (pthread_getname_np(pthread_self(), name, sizeof(name)) != 0) {
    return "unknown";
  }
  return name;
}

#define panic(message) panic_track(message, __FILE__, __LINE__)
static inline void panic_track(const char *message, const char *file,
                               int line) {
  fprintf(stderr, "thread '%s' panicked at '%s', %s:%d\n",
          current_thread_name(), message, file, line);
  abort();
}

#endif // CRABBY_UTILITIES_H_
