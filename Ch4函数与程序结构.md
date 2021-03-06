# Ch4 函数与程序结构

###4.1函数的基本知识

将输入中包含特定“模式”或字符串的各行打印出来

~~~c
# include <stdio.h>
# define MAXLINE 1000

int getline(char line[], int max);
int strindex(char source[], char searchfor[]);

char pattern[] = "ould"; // 待查的模式

// 找出所有与模式匹配的行
main(){
    char line[MAXLINE];
    int found = 0;

    while(getline(line, MAXLINE) > 0){
        if(strindex(line, pattern) >= 0){
            printf("%s", line);
            found++;
        }
    }
    return found; 
}

// 将行保存在s中并返回改行的长度
int getline(char s[], int lim){
    int c, i;
    i = 0;
    while(--lim>0 && (c=getchar())!=EOF && c!='\n')
        s[i++] = c;
    if(c == '\n')
        s[i++] = c;
    s[i] = '\0';
    return i;
}

// 返回t在s中的位置若未找到返回-1
int strindex(char s[], char t[]){
    int i, j, k;
    for(i=0; s[i]!='\0'; i++){
        for(j=i, k=0; t[k]!='\0' && s[j]==t[k]; j++, k++);
        if(k > 0 && t[k] == '\0')
            return i;
    }
    return -1;
}
~~~

*注*

该主函数中main返回了一个状态，即匹配的数目，该返回值可以在调用该程序的环境中使用

#### 4-1 编写strindex(s,t)返回字符串t在s中最右边出现的位置，如s中不含t则返回-1

~~~c
# include <stdio.h>
# define MAXLINE 1000

int getline(char line[], int max);
int strindex(char source[], char searchfor[]);

char pattern[] = "as"; // 待查的模式

// 找出所有与模式匹配的行
main(){
    char line[MAXLINE];
    int found = 0;

    while(getline(line, MAXLINE) > 0){
        printf("%d\n", strindex(line, pattern));
    }
}

// 将行保存在s中并返回改行的长度
int getline(char s[], int lim){
    int c, i;
    i = 0;
    while(--lim>0 && (c=getchar())!=EOF && c!='\n')
        s[i++] = c;
    if(c == '\n')
        s[i++] = c;
    s[i] = '\0';
    return i;
}

// 自己写的版本
int strindex(char s[], char t[]){
    int i, j, k, start;
    start = -1;
    for(i=0; s[i]!='\0'; i++){
        for(j=i, k=0; s[j]==t[k]; j++, k++){
            if(t[k+1] == '\0')
                start = i+1;
        }
    }
    return start;
}

// 教材
int strindex(char s[], char t[]){
    int i, j, k, start;
    start = -1;
    for(i=0; s[i]!='\0'; i++){
        for(j=i, k=0; t[k]!='\0' && s[j]==t[k]; j++, k++);
        if(k>0 && t[k] == '\0')
            start = i;
    }
    return start;
}

// 教材 效率更高
int strindex(char s[], char t[]){
    int i, j, k;
    
    for(i=strlen(s)-strlen(t); i>=0; i--){
        for(j=i, k=0; t[k] != '\0' && s[i]==t[k]; j++, k++);
        if(k>0 && t[k]!='\0')
            return i;
    }
    return -1;
}
~~~

### 4.2返回非整型的函数

把字符串s转换为相应的双精度浮点数

~~~
# include <stdio.h>
# include <ctype.h>
// 把字符串s转换为相应的双精度浮点数
double atof(char s[]){
    double val, power;
    int i, sign;

    for(i=0; isspace(s[i]); i++);
    sign = (s[i] == '-') ? -1 : 1;
    if(s[i] == '+' || s[i] == '-')
        i++;
    for(val = 0.0; isdigit(s[i]); i++)
        val = 10.0 * val + (s[i] - '0');
    if(s[i] == '.')
        i++;
    for(power = 1.0; isdigit(s[i]); i++){
        val = 10.0 * val + (s[i] - '0');
        power *= 10.0;
    }
    return sign * val / power;
}
~~~

*注意*

在字符型转整型浮点型时``s[i] - '0'``

在整型浮点型转字符型时``s[i] + '0'``

***

在每行中读取一个数（数的前面可能有正负号）并对他们求和，在每次输入完成后把这些数的累计总和打印

~~~c
main(){
    double sum, atof(char[]);
    char line[MAXLINE];
    int getline(char line[], int max);

    sum = 0;
    while((getline(line, MAXLINE)) > 0)
        printf("\t%g\n", sum += atof(line));
    return 0;
}
~~~

*注*

%g用来输出实数，它根据数值的大小，自动选f格式或e格式（选择输出时占宽度较小的一种），且不输出无意义的0。即%g是根据结果自动选择科学记数法还是一般的小数记数法

printf("%g\n", 0.00001234);
printf("%g\n", 0.0001234);
printf("%.2g\n", 123.45);
printf("%.2g\n", 23.45);

上面四句输出结果为:
1.234e-05
0.0001234
1.2e+02
23
对于指数小于-4或者大于给定精度的数值,按照%e的控制输出,否则按照%f的控制输出

