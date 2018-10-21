#include "main.h"

//ARM 프로세서 기능 구현 함수 모음

int carryOut;

//점프 기능
//branch, branch with link
void branch(const union BitField inst)
{
    BranchBitField bitField = inst.branch;
    int offset = bitField.offset;
    if (offset & 0x00800000)
        offset = -(0x00ffffff - offset + 1);
    if (bitField.L == 1)
        r[14] = r[15] - 4;
    r[15] = r[15] + offset * 4;
    pipeState = 0;
}

//레지스터 데이터 연산 기능
//AND, EOR, SUB, RSB, ADD, ADC, SBC, RSC, TST, TEQ, CMP, CMN, ORR, MOV, BIC, MVN
void data_processing(const union BitField inst)
{
    DataProcessingBitField bitField = inst.dataProcessing;
    unsigned int operand1 = r[bitField.Rn];
    unsigned int operand2 = bitField.I ? rotate(inst) : shifts(inst);
    unsigned int result;
    long long tmp = 0;
    switch (bitField.opcode)
    {
    case 0x0000: //AND
        result = operand1 & operand2;
        tmp = operand1 & operand2;
        break;
    case 0x0001: //EOR(exclusive or)
        result = operand1 ^ operand2;
        tmp = operand1 ^ operand2;
        break;
    case 0x0002: //SUB
        result = operand1 - operand2;
        tmp = operand1;
        tmp -= operand2;
        break;
    case 0x0003: //RSB
        result = operand2 - operand1;
        tmp = operand2;
        tmp -= operand1;
        break;
    case 0x0004: //ADD
        result = operand1 + operand2;
        tmp = operand1;
        tmp += operand2;
        break;
    case 0x0005: //ADC
        result = operand1 + operand2 + cpsr.c;
        tmp = operand1;
        tmp += operand2 + cpsr.c;
        break;
    case 0x0006: //SBC
        result = operand1 - operand2 + cpsr.c - 1;
        tmp = operand1;
        tmp += -operand2 + cpsr.c - 1;
        break;
    case 0x0007: //RSC
        result = operand2 - operand1 + cpsr.c - 1;
        tmp = operand2;
        tmp += -operand1 + cpsr.c - 1;
        break;
    case 0x0008: //TST
        result = operand1 & operand2;
        tmp = operand1 & operand2;
        break;
    case 0x0009: //TEQ
        result = operand1 ^ operand2;
        tmp = operand1 ^ operand2;
        break;
    case 0x000a: //CMP
        result = operand1 - operand2;
        tmp = operand1;
        tmp -= operand2;
        break;
    case 0x000b: //CMN
        result = operand1 + operand2;
        tmp = operand1;
        tmp += operand2;
        break;
    case 0x000c: //ORR
        result = operand1 | operand2;
        tmp = operand1 | operand2;
        break;
    case 0x000d: //MOV
        result = operand2;
        tmp = operand2;
        break;
    case 0x000e: //BIC
        result = operand1 & (-1 ^ operand2);
        tmp = operand1 & (-1 ^ operand2);
        break;
    case 0x000f: //MVN
        result = (-1 ^ operand2);
        tmp = (-1 ^ operand2);
        break;
    }
    if (bitField.S == 1 && r[bitField.Rd] != 15)
    {
        switch (bitField.opcode)
        {
        case 0x0000: //AND
        case 0x0001: //EOR(exclusive or)
        case 0x0008: //TST
        case 0x0009: //TEQ
        case 0x000c: //ORR
        case 0x000d: //MOV
        case 0x000e: //BIC
        case 0x000f: //MVN
            cpsr.n = ((tmp >> 31) & 1ll) == 1 ? 1 : 0;
            cpsr.z = (unsigned int)tmp == 0 ? 1 : 0;
            cpsr.c = carryOut ? 1 : 0;
            break;
        default: //SUB, RSB, ADD, ADC, SBC, RSC, CMP, CMN
            operand2 += cpsr.c;
            cpsr.n = ((tmp >> 31) & 1ll) == 1 ? 1 : 0;
            cpsr.z = (unsigned int)tmp == 0 ? 1 : 0;
            cpsr.c = tmp > 0xffffffff ? 1 : 0;
            cpsr.v = (((operand1 ^ operand2) >> 31) & 1) == 0 && (((operand1 ^ tmp) >> 31) & 1ll) == 1 ? 1 : 0;
            break;
        }
    }
    // 결과를 반영하는 연산의 경우
    if (!(bitField.opcode >= 0x0008 && bitField.opcode <= 0x000b))
        r[bitField.Rd] = result;
}

