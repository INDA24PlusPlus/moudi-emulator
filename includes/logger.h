#pragma once
#include "common.h"
#include <execinfo.h>

#define BACKTRACE_LENGTH 10
#define ANSI_START "\033["
#define COLOR_RESET_ANSI_SEQUENCE ANSI_START "0m"

#define RESET ANSI_START "0m"
#define BOLD ANSI_START "1m"
#define RED ANSI_START "38:2:198:56:53m" BOLD
#define GREEN ANSI_START "38:2:81:172:56m" BOLD
#define YELLOW ANSI_START "33;1m"
#define BLUE ANSI_START "38:2:19:96:178m" BOLD
#define MAGENTA ANSI_START "35;1m"
#define CYAN ANSI_START "36;1m"
#define WHITE ANSI_START "37;1m"
#define GREY ANSI_START "38:2:125:125:125m"

#define ASSERT(expr, msg) _assert(expr, __FILE__, __LINE__, msg)
#define ASSERT1(expr) ASSERT(expr, "")

void _assert(char expr, const char * file, const unsigned line, const char * msg);
