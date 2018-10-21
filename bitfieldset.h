#ifndef _BIT_FIELD_SET_H_
#define _BIT_FIELD_SET_H_

typedef struct _DataProcessingBitField
{
    unsigned operand2 : 12;
    unsigned Rd : 4;
    unsigned Rn : 4;
    unsigned S : 1;
    unsigned opcode : 4;
    unsigned I : 1;
    unsigned _0 : 2;
    unsigned cond : 4;
} DataProcessingBitField;

typedef struct _MultiplyBitField
{
    unsigned Rm : 4;
    unsigned _9 : 4;
    unsigned Rs : 4;
    unsigned Rn : 4;
    unsigned Rd : 4;
    unsigned S : 1;
    unsigned A : 1;
    unsigned _0 : 6;
    unsigned cond : 4;
} MultiplyBitField;

typedef struct _SingleDataSwapBitField
{
    unsigned Rm : 4;
    unsigned _9 : 8;
    unsigned Rd : 4;
    unsigned Rn : 4;
    unsigned _0 : 2;
    unsigned B : 1;
    unsigned _2 : 5;
    unsigned cond : 4;
} SingleDataSwapBitField;

typedef struct _SingleDataTransferBitField
{
    unsigned offset : 12;
    unsigned Rd : 4;
    unsigned Rn : 4;
    unsigned L : 1;
    unsigned W : 1;
    unsigned B : 1;
    unsigned U : 1;
    unsigned P : 1;
    unsigned I : 1;
    unsigned _1 : 2;
    unsigned cond : 4;
} SingleDataTransferBitField;

typedef struct _UndefinedInstructionBitField
{
    unsigned _unused0 : 4;
    unsigned _1 : 1;
    unsigned _unused1 : 20;
    unsigned _3 : 3;
    unsigned cond : 4;
} UndefinedInstructionBitField;

typedef struct _BlockDataTransferBitField
{
    unsigned RegisterList : 16;
    unsigned Rn : 4;
    unsigned L : 1;
    unsigned W : 1;
    unsigned S : 1;
    unsigned U : 1;
    unsigned P : 1;
    unsigned _4 : 3;
    unsigned cond : 4;
} BlockDataTransferBitField;

typedef struct _BranchBitField
{
    unsigned offset : 24;
    unsigned L : 1;
    unsigned _5 : 3;
    unsigned cond : 4;
} BranchBitField;

typedef struct _CoprocDataTransferBitField
{
    unsigned offset : 8;
    unsigned cpnum : 4;
    unsigned CRd : 4;
    unsigned Rn : 4;
    unsigned L : 1;
    unsigned W : 1;
    unsigned N : 1;
    unsigned U : 1;
    unsigned P : 1;
    unsigned _6 : 3;
    unsigned cond : 4;
} CoprocDataTransferBitField;

typedef struct _CoprocDataOperationBitField
{
    unsigned CRm : 4;
    unsigned _0 : 1;
    unsigned CP : 3;
    unsigned cpnum : 4;
    unsigned CRd : 4;
    unsigned CRn : 4;
    unsigned CPopc : 4;
    unsigned _14 : 4;
    unsigned cond : 4;
} CoprocDataOperationBitField;

typedef struct _CoprocRegisterTransferBitField
{
    unsigned CRm : 4;
    unsigned _1 : 1;
    unsigned CP : 3;
    unsigned cpnum : 4;
    unsigned CRd : 4;
    unsigned crn : 4;
    unsigned L : 1;
    unsigned CPopc : 3;
    unsigned _14 : 4;
    unsigned cond : 4;
} CoprocRegisterTransferBitField;

typedef struct _SoftwareInterruptBitField
{
    unsigned _unused0 : 24;
    unsigned _15 : 4;
    unsigned cond : 4;
} SoftwareInterruptBitField;

typedef struct _Mull
{
    unsigned rm : 4;
    unsigned _nine : 4;
    unsigned rs : 4;
    unsigned RdLo : 4;
    unsigned RdHi : 4;
    unsigned s : 1;
    unsigned u : 2;
    unsigned _one : 5;
    unsigned cond : 4;
} Mull;

union BitField{
    unsigned int inst;
    DataProcessingBitField dataProcessing;
    MultiplyBitField multiply;
    SingleDataSwapBitField singleDataSwap;
    SingleDataTransferBitField singleDataTransfer;
    UndefinedInstructionBitField undefinedInstruction;
    BlockDataTransferBitField blockDataTransfer;
    BranchBitField branch;
    CoprocDataTransferBitField coprocDataTransfer;
    CoprocDataOperationBitField coprocDataOperation;
    CoprocRegisterTransferBitField coprocRegisterTransfer;
    SoftwareInterruptBitField softwareInterrupt;
};

#endif