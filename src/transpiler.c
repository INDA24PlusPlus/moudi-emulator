#include "transpiler.h"
#include "emulator/cpu.h"
#include "architecture/registers.h"
#include "fmt.h"
#include "architecture/instructions.h"
#include "io.h"
#include "logger.h"
#include "slice.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Parser init_parser(char * path) {
    struct Parser parser = {.line = 1, .pos = 1};
    parser.src = read_file(path, &parser.length);
    return parser;
}

static inline char parser_get_char(struct Parser * parser) {
    ASSERT(parser->index < parser->length, "Index out of bounds");
    return parser->src[parser->index];
}

void parser_eat(struct Parser * parser, char cmp) {
    char c = parser_get_char(parser);
    parser->index += 1;

    if (c != cmp) {
        println("{2i::} Unexpected characther '{c}'", parser->line, parser->pos);
        exit(1);
    }

    parser->pos += 1;
}

void parser_skip_whitespace(struct Parser * parser) {
    while (parser->index < parser->length) {
        char c = parser_get_char(parser);
        switch (c) {
            case ' ':
            case '\t':
                break;
            default:
                return;
        }
        parser->pos += 1;
        parser->index += 1;
    }
}

void parser_skip_whitespace_and_newline(struct Parser * parser) {
    while (parser->index < parser->length) {
        char c = parser_get_char(parser);
        switch (c) {
            case ' ':
            case '\t':
            case '\n':
                break;
            default:
                return;
        }
        parser->pos += 1;
        parser->index += 1;
    }
}

size_t parser_length_to_next_whitespace(struct Parser * parser) {
    size_t length = 0;
    char c = parser_get_char(parser);
    while (c > ' ') {
        length += 1;
        parser->index += 1;
        c = parser_get_char(parser);
    }

    return length;
}

long parser_parse_arg(struct Parser * parser, struct raw_instruction * inst, size_t arg_index) {
    char is_reg = parser_get_char(parser) == 'R';

    char * start_of_arg = parser->src + parser->index;
    size_t length = parser_length_to_next_whitespace(parser);
    struct Slice reg_slice = init_slice(start_of_arg, length);
    
    if (!is_reg) {
        inst->arguments_type |= (1 << arg_index);
        long literal = strtol(start_of_arg, NULL, 10);
        return literal;
    }

    char * slice_str = slice_to_string(&reg_slice);
    struct register_data found;

    size_t i = 0;
    const size_t size = sizeof(registers_lookup) / sizeof(registers_lookup[0]);
    for (; i < size; ++i) {
        found = registers_lookup[i];
        if (length == found.length && strncasecmp(found.name, reg_slice.start, found.length) == 0) {
            break;
        }
    }

    if (i == size) {
        println("{2i::} Unsupported register '{s}'", parser->line, parser->pos, slice_to_string(&reg_slice));
        exit(1);
    }

    return found.value;
}

struct raw_instruction parser_parse_instruction(struct Parser * parser) {
    struct raw_instruction instruction = {0};
    struct instruction_data found;

    char * start_of_inst = parser->src + parser->index;
    size_t length = parser_length_to_next_whitespace(parser);
    struct Slice inst_slice = init_slice(start_of_inst, length);

    char * slice_str = slice_to_string(&inst_slice);

    size_t i = 0;
    const size_t size = sizeof(instruction_set) / sizeof(instruction_set[0]);
    for (; i < size; ++i) {

        found = instruction_set[i];
        if (length == found.inst_name_size && strncasecmp(found.inst_name, inst_slice.start, found.inst_name_size) == 0) {
            break;
        }
    }

    if (i == size) {
        println("{2i::} Unsupported instruction '{s}'", parser->line, parser->pos, slice_to_string(&inst_slice));
        exit(1);
    }

    instruction.inst = found.inst_code;

    for (size_t i = 0; i < found.arguments; ++i) {
        parser_skip_whitespace(parser);
        long value = parser_parse_arg(parser, &instruction, i);

        if (INST_ARG_IS_LITERAL(instruction, i)) {
            ASSERT(value < MAX_LITERAL_VALUE, "Literal value is bigger than MAX_LITERAL_VALUE. This is not supported yet.");
            instruction.literal_value = value;
        } else if (i == 0) {
            instruction.reg = value;
        } else if (i == 1) {
            instruction.reg_value = value;
        } else {
            ASSERT(0, "UNHANDLED PANIC");
        }
    }

    return instruction;
}

void parse_line(struct Parser * parser, FILE * fp) {
    union data {
        struct raw_instruction instruction;
        char buf[4];
    } data;

    parser_skip_whitespace_and_newline(parser);
    data.instruction = parser_parse_instruction(parser);

    /* cpu_print_instruction(data.instruction); */

    writef(fp, "{4c}", data.buf[0], data.buf[1], data.buf[2], data.buf[3]);
    while (parser->index < parser->length && parser_get_char(parser) != '\n') {
        parser->index += 1;
    }
    if (parser->index < parser->length) {
        parser_skip_whitespace_and_newline(parser);
    }
}

void transpile(char * path) {
    struct Parser parser = init_parser(path);
    FILE * fp = open_file("./out", "wb");
    while (parser.index < parser.length) {
        parse_line(&parser, fp);
    }
}
