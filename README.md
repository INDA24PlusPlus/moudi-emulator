# Build

1. git clone https://github.com/INDA24PlusPlus/moudi-compiler
2. cd moudi-compiler
3. git submodule init
4. git submodule update
5. ```./start.sh c```


# Architecture

## Instruction

- inst : 1 byte
- reg : 3 bits
- arguments_type : 2 bits
- reg_value : 3 bits
- literal_value : 2 bytes

