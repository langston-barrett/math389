/* file: langston-minunit.h */
#include <string.h>

#define assert(message, test) do { if (!(test)) return message; } while (0)

#define run_test(test) do { char *message = test(); tests_run++; \
    if (message) return message; } while (0)
extern int tests_run;

#define __BASENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#define test_main(all_tests_function) do { char *result = all_tests_function(); \
    if (result == 0) printf("%s: All tests passed.\n", __BASENAME__); \
    else if (result != 0) printf("%s: fail: %s\n", __BASENAME__, result); \
    printf("%s: Tests run: %d\n", __BASENAME__, tests_run);              \
    return result != 0; } while (0)