%g至多保留6位有效数字

#### *4-2 对atof函数进行扩充其可以处理123.45e-6的科学表示法，其中，浮点数后面可能会紧跟一个e或E以及一个指数（可能有正负号）

~~~c
# include <stdio.h>
# include <ctype.h>
# define MAXLINE 100
// 自己写的版本
double atof(char s[]){
    int i, sign1, sign2;
    double power;
    double val = 0.0;
    int val2 = 0;
    for(i=0; isspace(s[i]); i++);
    sign1 = (s[i] == '-') ? -1 : 1;
    if(s[i] == '+' || s[i] == '-')
        i++;
    for(; isdigit(s[i]); i++){
        val = 10.0 * val + (s[i] - '0');
    }
    if(s[i] == '.')
        i++;
    for(power=1.0; isdigit(s[i]); i++){
        val = 10.0 * val + (s[i] - '0');
        power *= 10.0;
    }

    if(s[i] == 'e' || s[i] == 'E')
        i++;
    sign2 = (s[i] == '-') ? -1 : 1;
    if(s[i] == '+' || s[i] == '-')
        i++;
    for(; isdigit(s[i]); i++)
        val2 = 10 * val2 + (s[i] - '0');
    if(sign2 == 1)
        while(val2){
            power /= 10.0;
            val2--;
        }
    else if(sign2 == -1)
        while(val2){
            power *= 10.0;
            val2--;
        }
    return sign1 * val / power;
}

// 教材
double atof2(char s[]){
    double val, power;
    int exp, i, sign;

    for(i=0; isspace(s[i]); i++);
    sign = (s[i] == '-') ? -1 : 1;
    if(s[i] == '+' || s[i] == '-')
        i++;
    for(val=0.0; isdigit(s[i]); i++)
        val = 10.0 * val + (s[i] - '0');
    if(s[i] == '.')
        i++;
    for(power=1.0; isdigit(s[i]); i++){
        val = 10.0 * val + (s[i] - '0');
        power *= 10.0;
    }
    val = sign * val / power;

    if(s[i] == 'e' || s[i] == 'E'){
        sign = (s[++i] == '-') ? -1 : 1;
        if(s[i] == '-' || s[i] == '+')
            i++;
        for(exp=0; isdigit(s[i]); i++)
            exp = 10 * exp + (s[i] - '0');
        if(sign == 1)
            while(exp-->0)
                val *= 10;
        else
            while(exp-->0)
                val /= 10;
    }
    return val;
}

main(){
    double atof(char[]),atof2(char[]);
    char line[MAXLINE] = "-123.45e10";
    printf("\t%lf\n", atof(line));
    printf("\t%lf\n", atof2(line));
    return 0;
}
~~~

*注*

![1532528434571](Ch4函数与程序结构.assets/1532528434571.png)

自己写的版本出现丢失精度的情况，是因为用**power/10的val2次方**时会出现小数，无法用二进制精确的表示出来，在大多数机器上，**0.1的二进制表示法都要比0.1稍微小一点**，用**10.0*0.1并不能精确地得到1.0**

因此**连续地除以10**比**连续地乘以0.1**更精确

因此更提倡教材的版本，即将指数部分单独出来进行计算

### 4.3外部变量

逆波兰式计算器

~~~c
# include <stdio.h>
# include <stdlib.h> // 为使用atof()函数

# define MAXOP 100 // 操作数或运算符的最大长度
# define NUMBER '0' // 标识找到一个数

int getop(char []);
void push(double);
double pop(void);

// 逆波兰式计算器
main(){
    int type;
    double op2;
    char s[MAXOP];

    while((type = getop(s)) != EOF){
        switch(type){
        case NUMBER:
            push(atof(s));
            break;
        case '+':
            push(pop() + pop());
            break;
        case '*':
            push(pop() * pop());
            break;
        case '-':
            op2 = pop();
            push(pop() - op2);
            break;
        case '/':
            op2 = pop();
            if(op2 != 0.0)
                push(pop() / op2);
            else
                printf("error: zero divisor\n");
            break;
        case '\n':
            printf("\t%.8g\n", pop());
            break;
        default:
            printf("error: unknown command %s\n", s);
            break;
        }
    }
    return 0;
}

/***************************************************************************/

# define MAXVAL 100 // 栈val的最大深度

int sp = 0; // 下一个空闲栈位置
double val[MAXVAL]; // 值栈

// 把f压入值栈中
void push(double f){
    if(sp < MAXVAL)
        val[sp++] = f;
    else
        printf("error: stack full, can't push %g\n",f);
}

// 弹出并返回栈顶的值
double pop(){
    if(sp > 0)
        return val[--sp];
    else{
        printf("error: stack empty\n");
        return 0.0;
    }
}

/***************************************************************************/
# include <ctype.h>
int getch(void); // 读入下一个待处理的字符
void ungetch(int); // 把字符放回到输入中

// 获取下一个运算符或数值操作数
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

/***************************************************************************/
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
~~~

*注*