//데이터 프로세싱 rotate 부가 연산
unsigned int rotate(const union BitField inst)
{
    DataProcessingBitField bitField = inst.dataProcessing;
    unsigned int imm, rotate, result;
    imm = (bitField.operand2 & 0x000000ff);
    rotate = ((bitField.operand2 & 0x00000f00) >> 8) * 2;
    result = imm << (32 - rotate) | imm >> rotate;
    carryOut = (result >> 31);
    return result;
}

//데이터 프로세싱 쉬프트 부가 연산
unsigned int shifts(const union BitField inst)
{
    DataProcessingBitField bitField = inst.dataProcessing;
    int check = (bitField.operand2 & 0x0010) >> 4;
    int shiftType = (bitField.operand2 & 0x060) >> 5;
    int m, shiftAmount;
    unsigned int result;
    m = bitField.operand2 & 0x000f;
    if (check == 0)
        shiftAmount = (bitField.operand2 & 0xF80) >> 7;
    else
        shiftAmount = r[(bitField.operand2 & 0xF00) >> 8];
    switch (shiftType)
    {
    case 0x0: //logical left
        result = (unsigned int)r[m] << shiftAmount;
        if (shiftAmount != 0)
        {
            carryOut = (unsigned int)r[m] >> (32 - shiftAmount);
        }
        else{
            carryOut = cpsr.c;
        }
        break;
    case 0x1: //logical right
        result = (unsigned int)r[m] >> shiftAmount;
        carryOut = (unsigned int)r[m] >> (shiftAmount - 1);
        break;
    case 0x2: //arithmetic right
        result = r[m] >> shiftAmount;
        if (shiftAmount != 0)
        {
            carryOut = (unsigned int)r[m] >> (shiftAmount - 1) & 1;
        }
        else
        {
            carryOut = r[m] >> 31;
        }
        break;
    case 0x3: //rotate right
        result = r[m] >> shiftAmount | r[m] << (32 - shiftAmount);
        carryOut = (result >> 31);
        break;
    }
    return result;
}

//곱셈 연산 기능
//MUL, MLA
void multiply(const union BitField inst)
{
    MultiplyBitField bitField = inst.multiply;
    r[bitField.Rd] = r[bitField.Rm] * r[bitField.Rs];
    if (bitField.A == 1)
    {
        r[bitField.Rd] += r[bitField.Rn];
    }
    if (bitField.S == 1)
    {
        if (r[bitField.Rd] < 0)
        {
            cpsr.n = 1;
        }
        if (r[bitField.Rd] == 0)
        {
            cpsr.z = 0;
        }
    }
}

//단일 데이터 세이브 및 로드 기능
//LDR, STR
void single_data_transfer(const union BitField inst)
{
    SingleDataTransferBitField bitField = inst.singleDataTransfer;
    int m = bitField.offset & 0xf;
    int base;
    bool isPreIndex = false;
    if (bitField.I == 1)
    {
        r[m] = shifts(inst);
    }
    if (bitField.U == 0)
        bitField.offset = -bitField.offset;
    if (bitField.P == 1)
    {
        base = r[bitField.Rn] + bitField.offset;
        isPreIndex = true;
    }
    if (bitField.B == 1)
    {
        r[bitField.Rd] = r[bitField.Rd] & 0xff000000;
    }
    if (bitField.L == 0)
    { //STR
        write_back(base, r[bitField.Rd]);
    }
    else if (bitField.L == 1)
    { //LDR
        if (bitField.B == 1)
        {
            r[bitField.Rd] = 0xff000000 & load(base);
        }
        else
            r[bitField.Rd] = load(base);
    }
    if (!isPreIndex)
    {
        r[bitField.Rn] = r[bitField.Rn] + bitField.offset;
    }
    else if (bitField.W == 1)
    {
        r[bitField.Rn] = r[bitField.Rn] + bitField.offset;
    }
}

