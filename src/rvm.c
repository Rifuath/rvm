#include "rvm.h"
/* ------------------------------------------------
 * Virtual Machine LC-3 By Rifey.
 * This Software is written for education purpose
 * with the Licence MIT.
 * Credit me if you use this code ;)
 * ------------------------------------------------*/

u16 globalMemory[MAX_MEMORY]; // `128` KiloBytes or `131, 072` Bytes
u16 globalRegisters[R_COUNT]; // `10` Registers


int main(int argc, char** argv)
{

    // ------------------Load Arguments-----------------------
    //if no arugument is provided
    if(argc < 2)
    {
        printf("Usage: \n\trvm [image-file].. \n");
        exit(2);
    }
    for(int i = 1; i < argc; i++)
    {
        if(!read_image(argv[i]))
        {
            printf("[ERROR] Failed to load the image {%s}\n", argv[i]);
            exit(1);
        }

    }

    //TODO(Rifey): setup code here

    //Exactly one condition flag at a time. for start ZERO FLAG
    globalRegisters[R_COND] = CF_ZERO;

    //Set the Program Counter Starting Position
    // 0x3000 is the default
    globalRegisters[R_PC] = PC_START;


    int Running = 1; //true
    while(Running)
    {
        //get the instruction from the memory at the location of Program counter and increment 
        //the program counter.
        u16 instruction = mem_read(globalRegisters[R_PC]++);
        //Get the Operation Code from the instruction.
        //the OPCode is at the first 4 bits
        u16 opcode = instruction >> 12;

        //go through the opcode with the appropriate instruction for 16 opcodes 
        switch (opcode)
        {
            case OP_BR: // Branch Instruction -> see doc-two.md
            {
                BRANCH(&instruction);
            } break;

            case OP_ADD: //Bitwise ADD operation ->see doc-one.md
            {
                BITWISE_ADD(&instruction);
            } break;

            case OP_LD: // Load Operation -> see doc-two.md
            {
                LOAD(&instruction);
            } break;

            case OP_ST: // Store operation -> see doc-two.md
            {
                STORE(&instruction);
            } break;

            case OP_JSR:
            {
                //TODO(Rifey): Jump Register Operation here
            } break;

            case OP_AND:
            {
                //TODO(Rifey): Bitwise And Operation here
            } break;

            case OP_LDR:
            {
                //TODO(Rifey): Load Register Operation here
            } break;

            case OP_STR:
            {
                //TODO(Rifey): Store Register Operation here
            } break;

            case OP_RTI:
            {
                //TODO(Rifey): Unused Operation here
            } break;

            case OP_NOT:
            {
                //TODO(Rifey): Bitwise Not Operation here
            } break;

            case OP_LDI: //Load Indirect. ->see doc-two.md
            {
                LOAD_INDIRECT(&instruction);
            } break;

            case OP_STI:
            {
                //TODO(Rifey): Store Indirect Operation here
            } break;

            case OP_JMP:
            {
                //TODO(Rifey): Jump Operation here
            } break;

            case OP_RES:
            {
                //TODO(Rifey): Reserved (Unused) Operation here
            } break;

            case OP_LEA:
            {
                //TODO(Rifey): Load Effective Address Operation here
            } break;

            case OP_TRAP:
            {
                //TODO(Rifey): Trap Operation here
            } break;

            default:
            {
                //TODO(Rifey): Bad OpCode here.
            } break;
        }
    }

    //TODO(Rifey): Shutdown here
    return(0);
}

//-------------FUNCTION DEFINITIONS-------------
//This function returns the 16 bit memory block of the given index, from the loaded image.
u16 mem_read(int index)
{
    //TODO(Rifey): Implement Mem_read for each instruction
    return 0;
}
u16 read_image(const char* src)
{
    //TODO(Rifey): Implement Read_Image, that takes is the src name
    return 0;
}

//function to extend 5 bit to 16 bit and handle signed numbers.
//Fill 0s for positive and 1s for negative
u16 sign_extend(u16 x, int bit_count)
{
    if( (x >> (bit_count - 1) ) & 1)
    {
        x |= (0xFFFF << bit_count);
    }
    return x;
}

//Function that updated the R_COND register with the appropriate
//flags, (Zero, Negative, Postive) depends on the last instruction.
void update_condition_flag(u16 r)
{
    if(globalRegisters[r] == 0)
    {
        globalRegisters[R_COND] = CF_ZERO;
    }
    else if(globalRegisters[r] >> 15) // if the last bit is 1, then we will set negative
    {
        globalRegisters[R_COND] = CF_NEG;
    }
    else 
    {
        globalRegisters[R_COND] = CF_POS;
    }
}

//------------OPERATION FUNCTIONS DEFINITIONS----------------
void BITWISE_ADD(u16* instruction)
{
    // Destination Register DR
    u16 r0 = ((*instruction) >> 9)  & 0x7;
    //First Operand SR1
    u16 r1 = ((*instruction) >> 6) & 0x7;
    //mode check, immediate
    u16 imm_flag = ((*instruction) >> 5) & 0x1;

    if(imm_flag) // Immediate Mode
    {
        //Check Docs-one for mor info.
        u16 imm5 = sign_extend((*instruction) & 0x1F, 5);
        globalRegisters[r0] = globalRegisters[r1] + imm5;
    }
    else //Register Mode
    {
        u16 r2 = (*instruction) & 0x7;
        globalRegisters[r0] = globalRegisters[r1] + globalRegisters[r2];

    }
    //and lastly update the COND register
    update_condition_flag(r0);
}


void LOAD_INDIRECT(u16* instruction)
{
    // Destination Register DR
    u16 r0 = ((*instruction) >> 9)  & 0x7;
    // PCoffset 9 
    u16 pc_offset = sign_extend((*instruction) & 0x1FF, 9);
    // Update Current PC, look into that address to get the final address
    globalRegisters[r0] = mem_read(mem_read(globalRegisters[R_PC] + pc_offset));
    //and lastly update the COND register
    update_condition_flag(r0);
}


void BRANCH(u16* instruction)
{
    // PCoffset 9 
    u16 pc_offset = sign_extend((*instruction) & 0x1FF, 9);
    //condition flag
    u16 condition_flag = ((*instruction) >> 9) & 0x7;

    //If the COND register value and conditio flag match then branch by incrementing pc_offset 
    //with Program Counter
    if(condition_flag & globalRegisters[R_COND])
    {
        globalRegisters[R_PC] += pc_offset;
    }

}

void LOAD(u16* instruction)
{
    //DR (Destinatin Register)
    u16 r0 = ((*instruction) >> 9) & 0x7;
    //pcOffset9
    u16 pc_offset = sign_extend( (*instruction) & 0x1FF, 9);
    //Directly load to r0 from the calculated address
    globalRegisters[r0] = mem_read(globalRegisters[R_PC] + pc_offset);
    //Finally update the condition flag
    update_condition_flag(r0);
}


void STORE(u16* instruction)
{
    //SR ( Source Register )
    u16 r0 = ((*instruction) >> 9) & 0x7;
    //pc offset
    u16 pc_offset = sign_extend( (*instruction) & 0x1FF, 9);
    //store the value from the register to the calculated offset address
    globalRegisters[R_PC + pc_offset] = r0;
}