1. main函数不需要了解控制栈的变量信息，只进行压入与弹出操作，因此可以把栈以及相关信息放入外部变量中，并只供push和pop函数访问，而不被main函数访问
2. getch和ungetch函数用于超前多读入一些输入用来判断已经读入的输入是否足够
3. 如输入**12.3**时第一次满足`` while((s[0] = c = getch()) == ' ' || c == '\t')``中的不等式时跳出就**已将第一个不等于‘ ’或‘\t’的值付给了s[0]** 
4. ``s[++i] = c = getch()``的执行顺序
   1. i++
   2. c = getch()
   3. s[i] = c

### &&求模运算和求余运算区别

其实取模和取余在目标上是一致的，但是因为语言对取余和取模上定义的不同，导致得到的结果不同。

对取余和取模定义不同的语言中，两者的不同点只有一个

**取余运算在计算商值向0方向舍弃小数位**

**取模运算在计算商值向负无穷方向舍弃小数位**（尽可能让商小的原则**）

从上面的区别可以总结出，取余（rem）和取模（mod）在被除数、除数同号时，结果是等同的，**异号时会有区别，所以要特别注意异号的情况**。

下面来看一些例子

1. 取模

   7 mod 4 = 3（商 = 1 或 2，1<2，取商=1）

   -7 mod 4 = 1（商 = -1 或 -2，-2<-1，取商=-2）

   7 mod -4 = -1（商 = -1或-2，-2<-1，取商=-2）

   -7 mod -4 = -3（商 = 1或2，1<2，取商=1）

2. 取余

   7 rem 4 = 3 

   -7 rem 4 = -3

另外各个环境下%运算符的含义不同，比如c/c++，java 为取余，而python则为取模。

#### *4-3 在计算器框架程序中加入取模（%）运算，并注意考虑负数的情况

~~~c
# include <stdio.h>
# include <stdlib.h> // 为使用atof()函数
# include <math.h>

# define MAXOP 100 // 操作数或运算符的最大长度
# define NUMBER '0' // 标识找到一个数

int getop(char []);
void push(double);
double pop(void);

// 逆波兰式计算器
main(){
    int type;
    double op2;
    char s[MAXOP];

    while((type = getop(s)) != EOF){
        //printf("%s\n",s);
        switch(type){
        case NUMBER:
            push(atof(s));
            break;
        case '+':
            push(pop() + pop());
            break;
        case '*':
            push(pop() * pop());
            break;
        case '-':
            op2 = pop();
            push(pop() - op2);
            break;
        case '/':
            op2 = pop();
            if(op2 != 0.0)
                push(pop() / op2);
            else
                printf("error: zero divisor\n");
            break;
        case '%':
            op2 = pop();
            if(op2 != 0.0)
                push((((int)pop() % (int)op2) + (int)op2) % (int)op2);
            break;
        case '\n':
            printf("\t%.8g\n", pop());
            break;
        default:
            printf("error: unknown command %s\n", s);
            break;
        }
    }
    return 0;
}

/***************************************************************************/

# define MAXVAL 100 // 栈val的最大深度

int sp = 0; // 下一个空闲栈位置
double val[MAXVAL]; // 值栈

// 把f压入值栈中
void push(double f){
    if(sp < MAXVAL)
        val[sp++] = f;
    else
        printf("error: stack full, can't push %g\n",f);
}

// 弹出并返回栈顶的值
double pop(){
    if(sp > 0)
        return val[--sp];
    else{
        printf("error: stack empty\n");
        return 0.0;
    }
}

/***************************************************************************/
# include <ctype.h>
int getch(void); // 读入下一个待处理的字符
void ungetch(int); // 把字符放回到输入中

// 获取下一个运算符或数值操作数
int getop(char s[]){
    int i, c;
    while((s[0] = c = getch()) == ' ' || c == '\t');

    s[1] = '\0';
    if(!isdigit(c) && c != '.' && c != '-')
        return c; // 不是数

    i = 0;
    if(c == '-'){ // 记录负数
        if(isdigit(c = getch()) || c == '.')
            s[++i] = c;
        else{  // 注意只有‘-’出现的情况
            if(c != EOF)
                ungetch(c);
            return '-';
        }
    }

    if(isdigit(c)) // 收集整数部分
        while(isdigit(s[++i] = c = getch()));
    if(c == '.') // 收集小数部分
        while(isdigit(s[++i] = c = getch()));
    s[i] = '\0';
    if(c != EOF)
        ungetch(c);
    return NUMBER;
}
/***************************************************************************/
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
~~~

*注*

在识别**- 1**时不为负数 只有在**-1** 符号**-**后面紧跟的字符为数时 才为负数

为保证取模运算正确 可用**``((x % n) + n ) % n``**

#### 4-4 在栈操作中添加几个命令，分别用于在不弹出元素的情况下打印栈顶元素；复制栈顶元素；交换栈顶两个元素的值。另外增加一个命令清空栈。

####*4-5 给计算器增加处理sin、exp、pow等库函数的操作

