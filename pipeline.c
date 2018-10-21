#include "main.h"

//파이프라인 기능 구현 함수 구현 모음

//메모리에서 명령어를 가지고 옴
unsigned int fetch(const unsigned int address)
{
    return load(address);
}

//메모리에 저장된 값 로드
unsigned int load(const unsigned int address)
{
    return memory[address];
}

//명령어 해석(실행 여부 포함)
int decode(const unsigned int inst)
{
    if (check_conds(inst))
    {
        if ((inst & 0x0fc00090) == 0x00000090)
        {
            //printf("Mul\n");
            return 1;
        }
        else if ((inst & 0x0ffffff0) == 0x012fff10)
        {
            //printf("BX\n");
            return 11;
        }
        else if ((inst & 0x0fb00ff0) == 0x01000090)
        {
            //printf("Single data swap\n");
            return 2;
        }
        else if ((inst & 0x0fb00000) == 0x03000000)
        {
            //printf("MoveImm16\n");
            return 12;
        }
        else if ((inst & 0x0c000000) == 0x00000000)
        {
            //printf("Data Processing\n");
            return 3;
        }
        else if ((inst & 0x0e000010) == 0x06000010)
        {
            //printf("Undefined instruction\n");
            return 4;
        }
        else if ((inst & 0x0c000000) == 0x04000000)
        {
            //printf("Single data transfer\n");
            return 5;
        }
        else if ((inst & 0x0e000000) == 0x08000000)
        {
            //printf("Block data trnsfer\n\n\n");
            return 6;
        }
        else if ((inst & 0x0e000000) == 0x0a000000)
        {
            //printf("Branch\n");
            return 7;
        }
        else if ((inst & 0x0f000010) == 0x0e000000)
        {
            //printf("Coproc data operation\n");
            return 8;
        }
        else if ((inst & 0x0f000010) == 0x0e000010)
        {
            //printf("Coproc register transfer\n");
            return 9;
        }
        else if ((inst & 0x0f000000) == 0x0f000000)
        {
            //printf("Swi\n");
            return 10;
        }
    }
    return -1;
}

bool check_conds(const unsigned int inst)
{
    int conds = (inst & 0xf0000000) >> 28;
    switch (conds)
    {
    case 0x0000: //0000 (EQ, equal)
    {
        if (cpsr.z == 1)
            return 1;
        else
            return 0;
    }
    case 0x0001: //0001 (NE, not equal)
    {
        if (cpsr.z == 0)
            return 1;
        else
            return 0;
    }
    case 0x0002: //0010 (CS, unsigned higher or same)
    {
        if (cpsr.c == 1)
            return 1;
        else
            return 0;
    }
    case 0x0003: //0011 (CC, unsigned lower)
    {
        if (cpsr.c == 0)
            return 1;
        else
            return 0;
    }
    case 0x0004: //0100 (MI, negative)
    {
        if (cpsr.n == 1)
            return 1;
        else
            return 0;
    }
    case 0x0005: //0101 (PL, positive or zero)
    {
        if (cpsr.n == 0)
            return 1;
        else
            return 0;
    }
    case 0x0006: //0110 (VS, overflow)
    {
        if (cpsr.v == 1)
            return 1;
        else
            return 0;
    }
    case 0x0007: //0111 (VC, no overflow)
    {
        if (cpsr.v == 0)
            return 1;
        else
            return 0;
    }
    case 0x0008: //1000 (HI, unsigned higher)
    {
        if (cpsr.c == 1 && cpsr.z == 0)
            return 1;
        else
            return 0;
    }
    case 0x0009: //1001 (LS, unsigned lower or same)
    {
        if (cpsr.c == 0 || cpsr.z == 1)
            return 1;
        else
            return 0;
    }
    case 0x000a: //1010 (GE, greater or equal)
    {
        if ((cpsr.n == 1 && cpsr.v == 1) ||
            (cpsr.n == 0 && cpsr.v == 0))
            return 1;
        else
            return 0;
    }
    case 0x000b: //1011 (LT, less than)
    {
        if ((cpsr.n == 1 && cpsr.v == 0) ||
            (cpsr.n == 0 && cpsr.v == 1))
            return 1;
        else
            return 0;
    }
    case 0x000c: //1100 (GT, greater than)
    {
        if (cpsr.z == 0 &&
            ((cpsr.n == 1 && cpsr.v == 1) ||
             (cpsr.n == 0 && cpsr.v == 0)))
            return 1;
        else
            return 0;
    }
    case 0x000d: //1101 (LE, less than or equal)
    {
        if (cpsr.z == 1 ||
            (cpsr.n == 1 && cpsr.v == 0) ||
            (cpsr.n == 0 && cpsr.v == 1))
            return 1;
        else
            return 0;
    }
    case 0x000e: //1110 (AL, always)
    {
        return 1;
    }
    case 0x000f: //1111 (NV, never)
    {
        return 0;
    }
    default:
        return -1;
    }
}

//명령어 실행
void execute(unsigned int inst, int instType)
{
    switch (instType)
    {
    case -0x0001: //undefined instruction or do not executed instruction.
        // printf("this instruction is not executed\n");
        break;
    case 0x0001: //Multifly
        multiply((union BitField)inst);
        break;
    case 0x0002: //Single Data Swap
        single_data_swap((union BitField)inst);
        break;
    case 0x0003: //Data Processing PSR Transfer
        data_processing((union BitField)inst);
        break;
    case 0x0004: //Undefined
        break;
    case 0x0005: //Single Data Transfer
        single_data_transfer((union BitField)inst);
        break;
    case 0x0006: //Block Data Transfer
        block_data_transfer((union BitField)inst);
        break;
    case 0x0007: //Branch
        branch((union BitField)inst);
        break;
    case 0x0008: //Coproc data operation
        break;
    case 0x0009: //Coproc data Transfer
        break;
    case 0x000a: //Software Interrupt
        software_interrupt();
        break;
    case 0x000b: //Branch and Exchange
        branch_and_exchange((union BitField)inst);
        break;
    case 0x000c: //Move Immediate 16bit
        move_imm16((union BitField)inst);
        break;
    }
}

//메모리에 연산 결과를 반영
void write_back(const unsigned int address, const unsigned int data)
{
    memory[address] = data;
}