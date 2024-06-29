#include <stdio.h>
#include <stdint.h>
#include <signal.h>

#define MEMORY_MAX (1 << 16) 

uint16_t memory[MEMORY_MAX]; 
uint16_t reg[R_COUNT];

/*
Registers
R0-R7   general purpose registers
PC      program counter register
COND    conditions flag register 
*/
enum
{
    R_R0 = 0,
    R_R1,
    R_R2,
    R_R3,
    R_R4,
    R_R5,
    R_R6,
    R_R7,
    R_PC,
    R_COND,
    R_COUNT
};

/*
Condition Flags
POS     positive
ZRO     zero
NEG     negative
*/
enum
{
    FL_POS = 1 << 0,
    FL_ZRO = 1 << 1,
    FL_NEG = 1 << 2,
};

/*
Opcodes
branch, add, load, store, jump reg, bitwise and,
load reg, stor reg, RTI (not used), bitwise not, load indirect, store indirect, jump,
reserved (not used),
*/
enum
{
    OP_BR = 0,
    OP_ADD,
    OP_LD,
    OP_ST,
    OP_JSR,
    OP_AND,
    OP_LDR,
    OP_STR,
    OP_RTI,
    OP_NOT,
    OP_LDI,
    OP_STI,
    OP_JMP,
    OP_RES,
    OP_LEA,
    OP_TRAP
};