~~~c
# include <stdio.h>
# include <stdlib.h> // 为使用atof()函数
# include <math.h>
# include <string.h>

# define MAXOP 100 // 操作数或运算符的最大长度
# define NUMBER '0' // 标识找到一个数
# define MATHCOM 'n'

int getop(char []);
void push(double);
double pop(void);
void cleanStack(void);

// 特殊计算命令
void mathfun(char s[]){
    double op2;
    if(strcmp(s, "sin") == 0) // strcmp函数比较两个字符串是否相等 相等返回0
        push(sin(pop()));
    else if(strcmp(s, "cos") == 0)
        push(cos(pop()));
    else if(strcmp(s, "exp") == 0)
        push(exp(pop()));
    else if(strcmp(s, "pow") == 0){
        op2 = pop();
        push(pow(pop(), op2));
    }
    else
        printf("error: %s not supported\n",s);
}

// 逆波兰式计算器
main(){
    int type;
    double op2, op1;
    char s[MAXOP];

    while((type = getop(s)) != EOF){
        //printf("%s\n",s);
        switch(type){
        case NUMBER:
            push(atof(s));
            break;
        case MATHCOM:
            mathfun(s);
            break;
        case '+':
            push(pop() + pop());
            break;
        case '*':
            push(pop() * pop());
            break;
        case '-':
            op2 = pop();
            push(pop() - op2);
            break;
        case '/':
            op2 = pop();
            if(op2 != 0.0)
                push(pop() / op2);
            else
                printf("error: zero divisor\n");
            break;
        case '%':
            op2 = pop();
            if(op2 != 0.0)
                push((((int)pop() % (int)op2) + (int)op2) % (int)op2);
            break;
        case '?': // 打印栈顶元素
            op2 = pop();
            printf("\t%.8g\n", op2);
            push(op2);
            break;
        case 'd': // 复制栈顶元素
            op2 = pop();
            push(op2);
            push(op2);
            break;
        case 's': // 交换栈顶元素
            op1 = pop();
            op2 = pop();
            push(op1);
            push(op2);
            break;
        case 'c': // 清空栈
            cleanStack();
            break;
        case '\n':
            printf("\t%.8g\n", pop());
            break;
        default:
            printf("error: unknown command %s\n", s);
            break;
        }
    }
    return 0;
}

/***************************************************************************/

# define MAXVAL 100 // 栈val的最大深度

int sp = 0; // 下一个空闲栈位置
double val[MAXVAL]; // 值栈

// 把f压入值栈中
void push(double f){
    if(sp < MAXVAL)
        val[sp++] = f;
    else
        printf("error: stack full, can't push %g\n",f);
}

// 弹出并返回栈顶的值
double pop(){
    if(sp > 0)
        return val[--sp];
    else{
        printf("error: stack empty\n");
        return 0.0;
    }
}

// 清空栈
void cleanStack(){
    sp = 0;
}
/***************************************************************************/
# include <ctype.h>
int getch(void); // 读入下一个待处理的字符
void ungetch(int); // 把字符放回到输入中

// 获取下一个运算符或数值操作数
int getop(char s[]){
    int i, c;
    while((s[0] = c = getch()) == ' ' || c == '\t');
    s[1] = '\0';

    i = 0;
    if(islower(c)){
        while(islower((s[++i] = c = getch())));
        s[i] = '\0';
        if(c != EOF){ // 将多读入的字符重新压回缓冲区
            ungetch(c);
        }
        if(strlen(s) > 1)
            return MATHCOM;
        else
            return c; // 不是命令
    }

    if(!isdigit(c) && c != '.' && c != '-')
        return c; // 不是数

    i = 0;
    if(c == '-'){ // 记录负数
        if(isdigit(c = getch()) || c == '.')
            s[++i] = c;
        else{  // 注意只有‘-’出现的情况
            if(c != EOF)
                ungetch(c);
            return '-';
        }
    }

    if(isdigit(c)) // 收集整数部分
        while(isdigit(s[++i] = c = getch()));
    if(c == '.') // 收集小数部分
        while(isdigit(s[++i] = c = getch()));
    s[i] = '\0';
    if(c != EOF)
        ungetch(c);
    return NUMBER;
}
/***************************************************************************/
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
~~~

*注*

1. strcmp函数比较两个字符串是否相等 相等返回0
2. 改进的getop函数只能识别出一个由小写字母组成的字符串并把它返回为类型MATHCOM进一步有主程序调用manthfun进行操作

#### *4.6 给计算器程序增加处理变量的命令（提供26个具有单个英文字母变量名（大写字母）的变量很容易）增加一个变量（v）存放最近的打印值

结果样例

~~~
3 A =
// 把3赋给A
2 A +
// 计算2+3(被赋予给变量A的值)=5 并将5赋给v
v 1 +
// 结果为5+1=6
~~~

用大写字母A-Z来代表变量，这些字母作为数组变量的索引，增加一个小写的字母变量v存放打印的值

在遇到一个变量名（A~Z或v）时，计算器把该变量的值压栈

增加一个新的操作符“=”，作用：把栈中的某个元素赋值给这个“=”前面的字母

