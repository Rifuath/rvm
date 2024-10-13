#ifndef _RVM_H
#define _RVM_H
/* ------------------------------------------------
 * Virtual Machine LC-3 By Rifey.
 * This Software is written for education purpose
 * with the Licence MIT.
 * Credit me if you use this code ;)
 * ------------------------------------------------*/
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// iam lazy so i like to write like this.
typedef uint8_t     u8;
typedef uint16_t    u16;
typedef uint32_t    u32;

//--------------------HARDWARE COMPONENTS--------------------

#define MAX_MEMORY (1 << 16) // 65536 or 64 Kilobytes

//Registers for LC-3
enum Registers
{
    R_R0 = 0,
    R_R1,
    R_R2,
    R_R3,
    R_R4,
    R_R5,
    R_R6,
    R_R7,
    R_PC,       //program counter register
    R_COND,     //condition flag register
    R_COUNT
};

//Operation Codes for LC-3
enum OperationCodes
{
    OP_BR = 0, // branch
    OP_ADD,    // add
    OP_LD,     // load
    OP_ST,     // store
    OP_JSR,    // jump register
    OP_AND,    // bitwise and
    OP_LDR,    // load register
    OP_STR,    // store register
    OP_RTI,    // unused
    OP_NOT,    // bitwise not
    OP_LDI,    // load indirect
    OP_STI,    // store indirect
    OP_JMP,    // jump
    OP_RES,    // reserved (unused)
    OP_LEA,    // load effective address
    OP_TRAP    // execute trap
};

// Condition Flags for LC-3 
enum ConditionFlags
{
    CF_POS   = 1 << 0, // Positive
    CF_ZERO  = 1 << 1, // Zero
    CF_NEG   = 1 << 2, // Negative
};

//Default hardcodes
enum Defaults
{ 
    PC_START = 0x3000 // Default Program Counter Starting Address
};



//-------------FUNCTION DECLARATIONS -------------
u16 mem_read(int index);
u16 read_image(const char* src);
u16 sign_extend(u16 x, int bit_count);
void update_condition_flag(u16 r);

// OPERATION FUNCTION DECLARATIONS
void BITWISE_ADD(u16* instruction);
void LOAD_INDIRECT(u16* instruction);
void BRANCH(u16* instruction);
void LOAD(u16* instruction);
void STORE(u16* instruction);



#endif //_RVM_H
