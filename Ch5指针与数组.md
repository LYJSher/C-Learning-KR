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

#### 5-2 模仿getint的实现方法，编写一个读取浮点数的函数getfloat。getfloat的返回类型应该是什么？

返回类型int 因为getfloat函数将返回EOF或者紧跟在浮点数后面那个字符的ASCII值，因此返回int

~~~c
#include<stdio.h>
#include<ctype.h>

int getch(void);
void ungetch(int);

// 模仿getint的实现方法，编写一个读取浮点数的函数getfloat
// 自己写的版本
int getfloat(float *pn){
    char c;
    int sign = 1; // 记录符号 默认为正

    while(isspace(c = getch())); // 跳过空白符

    if(!isdigit(c) && c!='+' && c!='-' && c!='.' && c!=EOF){ // 非数字返回0
        ungetch();
        return 0;
    }

    char tempsign; // 记录符号
    if(c == '+' || c == '-'){
        sign = (c == '-') ? -1 : 1;
        tempsign = c;
        c = getch();
    }
    if(!isdigit(c)){ // + - 后跟的不是数字
        ungetch(tempsign);
        if(c != EOF)
            ungetch(c);
    }
    else{ // + - 后为数字
        *pn = 0.0; // 初始为0
        for(; isdigit(c); c=getch()){ // 整数部分
            *pn = 10.0 * *pn + (c - '0');
        }
        if(c == '.'){ // 有小数部分
            float decimal = 0; // 记录小数部分
            int i = 1; // 记录小数位数
            for(; isdigit(c = getch()); i*=10){
                decimal = 10.0 * decimal + (c - '0');
            }
            if(decimal != 0){
                decimal = decimal / i;
            }
            *pn = *pn + decimal;
        }
        else if(c != EOF)
            ungetch(c);
        *pn = sign * *pn; // 保证正负号正确
        return c;
    }
}

// 教材
int getfloat(float *pn){
    int c, sign;
    float power;

    while(isspace(c = getch()));

    if(!isdigit(c) && c!=EOF && c!='.' && c!='+' && c!='-'){
        ungetch(c);
        return 0;
    }
    sign = (c == '-') ? -1 : 1;
    if(c == '+' || c == '-')
        c = getch();
    for(*pn = 0.0; isdigit(c); c = getch())
        *pn = 10.0 * *pn + ( c - '0' );
    if(c == '.')
        c = getch();
    for(power=1.0; isdigit(c); c = getch()){
        *pn = 10.0 * *pn + (c - '0');
        power *=10.0;
    }
    *pn *= sign / power;
    if(c != EOF)
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
    int n, getfloat(float*);
    float array[5]={0.0};
    for(n=0; n<5 && getfloat(&array[n])!=EOF; n++);// 必须将array[n]的地址传递给函数getfloat
                                                    // 否则函数getfloat将无法把转换得到的整数传回调用者
    int i;
    for(i=0; i<5; i++)
        printf("%f\n",array[i]);
    return 0;
}
~~~

### 5.3 指针与数组

~~~c
int a[10];
int *pa;
pa = &a[0]; // 将指针pa指向数组a的第0个元素 即pa的值是元素a[0]的地址
x = *pa; // 把数组a[0]中的内容复制给变量x
pa + i; // 数组元素a[i]的地址
*(pa + i); // 引用数组元素a[i]的内容
~~~

*注*

``pa = &a[0];``等价于``pa = a;``

对数组元素**a[i]的引用**也可写为``*(a + i)``

``&a[i]``与``a + i``的含义相同



当数组名传递给一个函数时，实际上传递的是该数组的第一个元素的地址

在被调用函数中，该参数是一个局部变量，因此数组名参数必须是一个指针

~~~c
// 另一个版本的strlen
int strlen(char *s){
    int n;
    for(n=0; *s != '\0'; s++)
    	n++;
    return n;
}
~~~



在函数定义时``char s[]; ``等价于``char *s;``

``f(&a[2])``等价于``f(a+2)``

``f(int arr[]){...}``等价于``f(int *a){...}``

### 5.4 地址算术符

存储分配程序

