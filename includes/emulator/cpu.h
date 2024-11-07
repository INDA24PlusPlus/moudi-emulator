#include "common.h"
#include "architecture/instructions.h"
#include "emulator/emulator.h"

struct CPU {
    char * src;
    size_t src_length;

    long * stack;
    size_t stack_size;

    long registers[REGISTER_COUNT];
    long RIP;
};

#define CPU_STACK_ELEMENT_SIZE sizeof(long)
#define CPU_STACK_DEFAULT_SIZE 256
#define CPU_STACK_MAX_SIZE (CPU_STACK_DEFAULT_SIZE << 8)

void cpu_stack_init(struct CPU * cpu);
void cpu_stack_grow(struct CPU * cpu);

void cpu_stack_push(struct CPU * cpu, long value);
void cpu_stack_pop(struct CPU * cpu);
long cpu_stack_get(struct CPU * cpu);

void cpu_set_register(struct CPU * cpu, enum registers reg, long value);
long cpu_get_value_from_instruction(struct CPU * cpu, struct raw_instruction inst, size_t index);

void cpu_print_instruction(struct raw_instruction instruction);