~~~c
# include <stdio.h>
# include <stdlib.h> // 为使用atof()函数
# include <math.h>

# define MAXOP 100 // 操作数或运算符的最大长度
# define NUMBER '0' // 标识找到一个数

int getop(char []);
void push(double);
double pop(void);
# define MAXVAL 100 // 栈val的最大深度
double val[MAXVAL]; // 值栈
// 逆波兰式计算器
main(){
    int type;
    int letter; // 存放变量为哪个字母
    double var[26] = {0.0}; // 对应存放变量值
    double op2, result;
    char s[MAXOP];

    while((type = getop(s)) != EOF){
        //printf("%s\n",s);
        switch(type){
        case NUMBER:
            push(atof(s));
            break;
        case '+':
            push(pop() + pop());
            break;
        case '*':
            push(pop() * pop());
            break;
        case '-':
            op2 = pop();
            push(pop() - op2);
            break;
        case '/':
            op2 = pop();
            if(op2 != 0.0)
                push(pop() / op2);
            else
                printf("error: zero divisor\n");
            break;
        case '%':
            op2 = pop();
            if(op2 != 0.0)
                push((((int)pop() % (int)op2) + (int)op2) % (int)op2);
            break;
        case '=': // 这个=只能用于给变量赋值操作
            pop(); // 把多余压入的字母变量pop
            if(letter >= 'A' && letter <= 'Z'){ // letter是上一个输入的字母变量 不能用type替代
                var[letter - 'A'] = pop();
                push(var[letter - 'A']);
                result = var[letter - 'A'];
            }
            else
                printf("error: no variable name\n");
            break;
        case '\n':
            result = pop();
            printf("\t%.8g\n", result);
            break;
        default:
            if(type >= 'A' && type <= 'Z')
                push(var[type - 'A']);
            else if(type == 'v')
                push(result);
            else
                printf("error: unknown command %s\n", s);
            break;
        }
        letter = type; // 记录字母
    }
    return 0;
}

/***************************************************************************/

//# define MAXVAL 100 // 栈val的最大深度

int sp = 0; // 下一个空闲栈位置
//double val[MAXVAL]; // 值栈

// 把f压入值栈中
void push(double f){
    if(sp < MAXVAL)
        val[sp++] = f;
    else
        printf("error: stack full, can't push %g\n",f);
}

// 弹出并返回栈顶的值
double pop(){
    if(sp > 0)
        return val[--sp];
    else{
        printf("error: stack empty\n");
        return 0.0;
    }
}

/***************************************************************************/
# include <ctype.h>
int getch(void); // 读入下一个待处理的字符
void ungetch(int); // 把字符放回到输入中

// 获取下一个运算符或数值操作数
int getop(char s[]){
    int i, c;
    while((s[0] = c = getch()) == ' ' || c == '\t');

    s[1] = '\0';
    if(!isdigit(c) && c != '.' && c != '-')
        return c; // 不是数

    i = 0;
    if(c == '-'){ // 记录负数
        if(isdigit(c = getch()) || c == '.')
            s[++i] = c;
        else{  // 注意只有‘-’出现的情况
            if(c != EOF)
                ungetch(c);
            return '-';
        }
    }

    if(isdigit(c)) // 收集整数部分
        while(isdigit(s[++i] = c = getch()));
    if(c == '.') // 收集小数部分
        while(isdigit(s[++i] = c = getch()));
    s[i] = '\0';
    if(c != EOF)
        ungetch(c);
    return NUMBER;
}
/***************************************************************************/
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
~~~

#### *4.7 编写ungets(s)，将整个字符串s压回到输入中。ungets函数需要使用buf和bufp吗？它能否仅使用ungetch函数？

若len为字符串s的字符个数，ungets将调用ungetch len次，每次都会把字符串s中的一个字符重新压回输入

ungets函数将按**逆序**把字符串重新压回输入

因为ungetch有对buf和bufp进行操作判断，所以ungets不需要直接操作

~~~c
// 把字符串s压回输入中
void ungets(char s[]){
    int len, i;
    len = strlen(s);
    while(len>0)
        ungetch(s[--len]);
}
~~~

#### *4.8 假定最多只压回一个字符，请相应地修改getch与ungetch这两个函数

输入缓存区buf不再是一个数组，因为任意时刻缓冲区最多保存一个字符

~~~c
char buf = 0; // 缓存

// 取一个字符（可能是压回的字符）
int getch(void){
    int c;
    if(buf != 0)
        c = getchar();
    else
        c = buf;
    buf = 0;
    return c;
}

// 把字符压回输入中
void ungetch(int c){
    if(buf != 0)
        printf("ungetch: too many characters\n");
    else
        buf = c;
}
~~~

*注*

buf的初值是0，而每次getch函数在读入一个字符后，会再次将buf置为0

#### *4.9 教材介绍的getch和ungetch函数不能正确处理压回的EOF，考虑压回EOF时应该如何处理

原先版本

~~~c
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
~~~