alloc(n)返回一个指向n个连续字符存储单元的指针，alloc函数的调用者可以利用该指针存储字符序列。

afree(p)释放已分配的存储空间，以便以后重用。

alloc与afree以栈（后进先出）的方式进行存储空间的管理

~~~c
#define ALLOCSIZE 10000 // 可用空间大小

static char allocbuf[ALLOCSIZE]; // alloc使用的存储区
static char *allocp = allocbuf; // 下一个空闲位置

char *alloc(int n){ // 返回指向n个字符的指针
    if(allocbuf + ALLOCSIZE - allocp >= n){ // 有足够的空间
        allocp += n;
        return allocp - n; // 分配前的指针
    }
    else
        return 0; 
}

void afree(char *p){ // 释放p指向的存储区
    if(p>=allcbuf && p<allocbuf+ALLOCSIZE)
        allocp = p;
}
~~~



如果指针p和q指向**同一组数组**的成员，那么他们之间可以进行类似==、！=、<、<=的关系比较运算，例如

~~~c
p < q; // 若为真 则表示p指向的元素的位置在q指向的元素的位置之前
~~~

*注*

指向**不同数组**的元素指针之间的算术或比较运算**没有定义**

特例：**指针的算术运算中可使用数组最后一个元素的下一个元素的地址**



如果q和p指向**相同数组**中的元素，且p<q，那么``q-p+1``表示**位于p和q指向的元素之间的元素的数目**

由此写另一个版本的strlen函数

~~~~c
int strlen(char *s){
    char *p = s;
    while(*p != '\0')
    	p++;
    return p-s;
}
~~~~

### 5.5 字符指针与函数

~~~c
char *pmessage;
pmessage = "now is the time";
~~~

把一个指向该字符数组的指针赋给pmessage，该过程并没有进行字符串的复制，**只是涉及指针的操作**

*C语言中没有提供将整个字符串作为一个整体进行处理的运算符*



~~~~c
char amessage[] = "now is the time"; // 定义一个数组
char *pmessage = "now is the time"; // 定义一个指针
~~~~

*注*

amessage是一个仅仅足以存放初始化字符串以及空字符'\0'的一组数组，**数组中单个字符可以修改，但amessage始终指向同一个存储位置**

pmessage是一个指针，其初值指向一个字符串常量，之后可以被修改指向其他地址，**但如果试图修改字符串的内容，结果没有定义**



将指针t指向的字符串复制到指针s指向的位置；使用数组下标实现的版本

这里把指针t指向的字符串复制到指针s指向的位置

~~~c
void strcpy(char *s, char *t){
    int i;
    i = 0;
    while((s[i] = t[i]) != '\0')
    	i++;
}
~~~

将指针t指向的字符串复制到指针s指向的位置；使用指针方式实现的版本1

~~~
void strcpy(char *s, char *t){
    while((*s = *t) != '\0'){
        s++;
        t++;
    }
}
~~~

*注* 

如果使用``s = t``**实质上只拷贝了指针，而并没有复制字符**

将指针t指向的字符串复制到指针s指向的位置；使用指针方式实现的版本2

~~~c
void strcpy(char *s, char *t){
    while((*s++ = *t++) != '\0')
        ;
}
~~~

``*t++``的值是执行自增运算符**之前**t所指向的字符

**后缀**运算符++表示在**在读取该字符后才改变t的值**

同样道理，在s执行自增运算之前，字符就被存储到指针s指向的旧位置，

**该字符同时也用来和空字符'\0'进行比较，以控制循环的执行**

最后的结果是一次将t指向的字符复制到s指向的位置，直到遇到结束符'\0'位置（同时也复制该结束符）



进一步精炼程序，表达式同‘\0’的比较是多余的，因为只要判断表达式的值是否为0

将指针t指向的字符串复制到指针s指向的位置；使用指针方式实现的版本3

~~~c
void strcpy(char *s, char *t){
    while((*s++ = *t++))
        ;
}
~~~

**标准库<string.h>中提供的函数strcpy把目标字符串作为函数值返回**



