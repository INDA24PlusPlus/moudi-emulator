#include "emulator/emulator.h"
#include "transpiler.h"
#include "common.h"
#include "architecture/instructions.h"

#define LINE_BREAKER "-------------------------\n"

int main(int argc, char ** args){
    if (sizeof(struct raw_instruction) != 4) {
        println("RAW INSTRUCTION IS BIGGER THAN IT IS SUPPOSED TO BE");
        exit(1);
    }

    char transpile_flag = 0;

    if (argc == 1) {
        println("Please specify a source file");
        exit(1);
    }

    for (size_t i = 2; i < argc; ++i) {
        if (args[i][0] != '-') {
            continue;
        }

        switch (args[i][1]) {
            case 't':
                transpile_flag = 1; break;
            case '-':
                if (strcmp("transpile", args[i] + 2) == 0) {
                    transpile_flag = 1;
                }
        }

    }

    if (transpile_flag) {
        transpile(args[1]);
    } else {
        emulate(args[1]);
    }
}
