# ARMv3(ARM7500FE) Processor Emulator

- Armv3 명령어를 시뮬레이트하고 메모리와 레지스터를 덤프하는 프로그램입니다. C언어로 작성되었습니다. 

## 사용 방법

1. MakeFile이 있는 경로에서 make 명령 실행
2. ./main.out filename.txt 명령 실행

- 실행이 완료되면 output.txt 파일이 생성됩니다.  

## 예제 코드

### 어셈블리 코드
``` assembly
00008354 <main>:
    8354:	e3a00002 	mov	r0, #2

00008358 <LOOP>:
    8358:	e2500001 	subs	r0, r0, #1
    835c:	1afffffd 	bne	8358 <LOOP>
    8360:	e1a0200e 	mov	r2, lr
    8364:	eb000001 	bl	8370 <main2>
    8368:	e1a0e002 	mov	lr, r2
    836c:	e12fff1e 	bx	lr

00008370 <main2>:
    8370:	e3a00003 	mov	r0, #3
    8374:	e12fff1e 	bx	lr
```

### C 코드
``` C
int i;

void main2(){
    i = 3;
}

void main(){
    i = 2;
    while(i--);
    main2();
}
```

## 이슈

- 보조 프로세서 관련 연산 구현 X
- PSR Transfer 연산 구현 X
- Float-Point 연산 구현 X
- CPSR C, V 플래그 설정 관련 문제 존재