C语言中不要求char变量是signed还是unsigned类型，**当把一个char类型转换成int类型时，转换结果不应该是一个负值**

不同机器进行转换时可能得到不同结果

负数（-1）        ->        字符        ->        整数

0xFFFF                          0xFF                   0x00FF（255）

0xFFFF                          0xFF                   0xFFFF（-1）

因此如果想像其他字符那样对待**EOF(-1)**，就应该把输入缓冲区buf声明为一个整数数组

即将``char buf[BUFSIZE]; // 用于ungetch()函数的缓冲区``变为``int buf[BUFSIZE];``

这样就不需要上面提到的转换而能正确处理负数

### 4.4 作用域规则

~~~c
extern int sp;
extern double val[];
~~~

只为源文件的其余部分**声明**了一个int类型的外部变量sp和一个double数组类型的外部变量val（**该数组长度在其他地方确定**） **但这个两个声明并没有建立变量或者为他们分配储存单元**

*注*

**外部变量**的**定义**中必须指定数组的长度 但 **extern声明**不一定要指出数组的长度

### 4.6 静态变量

通过**static**限定外部变量与函数，可以将其声明后的对象的**作用域限定为被编译文件的剩余部分**

通过static限定外部对象可以达到隐藏外部对象的目的

###&&[浅谈static和extern关系](https://www.cnblogs.com/zhaodun/p/6432615.html) 

#####C语言中的static关键字

在C语言中，static可以用来修饰局部变量，全局变量以及函数。在不同的情况下static的作用不尽相同。

1. 修饰局部变量  

   一般情况下，对于局部变量是存放在栈区的，并且局部变量的生命周期在该语句块执行结束时便结束了。但是如果用static进行修饰的话，该变量便存放在静态数据区，其生命周期一直持续到整个程序执行结束。但是在这里要注意的是，虽然**用static对局部变量进行修饰过后，其生命周期以及存储空间发生了变化，但是其作用域并没有改变，其仍然是一个局部变量，作用域仅限于该语句块。**

   在用static修饰局部变量后，该变量只在初次运行时进行初始化工作，且只进行一次。

   ~~~c
   #include<stdio.h>
   void fun(){
   	static int a=1;
   	a++;
   	printf("%d\n",a);
   }
   int main(void){
   	fun();
   	fun();
   	return 0;
   }
   ~~~

   程序执行结果为: 

   ![1537343809824](Ch4函数与程序结构.assets/1537343809824.png)

   说明在第二次调用fun()函数时，a的值为2，并且没有进行初始化赋值，直接进行自增运算，所以得到的结果为3.

   对于静态局部变量如果没有进行初始化的话，对于整形变量系统会自动对其赋值为0，对于字符数组，会自动赋值为'\0'.

