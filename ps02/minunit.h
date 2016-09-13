/* file: langston-minunit.h */
#define assert(message, test) do { if (!(test)) return message; } while (0)

#define run_test(test) do { char *message = test(); tests_run++; \
    if (message) return message; } while (0)
extern int tests_run;

#define test_main(all_tests_function) do { char *result = all_tests_function(); \
    if (result == 0) printf("All tests passed.\n"); \
    else if (result != 0) printf("fail: %s\n", result); \
    printf("Tests run: %d\n", tests_run); \
    return result != 0; } while (0)
