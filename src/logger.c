#include "logger.h"

#define BACKTRACE_LENGTH 10

void _assert(char expr, const char * file, const unsigned line, const char * msg) {
    if (!expr) {
        void * arr[BACKTRACE_LENGTH + 1];
        size_t size = backtrace(arr, BACKTRACE_LENGTH + 1), i;
        char ** strings = backtrace_symbols(arr, BACKTRACE_LENGTH + 1);
        if (strings != NULL) {
            println("Backtrace of last {i} function calls:", size - 1);
            for (size_t i = 1; i < size; ++i) {
                println("{i}: {s}", i, strings[i]);
            }
        }
        println(YELLOW "{s}" RESET ":" CYAN "{u}" RESET RED "\nASSERTION ERROR" RESET ": " RED "{s}" RESET, file, line, msg);
        exit(1);
    }
}