2. 修饰全局变量

   对于一个全局变量，它既可以在本源文件中被访问到，也可以在同一个工程的其它源文件中被访问(只需**用extern进行声明**即可)如:

   有file1.c

    ~~~c
   int a=1;
    ~~~

   file2.c

   ~~~c
   #include<stdio.h>
   extern int a;
   int main(void){
   	printf("%d\",a);
   	return 0;
   }
   ~~~

   则执行结果为 1

   但是如果**在file1.c中把int a=1改为static int a=1;**

   那么**在file2.c是无法访问到变量a的**。原因在于用static对全局变量进行修饰改变了其作用域的范围，由原来的整个工程可见变为本源文件可见。

   **注意这里的file1.c和file2.c文件同时编译**

3. 修饰函数

   用static修饰函数的话，情况与修饰全局变量大同小异，就是改变了函数的作用域。

#####C语言中的extern关键字

在C语言中，修饰符extern用在变量或者函数的声明前，用来说明“**此变量/函数是在别处定义的，要在此处引用**”。 在上面的例子中可以看出，在file2中如果想调用file1中的变量a，只须用extern进行声明即可调用a，这就是extern的作用。在这里要注意**extern声明的位置对其作用域也有关系**，如果是在main函数中进行声明的，则只能在main函数中调用，在其它函数中不能调用。其实要调用其它文件中的函数和变量，只需把该文件用#include包含进来即可，为啥要用extern？因为**用extern会加速程序的编译过程，这样能节省时间**。

#### *4-11 修改getop函数使其不必使用ungetch函数（提示：可以使用一个static类型的内部变量）

~~~c
# include <ctype.h>
int getch(void); // 读入下一个待处理的字符
void ungetch(int); // 把字符放回到输入中

// 获取下一个运算符或数值操作数
int getop(char s[]){
    int i, c;
    static int lastc = 0;
    if(lastc == 0) // lastc中有值则读入并置零 否则读入一个字符
        c = getch();
    else{
        c = lastc;
        lastc = 0;
    }
    while((s[0] = c) == ' ' || c == '\t')// 先判断读入的字符是否为blank是则继续读入直到不为blank
        c = getch();

    s[1] = '\0';
    if(!isdigit(c) && c != '.')
        return c; // 不是数

    i = 0;
    if(isdigit(c)) // 收集整数部分
        while(isdigit(s[++i] = c = getch()));
    if(c == '.') // 收集小数部分
        while(isdigit(s[++i] = c = getch()));
    s[i] = '\0';
    if(c != EOF)
        lastc = c; // 不为EOF将多读入的c存入lastc中
    return NUMBER;
}
~~~

### 4.7 寄存器变量

register声明只适用于自动变量以及函数的形式参数

~~~c
f(register unsigned m, register long n){
    register int i;
    ...
}
~~~

实际使用时，编译器会忽视过量或不支持的寄存器变量声明

但**无论寄存器变量实际上是不是放在寄存器中，它的地址都是不能访问的**

### 4.9 初始化

*注*

外部变量和静态变量的初始化表达式必须是常量表达式，且只初始化一次（从概念上来讲是在程序开始执行前进行）

~~~c
int days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
~~~

忽略数组长度时，编译器将把花括号中初始化的表达式个数作为数组的长度，数组长度为12



字符数组的初始化较为特殊

~~~c
char pattern[] = "ould";
~~~

等价于

~~~c
char pattern[] = {'o', 'u', 'l', 'd', '\0'}; // 注意'\0'
~~~

数组长度为5

### 4.10 递归

通过递归，函数printd首先调用他自身打印前面的（高位）数字，然后再打印后面的数字

~~~c
# include <stdio.h>
void printd(int n){
    if(n < 0){
        putchar('-');
        n = -n;
    }
    if(n / 10)
        printd(n/10);
    putchar(n % 10 + '0');
}

main(){
    printd(123);
}
~~~

*注*

此函数无法处理最大的负数

### **&&快速排序**

http://developer.51cto.com/art/201403/430986.htm

算法思想：

1. 对于一个给定的数组，从中选择一个元素，以该元素为界将其余元素划分为两个子集
2. 一个子集中的所有元素都小于该元素，另一个子集中的所有元素都大于或等于该元素
3. 对这两个子集递归执行这一操作，当某个子集中的元素数小于2时，这个子集就不需要再次排序，终止递归

~~~c
// 之后待改进版本
# include <stdio.h>
// 以递增顺序对v[left]...v[right]进行排序
void qsort(int v[], int left, int right){
    int i, last;
    void swap(int v[], int i, int j);

    if(left >= right) // 若数组包含的元素少于两个
        return; // 不执行任何操作
    swap(v, left, (left + right)/2); // 将划分子集的元素
    last = left; // 移动到v[0]
    for(i=left+1; i<=right; i++){ // 划分子集
        if(v[i] < v[left])
            swap(v, ++last, i);
    }
    swap(v, left, last); // 恢复划分子集的元素
    qsort(v, left, last-1);
    qsort(v, last+1, right);
}

void swap(int v[], int i, int j){
    int temp;
    temp = v[i];
    v[i] = v[j];
    v[j] = temp;
}
main(){
    int i;
    int a[] = {6, 1, 2, 5, 9, 3, 4, 7, 10, 8};
    qsort(a, 0, 9);
    for(i=0; i<10; i++)
        printf("%d ",a[i]);
}
~~~

#### *4-12 运用printd函数的设计思想编写一个递归版本的itoa函数，即通过递归调用把整数转换为字符串

~~~c
# include <stdio.h>
// 运用printd函数的设计思想编写一个递归版本的itoa函数
// 即通过递归调用把整数转换为字符串
// 自己写的版本
int itoa(int n, char s[]){
    static int i = 0; // 递归中方便控制i的值 下注
    if(n < 0){
        s[i++] = '-';
        n = -n;
    }
    if(n/10){
        itoa(n/10, s);
    }
    s[i++] = n % 10 + '0';
    s[i] = '\0';
    return i;
}

// 教材
void itoa(int n, char s[]){
    static int i = 0; // 递归中方便控制i的值 下注
    
    if(n/10)
        itoa(n/10, s);
    else{
        i = 0;
        if(n < 0)
            s[i++] = '-';
    }
    s[i++] = abs(n) % 10 + '0';
    s[i] = '\0';
}

main(){
    char s[10];
    itoa(-123, s);
    printf("\n%s", s);
}
~~~

*注*

用一个static变量i作为字符数组s的索引， 且因为``static int i``在itoa函数内定义，所以**i的作用域为itoa函数内**

每次递归调用都将用一个'\0'字符来结束字符数组s，但下一次递归调用后（除最后一次）将覆盖这个'\0'

#### *4-13 用递归实现revers(s)函数，以将字符串s倒置

字符串中的字符是按**由外到内**的顺序依次进行位置的

~~~c
# include <stdio.h>
# include <string.h>
// 用递归实现revers(s)函数，以将字符串s倒置
void reverse(char s[]){
    void reverser(char s[], int i, int j);
    reverser(s, 0, strlen(s));
}

void reverser(char s[], int i, int j){
    char temp;
    int len = j - 1 - i; // 注意与i位置交换的对应位置为j-1-i
    if(i < len){
        temp = s[i];
        s[i] = s[len];
        s[len] = temp;
        i++;
        reverser(s,i,j); // i先加再引用 下注
    }
    else
        return;
}
main(){
    char s[10] = "abc";
    char s2[10] = "abcd";
    reverse(s);
    reverse(s2);
    printf("%s\n%s", s,s2);
}
~~~

*注*

~~~c
i++;
reverser(s, i, j);
~~~

等价于

~~~c
reverser(s, ++i, j);
~~~

本题不适合用递归解决

*另一种做法*

每一层先把最后一个字符提出记录，并把最后一个字符置为\0

将字符串传递到下一层

若字符串长度已经为1，则直接返回

在函数的剩余部分，将字符串所有内容后移一位并将之前的字符放到首位

~~~c
# include <stdio.h>
# include <string.h>
// 用递归实现revers(s)函数，以将字符串s倒置
void reverse(char s[]){
    char temp;
    char c;
    int i;
    int len = strlen(s);
    if(len <= 1)
        return;
    temp = s[len-1]; // 最后一个字符
    s[len-1] = '\0';
    reverse(s);
    for(i=len-1; i>0; i--)
        s[i] = s[i-1];
    s[0] = temp;
}

main(){
    char s[10] = "abc";
    char s2[10] = "abcd";
    reverse(s);
    reverse(s2);
    printf("abc:%s\nabcd:%s", s,s2);
}
~~~

### 4.11.2 宏替换

~~~c
#define max(A, B) ((A) > (B) ? (A) : (B))
~~~

语句``x = max(p+q, r+s);``将被替换为

~~~c
x = ((p+q) > (r+s) ? (p+q) : (r+s));
~~~

**优点：**

如果对各种类型的参数的处理是一致的，则可以将**同一个宏定义应用于任何数据类型**，而无需针对不同的数据类型定义不同的max函数

**缺点：**

作为参数的表达式要重复计算两次，如果表达式中存在副作用（比如含有自增运算或输入/输出）会出现不正确的情况，eg.

![1534163171530](Ch4函数与程序结构.assets/1534163171530.png)

其中j的值加了两次 实际j的值应该为3

**注意**

使用圆括号以保证计算次序的正确性eg.

![1534163641267](Ch4函数与程序结构.assets/1534163641267.png)

应改为``# define square(X) (X) * (X)``答案才能正确为9



可以通过**#undef**指令取消名字的宏定义，保证后续的调用是函数调用

~~~c
#undef getchar
int getchar(void){...}
~~~



形式参数不能用带引号的字符串替换。但是，**如果在替换文本中，参数名以#作为前缀则结果将被扩展为由实际参数替换该参数的带引号的字符串**，例如，可以将它与字符串连接运算结合起来编写一个调试打印宏：

~~~c
# define dprint(expr) printf(#expr " = %g\n", expr)
~~~

使用语句``dprint(x/y);``调用该宏时，将宏扩展为

~~~c
printf("x/y" " = %g\n", expr);
~~~

其中的字符串被连接起来了，等价于``printf("x/y = %g\n", expr);``



预处理器预算符##为宏扩展提供了一种连接实际参数的手段。**如果替换文本的参数与##相邻，则该参数将被实际参数替换，##前后的空白符将被删除，并对替换后的结果重新扫描**，例如下面定义的宏paste用于连接两个参数

~~~c
# define paste(front, back) front ## back
~~~

因此宏调用``paste(name, 1)``的结果将建立记号name1

####4-14 定义宏swap(t, x, y)以交换t类型的两个参数（使用程序块结构会有帮助）

~~~c
#include<stdio.h>

#define swap(t, x, y) {t temp;\
                       temp = x;\
                       x = y;\
                       y = temp;}