//데이터 블록 세이브 및 로드 기능
//STM, LDM
void block_data_transfer(const union BitField inst)
{
    BlockDataTransferBitField bitField = inst.blockDataTransfer;
    int base = r[bitField.Rn];
    int direction = 1;
    int i, size = 0;
    bool isPreIndex = false;
    bool selectedRegister[16] = {
        false,
    };
    if (bitField.P == 1)
        isPreIndex = true;
    for (i = 0; i < 16; i++)
    {
        if ((bitField.RegisterList >> i) & 0x1)
        {
            selectedRegister[i] = true;
            size += 4;
        }
    }
    if (bitField.U == 0)
    {
        direction = -1;
        base -= size;
    }
    if (bitField.L == 1)
    { ////LDM bitField
        for (i = 0; i < 16; i++)
        {
            if (selectedRegister[i] == true)
            {
                if (isPreIndex)
                {
                    if (bitField.W == 1)
                    {
                        r[bitField.Rn] += (4 * direction);
                    }
                    base += (4 * direction);
                }
                r[i] = load(base);
                if (!isPreIndex)
                {
                    if (bitField.W == 1)
                    {
                        r[bitField.Rn] += (4 * direction);
                    }
                    base += (4 * direction);
                }
            }
        }
    }
    else
    { //STM bitField
        for (i = 0; i < 16; i++)
        {
            if (selectedRegister[i] == true)
            {
                if (isPreIndex)
                {
                    if (bitField.W == 1)
                    {
                        r[bitField.Rn] += (4 * direction);
                    }
                    base += (4 * direction);
                }
                write_back(base, r[i]);
                if (!isPreIndex)
                {
                    if (bitField.W == 1)
                    {
                        r[bitField.Rn] += (4 * direction);
                    }
                    base += (4 * direction);
                }
            }
        }
    }
}

//데이터 스왑 기능
//SWP
void single_data_swap(const union BitField inst)
{
    SingleDataSwapBitField bitField = inst.singleDataSwap;
    if (bitField.B == 1)
    {
        r[bitField.Rd] = memory[r[bitField.Rn]] & 0xff000000;
        r[bitField.Rn] = r[bitField.Rm] & 0xff000000;
    }
    else
    {
        r[bitField.Rd] = memory[bitField.Rn];
        r[bitField.Rn] = r[bitField.Rm];
    }
}

//소프트웨어 인터럽트(해당 프로그램에서는 종료 명령)
//SWI
void software_interrupt()
{
    FILE *output = fopen("output.txt", "w");
    int i;
    for (i = 0; i < 16; i++)
        printf("r%d:\t%08x\n", i, r[i]);
    for (i = 0; i <= 12; i++)
        fprintf(output, "r%d:\t%08x\n", i, r[i]);
    fprintf(output, "r%d sp:\t%08x\n", i, r[13]);
    fprintf(output, "r%d lr:\t%08x\n", i, r[14]);
    fprintf(output, "r%d pc:\t%08x\n", i, r[15]);
    for (i = 0; i < 1 << 20; i += 4)
    {
        fprintf(output, "memory %x:\t%08x\n", i, memory[i]);
    }
    exit(1);
}

//BX 명령
void branch_and_exchange(const union BitField inst)
{
    unsigned int bitField = inst.inst;
    int m = (bitField >> 28);
    r[15] = r[m];
    pipeState = 0;
}

//상수 로드
void move_imm16(const union BitField inst)
{
    unsigned int bitField = inst.inst;
    int U = (bitField & 0x00400000) >> 22;
    int imm = ((bitField & 0x000f0000) >> 4) | (bitField & 0x00000fff);
    int d = (bitField & 0x0000f000) >> 12;
    if (U == 1)
    {
        r[d] = (r[d] & 0x0000ffff) | (imm << 16);
    }
    else
    {
        r[d] = (r[d] & 0xffff0000) | imm;
    }
}
