#include <stdio.h>
#include <stdint.h>
#include <signal.h>
/* windows only */
#include <Windows.h>
#include <conio.h>

#define MEMORY_MAX (1 << 16) 

uint16_t memory[MEMORY_MAX]; 
uint16_t reg[R_COUNT];

/*
Registers
R0-R7   general purpose registers
PC      program counter register
COND    conditions flag register 
*/
enum{
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
enum{
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
enum{
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

/*
trap codes
*/
enum{
    TRAP_GETC = 0x20,  /* get character from keyboard, not echoed onto the terminal */
    TRAP_OUT = 0x21,   /* output a character */
    TRAP_PUTS = 0x22,  /* output a word string */
    TRAP_IN = 0x23,    /* get character from keyboard, echoed onto the terminal */
    TRAP_PUTSP = 0x24, /* output a byte string */
    TRAP_HALT = 0x25   /* halt the program */
};

/*
memory mapped registers
*/
enum{
    MR_KBSR = 0xFE00,
    MR_KBDR = 0xFE02  
};

/*
input buffering
*/
HANDLE hStdin = INVALID_HANDLE_VALUE;
DWORD fdwMode, fdwOldMode;
void disable_input_buffering(){
    hStdin = GetStdHandle(STD_INPUT_HANDLE);
    GetConsoleMode(hStdin, &fdwOldMode); /* save old mode */
    fdwMode = fdwOldMode
            ^ ENABLE_ECHO_INPUT  /* no input echo */
            ^ ENABLE_LINE_INPUT; /* return when one or
                                    more characters are available */
    SetConsoleMode(hStdin, fdwMode); /* set new mode */
    FlushConsoleInputBuffer(hStdin); /* clear buffer */
}

void restore_input_buffering(){
    SetConsoleMode(hStdin, fdwOldMode);
}

uint16_t check_key(){
    return WaitForSingleObject(hStdin, 1000) == WAIT_OBJECT_0 && _kbhit();
}

void handle_interrupt(int signal){
    restore_input_buffering();
    printf("\n");
    exit(-2);
}

/*
sign extend
*/
uint16_t sign_extend(uint16_t x, int bit_count){
    if((x >> (bit_count-1))& 1){
        x |= (0xFFFF << bit_count);
    }
    return x;
}

/*
swap
*/
uint16_t swap16(uint16_t x){
    return (x << 8) | (x >> 8);
}

/*
update flags
*/
void update_flags(uint16_t r){
    if(reg[r] == 0){
        reg[R_COND] = FL_ZRO;
    } else if(reg[r] >> 15){
        reg[R_COND] = FL_NEG;
    } else {
        reg[R_COND] = FL_POS;
    }
}

/*
read image file
*/
void read_image_file(FILE* file){
    uint16_t origin;
    fread(&origin, sizeof(origin), 1, file);
    origin = swap16(origin);
    uint16_t max_read = MEMORY_MAX - origin;
    uint16_t* p = memory + origin;
    size_t read = fread(p, sizeof(uint16_t), max_read, file);
    while(read-- > 0){
        *p = swap16(*p);
        ++p;
    }
}

/*
read image
*/
int read_image(const char* image_path){
    FILE* file = fopen(image_path, "rb");
    if (!file) { return 0; }
    read_image_file(file);
    fclose(file);
    return 1;
}

/*
memory access
*/
void mem_write(uint16_t address, uint16_t value){
    memory[address] = value;
}

uint16_t mem_read(uint16_t address){
    if(address == MR_KBSR){
        if(check_key()){
            memory[MR_KBSR] = (1 << 15);
            memory[MR_KBDR] = getchar();
        } else {
            memory[MR_KBSR] = 0;
        }
    }
    return memory[address];
}

int main (int argc, const char* argv[])
{
    if (argc < 2){
        printf("lc3 [image-file1]... \n");
        exit(2);
    }
    for (int j = 1 ; j < argc; ++j){
        if(!read_image(argv[j])){
            printf("failed to load image: %s\n", argv[j]);
            exit(1);
        }
    }

    signal(SIGINT, handle_interrupt);
    disable_input_buffering();

    reg[R_COND] = FL_ZRO;
    enum { PC_START = 0x3000};
    reg[R_PC] = PC_START;

    int running = 1;
    while(running){
        uint16_t instr = mem_read(reg[R_PC]++);
        uint16_t op = instr >> 12;
        switch (op)
        {
            case OP_ADD:
                //ADD
                break;
            case OP_AND:
                //AND
                break;
            case OP_NOT:
                //NOT
                break;
            case OP_BR:
                //BR
                break;
            case OP_JMP:
                //JMP
                break;
            case OP_JSR:
                //JSR
                break;
            case OP_LD:
                //LD
                break;
            case OP_LDI:
                //LDI
                break;
            case OP_LDR:
                //LDR
                break;
            case OP_LEA:
                //LEA
                break;
            case OP_ST:
                //ST
                break;
            case OP_STI:
                //STI
                break;
            case OP_STR:
                //STR
                break;
            case OP_TRAP:
                //TRAP
                break;
            case OP_RES:
            case OP_RTI:
            default:
                //BAD OPCODE
                break;
        }
    }
restore_input_buffering();
}