字符串比较函数strcmp(s,t)：比较字符串s和t并根据s按照字典序小于、等于、大于的结果分别返回负整数、0、正整数，返回值是s和t由前向后逐字符比较时遇到的第一个不相等字符处的字符的差值

~~~c
int strcmp(char *s, char *t){
    int i;
    for(i=0; s[i]==t[i]; i++)
    	if(s[i] == '\0')
    		return 0;
    return s[i] - t[i];
}
~~~

用指针方式实现

~~~c
int strcmp(char *s, char *t){
    for(; *s == *t; s++, t++)
    	if(*s == '\0')
    		return 0;
    return *s - *t;
}
~~~



***--p** : 在读取指针p指向的字符之前先对p执行自减运算



进栈出栈的标准用法

~~~c
*p++ = val; // 将val压入栈
val = *--p; // 将栈顶元素弹出到val中
~~~



#### 5-3 指针方式实现第二章的函数的strcat，函数strcat(s, t)将t指向的字符串复制到s指向的字符串的尾部

~~~c
# include<stdio.h>

void strcat(char *s, char *t){
    while(*s != '\0')
        s++;
    while(*s++ = *t++);
}

main(){
    char s[10] = "abc";
    char t[10] = "123";
    strcat(s,t);
    printf("%s", s);
}

~~~

#### 5-4 编写strend(s, t) 如果字符串t出现在字符串s的尾部，该函数返回1；否则返回0

~~~c
# include<stdio.h>
// 自己写的版本
int strend(char *s, char *t){
    int i=0, j=0;
    char *tp = t;
    while(*s != '\0'){
        s++;
        i++;
    }
    while(*t != '\0'){
        t++;
        j++;
    }
    if(i >= j){
        while(*s == *t && t!= tp){
            //printf("%c %c\n", *s, *t);
            s--;
            t--;
        }
        if(t != tp)
            return 0;
        else
            return 1;
    }
    else
        return 0;
}

main(){
    char s1[10] = "abc123";
    char s2[10] = "23";
    char t1[10] = "123";
    char t2[10] = "abc";
    printf("s1t1 %d\n", strend(s1,t1));
    printf("s1t2 %d\n", strend(s1,t2));
    printf("s2t1 %d\n", strend(s2,t1));
}
~~~

~~~c
// 教材
# include<stdio.h>

int strend(char *s, char *t){
    char *bs = s;
    char *bt = t;

    for(; *s; s++);
    for(; *t; t++);
    for(; *s == *t; s--, t--){
        if(t == bt || s == bs)
            break;
    }
    if(*s == *t && t == bt && *s!='\0')
        return 1;
    else
        return 0;
}

main(){
    char s1[10] = "abc123";
    char s2[10] = "23";
    char t1[10] = "123";
    char t2[10] = "abc";
    printf("s1t1 %d\n", strend(s1,t1));
    printf("s1t2 %d\n", strend(s1,t2));
    printf("s2t1 %d\n", strend(s2,t1));
}
~~~

#### 5-5 实现库函数strncpy、strncat和strncmp，他们最多对参数字符串中的前n个字符尽心操作。例如strncpy(s, t, n)将t中最多前n个字符复制到s中

~~~c
// 自己写的版本
#include <stdio.h>
// 实现库函数strncpy、strncat和strncmp，他们最多对参数字符串中的前n个字符尽心操作。
// 例如strncpy(s, t, n)将t中最多前n个字符复制到s中
void strncpy(char *s, char *t, int n){
    while(n--){
        (*s++) = (*t++);
    }
    *s = '\0';
}

void strncat(char *s, char *t, int n){
    while(*s++ != '\0');
    *s--;
    while(n--){
        *s++ = *t++;
    }
    *s = '\0';
}

int strncmp(char *s, char *t, int n){ // 错误 未处理s长度<n的情况
    for(; n && *s == *t; s++, t++, n--);
    if(n == 0)
        return 0;
    return *s - *t;
}

int main(){
    char s[10] = "123456";
    char t[10] = "12375";
    //strncpy(s, t, 3);
    //strncat(s,t,6);
    //printf("%s",s);
    printf("%d", strncmp(s,t,5));
    return 0;
}
~~~

