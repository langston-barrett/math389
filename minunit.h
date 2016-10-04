/* file: langston-minunit.h */
#ifndef __minunit_h
#define __minunit_h

#include <string.h>

#define __BASENAME__                                                           \
  (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#define assert(test)                                                           \
  do {                                                                         \
    tests_run++;                                                               \
    if (!(test)) {                                                             \
      tests_failed++;                                                          \
      snprintf(message, 1024, "%s failed:\n\t%s:%d: %s", __func__, __FILE__,   \
               __LINE__, #test);                                               \
      return message;                                                          \
    }                                                                          \
  } while (0)

// compare two int64_ts
#define assert_int64_eq(expected, actual)                                      \
  do {                                                                         \
    tests_run++;                                                               \
    if (expected != actual) {                                                  \
      tests_failed++;                                                          \
      snprintf(message, 1024,                                                  \
               "%s failed:\n\t%s:%d: expected %s == %li, got %li", __func__,   \
               __FILE__, __LINE__, #actual, (int64_t)expected,                 \
               (int64_t)actual);                                               \
      return message;                                                          \
    }                                                                          \
  } while (0)

// TODO: assert_str_eq
#define run_test(test)                                                         \
  do {                                                                         \
    char *message = test();                                                    \
    if (message)                                                               \
      return message;                                                          \
  } while (0)

#define test_main(all_tests_function)                                          \
  do {                                                                         \
    char *result = all_tests_function();                                       \
    if (result != 0) {                                                         \
      printf("%s\n", result);                                                  \
    }                                                                          \
    printf("%s: Passed: %lu\n", __BASENAME__, tests_run - tests_failed);       \
    printf("%s: Failed: %lu\n", __BASENAME__, tests_failed);                   \
    printf("%s: Total: %lu\n", __BASENAME__, tests_run);                       \
    return (tests_failed != 0);                                                \
  } while (0)

#endif
