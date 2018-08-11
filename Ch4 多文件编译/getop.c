# include <stdio.h>
# include <ctype.h>
# include "calc.h"
# include "getch.c"
int getop(char s[]){
    int i, c;
    while((s[0] = c = getch()) == ' ' || c == '\t'); // ������ע3

    s[1] = '\0';
    if(!isdigit(c) && c != '.')
        return c; // ������
    i = 0;
    if(isdigit(c)) // �ռ���������  ������ע3
        while(isdigit(s[++i] = c = getch()));
    if(c == '.') // �ռ�С������
        while(isdigit(s[++i] = c = getch()));
    s[i] = '\0';
    if(c != EOF)
        ungetch(c);
    return NUMBER;
}
