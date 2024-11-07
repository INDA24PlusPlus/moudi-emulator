#pragma once

#define GENERATE_ENUM_ELEMENT(EL) EL,

#define GENERATE_LOOKUP_TABLE_ELEMENT(EL) {#EL, EL, (sizeof(#EL) / sizeof(char)) - 1},
#define GENERATE_LOOKUP_TABLE(FOREACH_LIST) FOREACH_LIST(GENERATE_LOOKUP_TABLE_ELEMENT)

#define REGISTER_LIST(f) \
    f(R1)\
    f(R2)\
    f(R3)\
    f(R4)\
    f(R5)\
    f(R6)\
    f(RSP)\
    f(RBP)

#define REGISTER_COUNT 8

enum registers {
    REGISTER_LIST(GENERATE_ENUM_ELEMENT)
};

const static struct register_data {
    char * name;
    enum registers value;
    unsigned int length;
} registers_lookup[] = {
    GENERATE_LOOKUP_TABLE(REGISTER_LIST)
};