~~~c
// 教材
#include <stdio.h>
// 实现库函数strncpy、strncat和strncmp，他们最多对参数字符串中的前n个字符尽心操作。
// 例如strncpy(s, t, n)将t中最多前n个字符复制到s中
void strncpy(char *s, char *t, int n){
    while(*t && n--){
        (*s++) = (*t++);
    }// 若t中内容短于n会将t末尾的'\0'复制过去
    while(n-- > 0) // 若t中的数量<n
        *s = '\0';
}

void strncat(char *s, char *t, int n){
    int strlen(char*);
    strncpy(s+strlen(s), t, n);
}

int strncmp(char *s, char *t, int n){
    for(; *s == *t; s++, t++, n--)
        if(--n <= 0 || *s == '\0')
            return 0;
    return *s - *t;
}

int main(){
    char s[10] = "123";
    char t[10] = "12375";
    //strncpy(s, t, 3);
    //strncat(s,t,6);
    //printf("%s",s);
    printf("%d", strncmp(s,t,4));
    return 0;
}
~~~

#### 5-6 采用指针而非数组索引方式改写getline、atoi、itoa以及他们的变体形式，reverse、strindex、getop等

### 5.6 指针数组以及指向指针的指针

对文本进行排序

1. 读取所有输入行
2. 对文本进行排序
3. 按次序打印文本

~~~c
#include <stdio.h>
#include <string.h>

#define MAXLINES 5000 // 进行排序的最大文本行数

char *lineptr[MAXLINES]; // 指向文本行的指针数组

int readlines(char *lineptr[], int nlines);
void writelines(char *lineptr[], int nlines);

void qsort_(char *lineptr[], int left, int right);

// 对输入文本进行排序
main(){
    int nlines; // 读取输入行数目

    if((nlines = readlines(lineptr, MAXLINES)) >= 0){
        qsort_(lineptr, 0, nlines-1);
        writelines(lineptr, nlines);
        return 0;
    }
    else{
        printf("error: input too big to sort\n");
        return 1;
    }
}

#define MAXLEN 1000 // 每个输入行的最大长度
int getline(char *, int);
char *alloc(int);

#include <malloc.h>
char *alloc(int n){
    return (char*)malloc(n*sizeof(char));
}

int getline(char *s, int max){
    char c;
    char *t = s;
    while(--max>0 && (c = getchar())!=EOF && c!='\n')
        *s++ = c;
    if(c == '\n')
        *s++ = c;
    *s = '\0';
    return s - t;
}

// readlines: 读取输入行
int readlines(char *lineptr[], int maxlines){
    int len, nlines;
    char *p, line[MAXLEN];
    nlines = 0;
    while((len = getline(line, MAXLEN)) > 0){ // 获取输入行的输入字符个数
        if(nlines >= maxlines || (p = alloc(len)) == NULL)
            return -1;
        else{
            line[len-1] = '\0'; // 删除换行符
            strcpy(p, line);
            lineptr[nlines++] = p;
        }
    }
    return nlines;
}

// writelines：写输入行
/*void writelines(char *lineptr[], int nlines){
    int i;

    for(i=0; i<nlines; i++){
        printf("%s\n", lineptr[i]);
    }
}*/
// 由于lineptr本身是个数组名，因此可将writelines写为
void writelines(char *lineptr[], int nlines){
    while(nlines--){
        printf("%s\n",*lineptr++);
    }
}

// qsort: 按递增顺序对v[left]...v[right]进行排序
void qsort_(char*v[], int left, int right){

    int i, last;
    void swap(char *v[], int i, int j);

    if(left >= right)
        return;

    swap(v, left, (left + right)/2); // 将一开始中间位置的数作为枢轴并将它放到首位，即与第一个位置的数交换
    last = left;

    for(i=left+1; i<=right; i++){
        if(strcmp(v[i], v[left]) < 0) // 将接下来的每个数与首位枢轴的数（即v[left]）相比较
            swap(v, ++last, i);       // last记录最近（最右）一个满足小于数轴数的位置
    }                                 // 下一位i如果小于数轴，则将i和last+1位置的数交换，并让last往后移一位，保证last指向的特性

    swap(v, left, last);    // 将枢轴的数和最右一个比枢轴小的数交换位置
                            // 现在last位置上的数为这一轮的枢轴数，完成last右边都小于它，左边都大于它
    qsort_(v, left, last-1); // 递归划分
    qsort_(v, last+1, right);
}

