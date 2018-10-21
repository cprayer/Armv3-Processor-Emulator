#include "main.h"

// 문자열을 읽어나가다 첫 번째로 발견하는 16진수의 값을 반환
// ex) _```1234a@@ -> return 0x1234a
unsigned int str_to_int(const char *str)
{
    int i = 0, num = 0;
    int flag_decimal, flag_alphabet;
    flag_decimal = str[i] >= '0' && str[i] <= '9';
    flag_alphabet = str[i] >= 'a' && str[i] <= 'f';
    for (; !(flag_decimal || flag_alphabet); i++)
    {
        flag_decimal = str[i + 1] >= '0' && str[i + 1] <= '9';
        flag_alphabet = str[i + 1] >= 'a' && str[i + 1] <= 'f';
    }
    for (; flag_decimal || flag_alphabet; i++)
    {
        if (flag_decimal)
            num += (str[i] - '0');
        else if (flag_alphabet)
            num += (str[i] - 'a' + 10);
        flag_decimal = str[i + 1] >= '0' && str[i + 1] <= '9';
        flag_alphabet = str[i + 1] >= 'a' && str[i + 1] <= 'f';
        if (flag_decimal || flag_alphabet)
            num *= 16;
    }
    return num;
}

//str[pos] == ch를 처음으로 만족하는 pos의 위치 반환
int find_pos(const char *str, const char ch)
{
    int pos;
    for (pos = 0; str[pos] != ch; pos++)
        ;
    return pos;
}