#pragma once
#include "common.h"

struct Parser {
    char * src;
    size_t length;
    size_t index;
    size_t line, pos;
};

struct Parser init_parser(char * path);
void transpile(char * path);
