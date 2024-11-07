#pragma once

#include "common.h"

FILE * open_file (const char * filename, const char * options);
char* read_file(const char * filename, size_t * length);
