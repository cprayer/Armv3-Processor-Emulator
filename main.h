#ifndef _STDIO_H_
#define _STDIO_H_
#include <stdio.h>
#endif

#ifndef _STDLIB_H_
#define _STDLIB_H_
#include <stdlib.h>
#endif

#ifndef _STDBOOL_H_
#define _STDBOOL_H_
#include <stdbool.h>
#endif

#include "bitfieldset.h"

/* Current program state register*/
#ifndef _FLAGS_H_
#define _FLAGS_H_
typedef struct flags{

	bool z;
	bool c;
	bool n;
	bool v;

}flags;
#endif

extern flags cpsr;// current program state register
extern unsigned int r[16];// normal register
extern unsigned int memory[1 << 20];
extern int pipeState;

//instruction.c
void branch(union BitField inst);
void data_processing(union BitField inst);
unsigned int rotate(union BitField inst);
unsigned int shifts(union BitField inst);
void multiply(union BitField inst);
void single_data_transfer(union BitField inst);
void block_data_transfer(union BitField bitField);
void single_data_swap(union BitField inst);
void branch_and_exchange(union BitField inst);
void move_imm16(union BitField inst);
void software_interrupt();

//util.c
unsigned int str_to_int(const char *str);
int find_pos(const char *str, const char ch);

//pipeline.c
unsigned int fetch(const unsigned int address);
unsigned int load(const unsigned int address);
int decode(const unsigned int inst);
bool check_conds(const unsigned int inst);
void execute(unsigned int inst, int instType);
void write_back(const unsigned int address, const unsigned int data);