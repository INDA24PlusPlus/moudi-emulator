#include "emulator/cpu.h"
#include "architecture/registers.h"
#include "fmt.h"
#include "architecture/instructions.h"
#include "logger.h"

void cpu_stack_init(struct CPU * cpu) {
    cpu->stack_size = CPU_STACK_MAX_SIZE;
    cpu->stack = calloc(cpu->stack_size, CPU_STACK_ELEMENT_SIZE);
}

void cpu_stack_grow(struct CPU * cpu) {
    cpu->stack = realloc(cpu->stack, (cpu->stack_size + CPU_STACK_DEFAULT_SIZE) * CPU_STACK_ELEMENT_SIZE);
    memset(cpu->stack + cpu->stack_size, 0, CPU_STACK_DEFAULT_SIZE * CPU_STACK_ELEMENT_SIZE);
    cpu->stack_size += CPU_STACK_DEFAULT_SIZE;
}

void cpu_stack_push(struct CPU * cpu, long value) {
    ASSERT(cpu->registers[RSP] < CPU_STACK_MAX_SIZE, "Emulator: A Stack Overflow has occured");
    cpu->stack[++cpu->registers[RSP]] = value;
}

void cpu_stack_pop(struct CPU * cpu) {
    ASSERT(cpu->registers[RSP] > 0, "Emulator: A Stack Underflow has occured");
    cpu->registers[RSP] -= 1;
}

long cpu_stack_get(struct CPU * cpu) {
    ASSERT(cpu->registers[RSP] > 0, "Emulator: A Stack Underflow has occured");
    return cpu->stack[cpu->registers[RSP]];
}

void cpu_set_register(struct CPU * cpu, enum registers reg, long value) {
    if (reg >= REGISTER_COUNT) {
        println("Emulator: Trying to set an unknown register number {i}", reg);
        exit(1);
    }

    cpu->registers[reg] = value;
}


long cpu_get_value_from_instruction(struct CPU * cpu, struct raw_instruction inst, size_t index) {
    if (index >= 2) {
        println("Emulator: Invalid instruction argument {i}", index + 1);
        exit(1);
    }

    if (INST_ARG_IS_LITERAL(inst, index)) {
        return inst.literal_value;
    } else {
        return cpu->registers[index == 0 ? inst.reg : inst.reg_value];
    }
}

void cpu_print_instruction(struct raw_instruction instruction) {
    struct instruction_data lookup = instruction_set[instruction.inst];
    if (lookup.inst_code != instruction.inst) {
        println("CPU: Invalid instruction lookup. {i} != {i}", lookup.inst_code, instruction.inst);
        exit(1);
    }
    print("{s} [Code: {i}, Arguments Type: {x}, Arguments: [", lookup.inst_name, lookup.inst_code, instruction.arguments_type);
    for (size_t i = 0; i < lookup.arguments; ++i) {
        if (i != 0) {
            print(", ");
        }
        if (INST_ARG_IS_LITERAL(instruction, i)) {
            print("{i}", instruction.literal_value);
        } else {
            print("{s}", registers_lookup[i == 0 ? instruction.reg : instruction.reg_value]);
        }
    }
    println("]]");
}
