# include <stdio.h>
# define BUFSIZE 100
char buf[BUFSIZE]; // ����ungetch()�����Ļ�����
int bufp = 0; // buf����һ������λ��

// ȡһ���ַ���������ѹ�ص��ַ���
int getch(void){
    return (bufp > 0) ? buf[--bufp] : getchar();
}

// ���ַ�ѹ��������
void ungetch(int c){
    if(bufp >= BUFSIZE)
        printf("ungetch: too many characters\n");
    else
        buf[bufp++] = c;
}
