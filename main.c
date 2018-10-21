#include "main.h"

flags cpsr, spsr; // current program state register
unsigned int r[16]; // normal register
unsigned int memory[1 << 20];
int pipeState;

//Armv3 어셈블리 파일을 메모리에 로드 후 실행(개념상)
int main(int argc, char *argv[])
{
	int pc;
	if (argc < 2)
	{
		printf("usage : ./main filename.txt\n");
		return 0;
	}
	FILE *instFile = fopen(argv[1], "r");
	unsigned int insts[3] = {
		0,
	};
	unsigned int decodes[2];
	char line[100];
	if (instFile == NULL)
	{
		return -1;
	}
	fgets(line, 100, instFile);
	//스택 및 cpsr, 초기 시작 주소 설정
	cpsr.z = 1;
	cpsr.c = 1;
	r[13] = 0x10200;
	r[14] = 0;
	r[15] = str_to_int(line);
	//메모리에 프로그램 로드
	while (!feof(instFile))
	{
		fgets(line, 100, instFile);
		unsigned int address = str_to_int(line);
		unsigned int value = str_to_int(line + find_pos(line, ':'));
		memory[address] = value;
	}
	fclose(instFile);
	while (1)
	{
		//virtual pipeline
		if (pipeState >= 12)
		{
			pc = r[15];
			execute(insts[0], decodes[0]);
			// printf("%8x %8x %8x %8x %8x %8x %8x %8x %8x %8x\n", insts[0], pc - 8, r[14], r[13], r[12], r[11], r[3], r[2], r[1], r[0]);
			// printf("%8x %8x %8x %8x\n", cpsr.n, cpsr.z, cpsr.v, cpsr.c);
		}
		if (pipeState >= 8)
		{
			decodes[1] = decode(insts[1]);
		}
		if (pipeState >= 4)
		{
			if (r[15] == 0)
			{
				software_interrupt();
			}
			insts[2] = fetch(r[15]);
			r[15] += 4;
		}
		decodes[0] = decodes[1];
		insts[0] = insts[1];
		insts[1] = insts[2];
		if (pipeState < 12)
			pipeState += 4;
	}
	return 0;
}
