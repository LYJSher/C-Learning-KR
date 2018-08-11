# include <stdio.h>
# include <ctype.h>
# include "calc.h"
# include "getch.c"
int getop(char s[]){
    int i, c;
    while((s[0] = c = getch()) == ' ' || c == '\t'); // 看下面注3

    s[1] = '\0';
    if(!isdigit(c) && c != '.')
        return c; // 不是数
    i = 0;
    if(isdigit(c)) // 收集整数部分  看下面注3
        while(isdigit(s[++i] = c = getch()));
    if(c == '.') // 收集小数部分
        while(isdigit(s[++i] = c = getch()));
    s[i] = '\0';
    if(c != EOF)
        ungetch(c);
    return NUMBER;
}
