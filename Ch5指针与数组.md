# Ch5 指针与数组

### 5.1 指针与地址

~~~c
p = &c // 把c的地址赋给变量p
~~~

**地址运算符&**只能应用于**内存中**的对象，即**变量与数组元素**，不能作用于表达式、常量或register类型的变量

**间接寻址或间接引用运算符***当它作用于指针时，将访问指针所指向的对象

~~~c
#include<stdio.h>

int main(){
    int x = 1, y = 2, z[10];
    int *ip; // ip是指向int类型的指针
    ip = &x; // ip现在指向x
    y = *ip; // y的值现在为1
    *ip = 0; // x的值现在为0
    ip = &z[0]; // ip现在指向z[0]
}
~~~

对函数的声明也可以采用这种方法。

~~~c
double *dp, atof(char*); // *dp和atof(s)的值都是double类型，且atof的参数是一个指向char类型的指针
~~~

**每个指针都必须指向某种特定的数据类型**

*注*：一个例外情况是**指向void类型的指针可以存放指向任何类型的指针**，但它**不能间接引用其自身**（5.11详细讨论）

如果指针ip指向整型变量x，那么在x可以出现的任何上下文中都可以使用*ip, 因此语句

~~~~c
*ip = *ip + 10;
~~~~

把ip指向的值 即x 的值增加10

**一元运算符*和&的优先级比算术运算符的优先级高**

~~~c
y = *ip + 1; // 把*ip指向的对象的值取出并加1，然后再将结果赋予y
~~~

~~~c
*ip += 1; // 将ip指向的对象的值加1 等同于++*ip 或 (*ip)++ 
~~~

*注* ：

**(*ip)++**中的圆括号是必须的，否则该表达式将对ip进行加1运算，而不是对ip指向的对象进行加1运算，这是因为类似于*****和**++**这样的一元运算符遵循从右至左的结合顺序。



由于指针也是变量，所以在程序中可以直接使用，而不必通过间接引用的方法使用，例如，如果iq是另一个指向整型的指针

~~~c
iq = ip; // 将把ip中的值拷贝到iq中，这样指针iq也将指向ip指向的对象
~~~

### 5.2 指针与函数参数

C语言是以**传值**的方式将参数值传递给被调用函数，因此被调用函数**不能直接**修改主调函数中变量的值

例如

~~~c
void swap(int x, int y){
    int temp;
    temp = x;
    x = y;
    y = temp;
}

~~~

由于参数传递采用传值方式，因此上述的swap函数不会影响到调用它的例程中参数a和b的值

![1534240652280](Ch5指针与数组.assets/1534240652280.png)

为实现交换的目的，可以使主调程序将指向所要交换的变量的指针传递给被调函数即

~~~c
#include<stdio.h>

void swap(int *px, int *py){ // 通过指针间接访问操作数
    int temp;
    temp = *px;
    *px = *py;
    *py = temp;
}

int main(){
    int a = 1, b = 2;
    swap(&a, &b); // &a：指向变量a的指针
    printf("a:%d b:%d", a, b);
}
~~~

![1534240875565](Ch5指针与数组.assets/1534240875565.png)



函数getint接受自由格式的输入，并执行转换，将输入的字符流分解成整数，且每次调用得到一个整数

将标识是否到达文件结尾的状态作为getint函数的返回值，同时使用一个指针参数储存转换后得到的整数并传回给主调函数

~~~c
int n, array[SIZE], getint(int*);

for(n=0; n<SIZE && getint(&array[n])!=EOF; n++);// 必须将array[n]的地址传递给函数getint
											// 否则函数getint将无法把转换得到的整数传回调用者
~~~

*注*

每次调用getint时，输入流中的下一个整数 将 被赋值给数组元素array[n]，同时n的值＋1



~~~c
#include<stdio.h>
#include<ctype.h>

int getch(void);
void ungetch(int);

// 该版本的getint在到达文件结尾时返回EOF，当下一个输入不是数字时返回0，当输入中包含一个有意义的数字时传回给调用者
// 将输入中的下一个整形数赋给*pn
int getint(int *pn){
    int c, sign;
    while(isspace(c = getch())) // 跳过空白符
    ;
    if(!isdigit(c) && c!=EOF && c!='+' && c!= '-'){
        ungetch(c); // 输入不是一个数字
        return 0;
    }
    sign = (c == '-') ? -1 : 1;
    if(c == '+' || c == '-') // 这里的c是上方getch得到的
        c = getch();
    for(*pn=0; isdigit(c); c=getch())
        *pn = 10 * *pn + (c - '0');
    *pn *= sign; // 恢复其正负号 并同时将转换后的整数返回给主调函数
    if(c != EOF) // 上方得到的c
        ungetch(c);
    return c;
}

# define BUFSIZE 100

char buf[BUFSIZE]; // 用于ungetch()函数的缓冲区
int bufp = 0; // buf中下一个空闲位置

// 取一个字符（可能是压回的字符）
int getch(void){
    return (bufp > 0) ? buf[--bufp] : getchar();
}

// 把字符压回输入中
void ungetch(int c){
    if(bufp >= BUFSIZE)
        printf("ungetch: too many characters\n");
    else
        buf[bufp++] = c;
}

int main(){
    int n, array[5]={0}, getint(int*);
    for(n=0; n<5 && getint(&array[n])!=EOF; n++);// 必须将array[n]的地址传递给函数getint
                                                // 否则函数getint将无法把转换得到的整数传回调用者
    int i;
    for(i=0; i<5; i++)
        printf("%d\n",array[i]);
}
~~~

#### 5-1 在上面的例子中，如果符号+或-的后面紧跟的不是数字，getint函数将把符号视为数字0的有效表达式。修改函数，将这种形式的+或-符号重新写回输入流中

~~~c
#include<stdio.h>
#include<ctype.h>

int getch(void);
void ungetch(int);

// 将输入中的下一个整形数赋给*pn
int getint(int *pn){
    int c, sign;
    while(isspace(c = getch())) // 跳过空白符
    ;
    if(!isdigit(c) && c!=EOF && c!='+' && c!= '-'){
        ungetch(c); // 输入不是一个数字
        return 0;
    }
    sign = (c == '-') ? -1 : 1;
    if(c == '+' || c == '-'){
        char tempsign = c;
        if(!isdigit(c=getch())){
            ungetch(tempsign);
            if(c != EOF)
                ungetch(c);
            return tempsign;
        }
    }
    for(*pn=0; isdigit(c); c=getch())
        *pn = 10 * *pn + (c - '0');
    *pn *= sign; // 恢复其正负号 并同时将转换后的整数返回给主调函数
    if(c != EOF) // 上方得到的c
        ungetch(c);
    return c;
}

# define BUFSIZE 100

char buf[BUFSIZE]; // 用于ungetch()函数的缓冲区
int bufp = 0; // buf中下一个空闲位置

// 取一个字符（可能是压回的字符）
int getch(void){
    return (bufp > 0) ? buf[--bufp] : getchar();
}

// 把字符压回输入中
void ungetch(int c){
    if(bufp >= BUFSIZE)
        printf("ungetch: too many characters\n");
    else
        buf[bufp++] = c;
}

int main(){
    int n, array[5]={1,1,1,1,1}, getint(int*);
    for(n=0; n<5 && getint(&array[n])!=EOF; n++);// 必须将array[n]的地址传递给函数getint
                                                // 否则函数getint将无法把转换得到的整数传回调用者
    int i;
    for(i=0; i<5; i++)
        printf("%d\n",array[i]);
    return 0;
}
~~~

