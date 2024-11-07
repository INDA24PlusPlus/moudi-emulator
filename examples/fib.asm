MOV R1 10  // Fib of this number
JNZ R1 5 // Start of fib function
MOV R1 0
JMP 19 // Jump to end of fib function
MOV R2 2 // i
MOV R3 0 // temp
MOV R4 1 // prev
MOV R5 1 // curr
MOV R6 R2 // Start of loop
SUB R6 R1 // n - i
JNZ R6 13 // n != i
JMP 18 // Jump to end of loop
MOV R3 R5 // temp = curr
ADD R5 R4 // curr += prev
MOV R4 R3 // prev = temp
ADD R2 1  // i += 1
JMP 9 // Jump to start of loop
MOV R1 R5
PRINT R1 // End of fib function
PUT 10
