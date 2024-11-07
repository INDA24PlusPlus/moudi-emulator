#include "emulator/emulator.h"
#include "emulator/cpu.h"
#include "architecture/registers.h"
#include "common.h"
#include "architecture/instructions.h"
#include "fmt.h"
#include "io.h"
#include "logger.h"

void emulator_error(struct CPU * cpu, struct raw_instruction instruction, char * reason) {
    print("Emulator Error(RIP: {li}, Line: {li}) Invalid instruction: {s}\n\t", cpu->RIP, (cpu->RIP / 4) + 1, reason);
    cpu_print_instruction(instruction);
}

void emulate_pop(struct CPU * cpu, struct raw_instruction instruction) {
    if (INST_ARG_IS_LITERAL(instruction, 0)) {
        emulator_error(cpu, instruction, "Argument 1 must be a register");
        exit(1);
    }
    
    cpu_set_register(cpu, instruction.reg, cpu_stack_get(cpu));
}

void emulate_push(struct CPU * cpu, struct raw_instruction instruction) {
    cpu_stack_push(cpu, cpu_get_value_from_instruction(cpu, instruction, 0));
}

void emulate_mov(struct CPU * cpu, struct raw_instruction instruction) {
    if (INST_ARG_IS_LITERAL(instruction, 0)) {
        emulator_error(cpu, instruction, "Argument 1 must be a register");
        exit(1);
    }

    cpu_set_register(cpu, instruction.reg, cpu_get_value_from_instruction(cpu, instruction, 1));
}

#define GENERATE_EMULATE_OP_FUNCTION(NAME, OPERAND) \
    long NAME ##_op(long lhs, long rhs) {return lhs OPERAND rhs; }

GENERATE_EMULATE_OP_FUNCTION(and, +);
GENERATE_EMULATE_OP_FUNCTION(or, |);
GENERATE_EMULATE_OP_FUNCTION(xor, ^);
GENERATE_EMULATE_OP_FUNCTION(add, +);
GENERATE_EMULATE_OP_FUNCTION(sub, -);
GENERATE_EMULATE_OP_FUNCTION(mul, *);
GENERATE_EMULATE_OP_FUNCTION(div, /);
GENERATE_EMULATE_OP_FUNCTION(rem, %);

long not_op(long lhs, long rhs) {
    return ~rhs;
}

void emulate_op(struct CPU * cpu, struct raw_instruction instruction, long f(long, long)) {
    if (INST_ARG_IS_LITERAL(instruction, 0)) {
        emulator_error(cpu, instruction, "Argument 1 must be a register");
        exit(1);
    }

    long left = cpu_get_value_from_instruction(cpu, instruction, 0), right = cpu_get_value_from_instruction(cpu, instruction, 1);

    cpu_set_register(cpu, instruction.reg, f(left, right));
}

void emulate_jmp(struct CPU * cpu, struct raw_instruction instruction) {
    cpu->RIP = (cpu_get_value_from_instruction(cpu, instruction, 0) - 1) * 4;
}

void emulate_jnz(struct CPU * cpu, struct raw_instruction instruction) {
    if (INST_ARG_IS_LITERAL(instruction, 0)) {
        emulator_error(cpu, instruction, "Argument 1 must be a register");
        exit(1);
    }

    if (cpu_get_value_from_instruction(cpu, instruction, 0)) {
        cpu->RIP = (cpu_get_value_from_instruction(cpu, instruction, 1) - 1) * 4;
    }
}

void emulate_print(struct CPU * cpu, struct raw_instruction instruction) {
    print("{li}", cpu_get_value_from_instruction(cpu, instruction, 0));
}

void emulate_put(struct CPU * cpu, struct raw_instruction instruction) {
    print("{c}", cpu_get_value_from_instruction(cpu, instruction, 0));
}

void emulate(char * path) {
    struct CPU cpu = {0};
    union data {
        struct raw_instruction instruction;
        char buf[4];
    } data;

    cpu.src = read_file(path, &cpu.src_length);

    while (cpu.RIP < cpu.src_length) {
        memcpy(data.buf, cpu.src + cpu.RIP, 4);
        cpu.RIP += 4;

        switch (data.instruction.inst) {
            case POP:
                emulate_pop(&cpu, data.instruction); break;
            case PUSH:
                emulate_push(&cpu, data.instruction); break;
            case MOV:
                emulate_mov(&cpu, data.instruction); break;

            case NOT:
                emulate_op(&cpu, data.instruction, not_op); break;
            case AND:
                emulate_op(&cpu, data.instruction, and_op); break;
            case OR:
                emulate_op(&cpu, data.instruction, or_op); break;
            case XOR:
                emulate_op(&cpu, data.instruction, xor_op); break;
            case ADD:
                emulate_op(&cpu, data.instruction, add_op); break;
            case SUB:
                emulate_op(&cpu, data.instruction, sub_op); break;
            case MUL:
                emulate_op(&cpu, data.instruction, mul_op); break;
            case DIV:
                emulate_op(&cpu, data.instruction, div_op); break;
            case REM:
                emulate_op(&cpu, data.instruction, rem_op); break;

            case JMP:
                emulate_jmp(&cpu, data.instruction); break;
            case JNZ:
                emulate_jnz(&cpu, data.instruction); break;

            case PRINT:
                emulate_print(&cpu, data.instruction); break;
            case PUT: // print value as characther
                emulate_put(&cpu, data.instruction); break;
            default:
                println("Emulator: Invalid instruction code {x}", data.instruction.inst);
                exit(1);
        }
    }
}