int main(){
    int x = 1;
    int y = 2;
    swap(int,x,y);
    printf("x:%d y:%d", x, y);
}
~~~

*注*

上面swap宏只有在两个参数名**都不为temp**的前提下才能工作

### 4.11.3 条件包含

**#if语句**对其中的常量表达式（其中不能包括sizeof、类型转换符或enum常数量）进行求值，若该表达式的值不等于0，则包括其后的各行，直到遇到**#endif**、**#elif**或者**#else**语句为止

在**#if**语句中使用表达式defined（名字），该表达式的值遵循：**当名字已经定义时，其值为1；否则为0**

例如，为了保证hdr.h文件的内容**只被包含一次**则

~~~c
#if !defined(HDR)
#define HDR
/*	hdr.h文件的内容放在这里	*/
#endif
~~~

第一次包含头文件hdr.h时将定义名字HDR；此后再次包含该头文件会发现改名字已经定义，直接跳转到#endif处。``#if !defined(HDR)``等价于``#ifndef HDR``

类似的方法也能用来避免多次重复包含同一文件，例如以下这段预处理代码首先测试系统变量SYSTEM，然后根据该变量的值确定包含哪个版本的头文件

~~~c
#if SYSTEM == SYSV
	#define HDR "sysv.h"
#eif SYSTEM == BSD
	#define HDR "bsd.h"
#eif SYSTEM == MSDOS
	#define HDR "msdos.h"
#else
	#define HDR "default.h"
#endif
#include HDR
~~~

