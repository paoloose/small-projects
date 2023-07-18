#pragma once

#include <string.h> // strrchr()

#define APP_RESET   "\x1b[0m"
#define APP_RED     "\x1b[31m"
#define APP_MAGENTA "\x1b[35m"
#define APP_BLUE    "\x1b[34m"
#define APP_CYAN    "\x1b[36m"
#define APP_BRIGHT_RED   "\x1b[91m"
#define APP_BRIGHT_GREEN "\x1b[92m"

// Returns the filename only
#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

// Environment
#define APP_ENV_IS_DEV  1
#define APP_ENV_IS_PROD 0

#if APP_ENV_IS_DEV
#   define APP_LOG(str, ...) \
        printf( \
            APP_CYAN "%s:" APP_BLUE "%d(" APP_CYAN "%s" APP_BLUE "): " APP_RESET str "\n", \
            __FILENAME__, \
            __LINE__, \
            __func__, \
            ##__VA_ARGS__ \
        );
#else
#   define APP_LOG(str, ...) ((void)0)
#endif

#define APP_ASSERT(to_be_true, message) \
    if (!(to_be_true)) { \
        fprintf( \
            stderr, \
            APP_RED "%s:%d(" APP_BRIGHT_RED "%s" APP_RED "): assertion failed: " APP_RESET message "\n", \
            __FILENAME__, \
            __LINE__, \
            __func__ \
        ); \
        exit(1); \
    }