// swap: 交换v[i]和v[j]
void swap(char *v[], int i, int j){
    char *temp;
    temp = v[i];
    v[i] = v[j];
    v[j] = temp;
}

~~~

#### 5-7 *重写readlines，将输入的文本行存储到由main函数提供的一个数组中，而不是存储到调用的alloc分配的存储空间。该函数的运行速度比改写前快多少？

比原始版本快一点点

~~~c
#include <stdio.h>
#include <string.h>

#define MAXLINES 5000 // 进行排序的最大文本行数
#define MAXSTOR 5000

char *lineptr[MAXLINES]; // 指向文本行的指针数组

int readlines(char *lineptr[], int nlines, char *temp);
void writelines(char *lineptr[], int nlines);

void qsort_(char *lineptr[], int left, int right);

// 对输入文本进行排序
main(){
    int nlines; // 读取输入行数目
    char temp[MAXSTOR];

    if((nlines = readlines(lineptr, MAXLINES,temp)) >= 0){
        qsort_(lineptr, 0, nlines-1);
        writelines(lineptr, nlines);
        return 0;
    }
    else{
        printf("error: input too big to sort\n");
        return 1;
    }
}

#define MAXLEN 1000 // 每个输入行的最大长度
int getline(char *, int);
char *alloc(int);

#include <malloc.h>
char *alloc(int n){
    return (char*)malloc(n*sizeof(char));
}

int getline(char *s, int max){
    char c;
    char *t = s;
    while(--max>0 && (c = getchar())!=EOF && c!='\n')
        *s++ = c;
    if(c == '\n')
        *s++ = c;
    *s = '\0';
    return s - t;
}

// readlines: 读取输入行
int readlines(char *lineptr[], int maxlines, char *temp){
    int len, nlines;
    char line[MAXLEN];
    char *p = temp;
    char *linestop = temp + MAXSTOR;

    nlines = 0;
    while((len = getline(line, MAXLEN)) > 0){ // 获取输入行的输入字符个数
        if(nlines >= maxlines || p+len > linestop)
            return -1;
        else{
            line[len-1] = '\0'; // 删除换行符
            strcpy(p, line);
            lineptr[nlines++] = p;
            p += len;
        }
    }
    return nlines;
}

// writelines：写输入行
/*void writelines(char *lineptr[], int nlines){
    int i;

    for(i=0; i<nlines; i++){
        printf("%s\n", lineptr[i]);
    }
}*/
// 由于lineptr本身是个数组名，因此可将writelines写为
void writelines(char *lineptr[], int nlines){
    while(nlines--){
        printf("%s\n",*lineptr++);
    }
}

// qsort: 按递增顺序对v[left]...v[right]进行排序
void qsort_(char*v[], int left, int right){

    int i, last;
    void swap(char *v[], int i, int j);

    if(left >= right)
        return;

    swap(v, left, (left + right)/2); // 将一开始中间位置的数作为枢轴并将它放到首位，即与第一个位置的数交换
    last = left;

    for(i=left+1; i<=right; i++){
        if(strcmp(v[i], v[left]) < 0) // 将接下来的每个数与首位枢轴的数（即v[left]）相比较
            swap(v, ++last, i);       // last记录最近（最右）一个满足小于数轴数的位置
    }                                 // 下一位i如果小于数轴，则将i和last+1位置的数交换，并让last往后移一位，保证last指向的特性

    swap(v, left, last);    // 将枢轴的数和最右一个比枢轴小的数交换位置
                            // 现在last位置上的数为这一轮的枢轴数，完成last右边都小于它，左边都大于它
    qsort_(v, left, last-1); // 递归划分
    qsort_(v, last+1, right);
}

// swap: 交换v[i]和v[j]
void swap(char *v[], int i, int j){
    char *temp;
    temp = v[i];
    v[i] = v[j];
    v[j] = temp;
}
~~~

