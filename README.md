# Build

1. git clone https://github.com/INDA24PlusPlus/moudi-emulator
2. cd moudi-emulator
3. git submodule init
4. git submodule update
5. ```./start.sh c```

# Run example

- ```./start.sh examples/fib.asm -t && ./start.sh out```

# Architecture

## Instruction

- inst : 1 byte
- reg : 3 bits
- arguments_type : 2 bits
- reg_value : 3 bits
- literal_value : 2 bytes

