#pragma once
#include "common.h"
#include "architecture/registers.h"

// instructions which have a choice between REG and VAL need to be even
enum instructions {
    NOP = 0,

    POP,
    PUSH,
    MOV,

    NOT,
    AND,
    OR,
    XOR,
    
    ADD,
    SUB,
    MUL,
    DIV,
    REM,

    JMP,  // JUMP
    JNZ,  // JUMP NOT ZERO

    PRINT, // print value as number
    PUT // print value as characther
};

#define INSTRUCTION_SET_EL(name, args) { #name, (sizeof(#name) / sizeof(char)) - 1, args, name}

#define LITERAL_VALUE_BIT_SIZE 16
#define MAX_LITERAL_VALUE ((1 << LITERAL_VALUE_BIT_SIZE) - 1)

#define INST_ARG_IS_LITERAL(inst, index) ((inst).arguments_type & (1 << index))

struct raw_instruction {
    unsigned int inst : 8;
    unsigned int reg : 3;
    unsigned int arguments_type : 2; // Bitwise [0: REG, 1: LITERAL]
    unsigned int reg_value : 3;
    unsigned int literal_value : LITERAL_VALUE_BIT_SIZE; // if a literal can fit into here then it will be stored here instead of after the instruction
};

const static struct instruction_data {
    char * inst_name;
    size_t inst_name_size;
    size_t arguments;
    enum instructions inst_code;
} instruction_set[] = {
    INSTRUCTION_SET_EL(NOP, 0),

    INSTRUCTION_SET_EL(POP, 1),
    INSTRUCTION_SET_EL(PUSH, 1),
    INSTRUCTION_SET_EL(MOV, 2),

    INSTRUCTION_SET_EL(NOT, 1),
    INSTRUCTION_SET_EL(AND, 2),
    INSTRUCTION_SET_EL(OR, 2),
    INSTRUCTION_SET_EL(XOR, 2),
    
    INSTRUCTION_SET_EL(ADD, 2),
    INSTRUCTION_SET_EL(SUB, 2),
    INSTRUCTION_SET_EL(MUL, 2),
    INSTRUCTION_SET_EL(DIV, 2),
    INSTRUCTION_SET_EL(REM, 2),
    
    INSTRUCTION_SET_EL(JMP, 1),
    INSTRUCTION_SET_EL(JNZ, 2),

    INSTRUCTION_SET_EL(PRINT, 1),
    INSTRUCTION_SET_EL(PUT, 1),
};
