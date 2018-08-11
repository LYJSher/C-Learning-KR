# include <stdio.h>
# include "calc.h"
# define MAXVAL 100 // ջval��������
int sp = 0; // ��һ������ջλ��
double val[MAXVAL]; // ֵջ

// ��fѹ��ֵջ��
void push(double f){
    if(sp < MAXVAL)
        val[sp++] = f;
    else
        printf("error: stack full, can't push %g\n",f);
}

// ����������ջ����ֵ
double pop(){
    if(sp > 0)
        return val[--sp];
    else{
        printf("error: stack empty\n");
        return 0.0;
    }
}