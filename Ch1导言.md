#  Ch1 导言

### 1.5.2字符计数

~~~c
#include <stdio.h>

main(){
    long count = 0;
    char c;

    for(; (c = getchar())!=EOF; ++count)
        printf("count：%ld char:%c\n", count, c);
}

~~~

![1527333738819](C:\Users\Lee\AppData\Local\Temp\1527333738819.png)

因为输入a回车会被识别为两个输入符号 a 和 回车

### 1.5.3行计数

~~~c
#include <stdio.h>

main(){
    int c, nl;
    for(nl = 0; (c = getchar()) != EOF;)
        if(c == '\n'){
            nl++;
            printf("%d\n", nl);
        }
}
~~~

*注意：*

**'\n'**是单个字符，在表达式中只是一个整数型（ASCII中为10）

**"\n"**是一个仅包含一个字符的字符串常量

#### 1-8统计空格、制表符、换行符个数

~~~c
#include <stdio.h>
//统计空格、制表符、换行符个数
main(){
    int c, spCount, tabCount, enCount;
    spCount = tabCount = enCount = 0;
    while((c = getchar()) != EOF)
        if(c == '\n'){
            enCount++;
            printf("enCount:%d\n", enCount);
        }
        else if(c == ' '){
            spCount++;
            printf("spCount:%d\n", spCount);
        }
        else if(c == '\t'){
            tabCount++;
            printf("tabCount:%d\n", tabCount);
        }
}
~~~

#### 1-9*将输入复制到输出的程序并将其中连续的多个空格用一个空格代替

~~~c
#include <stdio.h>
//将输入复制到输出的程序并将其中连续的多个空格用一个空格代替
#define NONBLANK 'a'
main(){
    char c, lastc;
    lastc = NONBLANK;
    while((c = getchar()) != EOF){
        if(c == ' '){
            if(lastc != ' ')
                putchar(c);
        }
        else
            putchar(c);
        
        lastc = c;
    }
}
~~~

*注意：*

``` if(c == ' ')```后的{}必须有，否则else可能识别为```if(lastc != ' ')```的else

或用if-else

~~~c
#include <stdio.h>
//将输入复制到输出的程序并将其中连续的多个空格用一个空格代替
#define NONBLANK 'a'
main(){
    char c, lastc;
    lastc = NONBLANK;
    while((c = getchar()) != EOF){
        if(c != ' ')
        	putchar(c);
		else if(lastc != ' ')
                putchar(c);

        lastc = c;
    }
}
~~~

或用逻辑或

~~~c
#include <stdio.h>
//将输入复制到输出的程序并将其中连续的多个空格用一个空格代替
#define NONBLANK 'a'
main(){
    char c, lastc;
    lastc = NONBLANK;
    while((c = getchar()) != EOF){
        if(c != ' ' || lastc != ' ')
			putchar(c);
        lastc = c;
    }
}
~~~

#### 1-10将输入复制到输出，并将其中的制表符替换为\t，把回退符替换成\b，把反斜杠替换成\\ 这样可以将制表符和回退符以可见方式显示

~~~c
#include <stdio.h>
//将输入复制到输出，并将其中的制表符替换为\t，把回退符替换成\b，把反斜杠替换成\\ 这样可以将制表符和回退符以可见方式显示
main(){
    char c;
    while((c = getchar()) != EOF){
        if(c == '\t')
            printf("\\t");
        else if(c == '\b')
            printf("\\b");
        else if(c == '\\')
            printf("\\\\");
        else
            putchar(c);
    }
}
~~~

 ### 1.5.4单词计数

~~~c
#include <stdio.h>
//统计输入行数、单词数、字符数
#define OUT 0 //单词外
#define IN 1 //单词内
main(){
    char c;
    int nl, nw, nc, state;
    nl = nw = nc = 0;
    state = OUT;
    while((c = getchar()) != EOF){
        ++nc;
        if(c == '\n')
            ++nl;
        if(c == ' ' || c == '\n' || c == '\t')
            state = OUT;
        else if(state == OUT){
            state = IN;
            ++nw;
        }
    }
}

~~~

*注意：*

因为存在![1527345374192](C:\Users\Lee\AppData\Local\Temp\1527345374192.png)一开始就为空格的情况，所以先判断空格判定状态

**满足边界条件的输入情况有助于发现单词计数程序中的错误**，包括

+ 无输入
+ 无单词（只有换行符）
+ 无单词（只有空格、制表符、换行符）
+ 每个单词占一行（无空格和制表符）
+ 单词仅出现于文本行行首
+ 单词出现于一串空格之后

#### 1-12以每行一个单词的形式打印其输入

~~~c
#include <stdio.h>
//以每行一个单词的形式打印其输入
#define OUT 0 //单词外
#define IN 1 //单词内
main(){
    char c;
    state = OUT;
    while((c = getchar()) != EOF){
        if(state == IN){
            if(c == ' ' || c == '\n' || c == '\t'){
                state = OUT;
                printf("\n");
            }
            else{
                putchar(c);
            }
        }
        else if(c != ' ' && c != '\n' && c != '\t'){
            state = IN;
            putchar(c);
        }
    }
}

~~~

### 1.6数组

~~~c
#include <stdio.h>
//统计各个数字、空白符以及其他字符出现的次数
main(){
    int c, i, nwhite, nother;
    int ndigit[10];

    nwhite = nother = 0;
    for(i=0; i<10; i++)
        ndigit[i] = 0;

    while((c = getchar()) != EOF){
        if(c>='0' && c<='9')//这里注意c是字符无法直接与数字进行比较
            ndigit[c-'0']++;
        else if(c == ' ' || c == '\n' || c == '\t')
            nwhite++;
        else
            nother++;
    }
    printf("white: %d \nother: %d \n", nwhite, nother);
    for(i=0; i<10; i++)
        printf("%d: %d\n",i,ndigit[i]);
}

~~~

#### 1-13*打印输入中单词长度的直方图（水平 垂直）

**统计不同长度单词的数量**

当前字符为空白符{

​	状态->单词外

​	初始状态（尚未输入单词）{

​		单词长度nc=0 

​	}

​	一个单词结束{

​		单词长度nc未超过允许的最大长度{

​			单词结束则该长度下的单词数wl[nc]+1

​		}

​		单词长度超过允许的最大长度{

​			超出长度的单词数ovflow+1

​		}

​		单词长度nc=0

​	}

}

当前字符为非空白符{

​	状态为单词外（即新的单词开始）{

​		记录单词长度为nc=1

​		状态->单词内

​	}

​	状态为单词内（一个单词未输完）{

​		单词长度nc+1

​	}

}

~~~c
#include <stdio.h>

#define IN 1
#define OUT 0
#define MAXWORD 11 // 单词最大长度
#define MAXHIST 15 // 直方图条形长度
// 打印输入中单词长度的直方图（水平、垂直）
// 单词长度为1到10的单词各有几个
// print horizontal histogram 水平直方图
main(){
    int c, i, state;
    int nc; // 记录单词目前为止的长度
    int len;
    int wl[MAXWORD];
    int maxvalue;
    int ovflow;

    //初始化
    state = OUT;
    nc = 0;
    ovflow = 0;
    for(i=0; i<MAXWORD; ++i)
        wl[i] = 0;

    // 统计不同长度单词的数量
    while((c = getchar()) != EOF){
        if(c == ' ' || c == '\n' || c == '\t'){
            state = OUT;
            if(nc > 0)
                if(nc < MAXWORD) // 目前截至单词长度小于10
                    ++wl[nc];
                else
                    ++ovflow;
            nc = 0;
        } else if(state == OUT){
            state = IN;
            nc = 1;
        } else
            ++nc;
    }

    // 找出最长单词
    maxvalue = 0;
    for(i=1; i<MAXWORD; ++i)
        maxvalue = wl[i]>maxvalue?wl[i]:maxvalue;

    // 画直方图
    for(i=1; i<MAXWORD; ++i){
        printf("%5d - %5d : ", i, wl[i]);
        if(wl[i] > 0){
            if((len = wl[i] * MAXHIST / maxvalue) <= 0)
                len = 1;
        }
        else
            len = 0;
        while(len > 0){
            putchar('*');
            --len;
        }
        putchar('\n');
    }

    if(ovflow > 0)
        printf("There are %d words >= %d\n", ovflow, MAXWORD);
}

~~~



![1529493613580](C:\Users\Lee\AppData\Local\Temp\1529493613580.png)

**画垂直方向的直方图**

垂直方向直方图的所有直方条同步打印

~~~c
#include <stdio.h>

#define IN 1
#define OUT 0
#define MAXWORD 11 // 单词最大长度
#define MAXHIST 15 // 直方图条形长度
// 打印输入中单词长度的直方图（水平、垂直）
// 单词长度为1到10的单词各有几个
// print vertical histogram 垂直方向
main(){
    int c, i, j, state;
    int nc; // 记录单词目前为止的长度
    int len;
    int wl[MAXWORD];
    int maxvalue;
    int ovflow;

    //初始化
    state = OUT;
    nc = 0;
    ovflow = 0;
    for(i=0; i<MAXWORD; ++i)
        wl[i] = 0;

    // 统计不同长度单词的数量
    while((c = getchar()) != EOF){
        if(c == ' ' || c == '\n' || c == '\t'){
            state = OUT;
            if(nc > 0)
                if(nc < MAXWORD) // 目前截至单词长度小于10
                    ++wl[nc];
                else
                    ++ovflow;
            nc = 0;
        } else if(state == OUT){
            state = IN;
            nc = 1;
        } else
            ++nc;
    }

    // 找出最长单词
    maxvalue = 0;
    for(i=1; i<MAXWORD; ++i)
        maxvalue = wl[i]>maxvalue?wl[i]:maxvalue;

    // 画直方图
    for(i=MAXHIST; i>0; --i){
        for(j=1; j<MAXWORD; ++j){
            if(wl[j] * MAXHIST / maxvalue >= i)
                printf("  *  ");
            else
                printf("     ");
        }
        putchar('\n');
    }
    for(i=1; i<MAXWORD; ++i)
        printf("  %d  ", i);
    putchar('\n');
    for(i=1; i<MAXWORD; ++i)
        printf("  %d  ", wl[i]);

    if(ovflow > 0)
        printf("There are %d words >= %d\n", ovflow, MAXWORD);
}

~~~



![1529495836028](C:\Users\Lee\AppData\Local\Temp\1529495836028.png)

#### 1-14打印输入中各个字符出现频度的直方图

~~~c
//自己写的版本
#include <stdio.h>
#define MAXCHAR 50
#define MAXHIST 15
// 打印输入中各个字符出现频度的直方图
main(){
    int in, i, j, len;
    int isVisible = 0; // 字符不可见
    int isExist = 0; // 字符未存在
    int sum = 0; // 输入不同字符数
    char c[MAXCHAR] = ""; // 存放字符
    int count[MAXCHAR] = {0}; // 存放每个字符出现次数
    int maxcount = 0; //字符出现最多次数

    while((in = getchar()) != EOF){
        if(in == ' ' || in == '\n' || in == '\t')
            isVisible = 0;
        else
            isVisible = 1;

        isExist = 0;
        if(isVisible){
            for(i=0; i<sum; ++i){
                if(in == c[i]){
                    count[i]++;
                    isExist = 1;
                    break;
                }
            }
            if(!isExist){
                sum++;
                c[sum-1] = in;
                count[sum-1]++;
            }
        }
    }

    for(i=0; i<sum; ++i)
        maxcount = count[i] > maxcount ? count[i] : maxcount;

    /*// 水平
    for(i=0; i<sum; ++i){
        printf("%5c - %5d : ", c[i], count[i]);
        if(count[i] > 0){
            if((len = count[i] * MAXHIST / maxcount) <= 0)
                len = 1;
        }
        else
            len = 0;
        while(len > 0){
            putchar('*');
            --len;
        }
        putchar('\n');
    }*/

	// 垂直
    for(i=MAXHIST; i>=0; --i){
        for(j=0; j<sum; ++j){
            if((count[j] * MAXHIST / maxcount) >= i)
                printf("  *  ");
            else
                printf("     ");
        }
        putchar('\n');
    }
    for(i=0; i<sum; ++i)
        printf("  %c  ", c[i]);
    putchar('\n');
    for(i=0; i<sum; ++i)
        printf("  %d  ", count[i]);
}
~~~

![1529507611801](C:\Users\Lee\AppData\Local\Temp\1529507611801.png)

~~~c
// 教材
#include <stdio.h>
#include <ctype.h>

#define MAXCHAR 128 // max different characters
#define MAXHIST 15
// 打印输入中各个字符出现频度的直方图
// print horizontal histogram freq. of different characters
main(){
    int c, i, len;
    int maxvalue;
    int cc[MAXCHAR];

    for(i=0; i<MAXCHAR; i++)
        cc[i] = 0;

    while((c = getchar()) != EOF){
        if(c < MAXCHAR)
            ++cc[c];
    }

    maxvalue = 0;
    for(i=1; i<MAXCHAR; ++i)
        maxvalue = cc[i] > maxvalue ? cc[i] : maxvalue;

    // 水平
    for(i=1; i<MAXCHAR; ++i){
        if(isprint(i))
            printf("%5d - %5c - %5d : ", i, i, cc[i]);
        else
            printf("%5d -   - %5d : ", i, cc[i]);
        if(cc[i] > 0){
            if((len = cc[i] * MAXHIST / maxvalue) <= 0)
                len = 1;
        }
        else
            len = 0;
        while(len > 0){
            putchar('*');
            --len;
        }
        putchar('\n');
    }
}
~~~

![1529508530115](C:\Users\Lee\AppData\Local\Temp\1529508530115.png)

### 1.8参数——传值调用

C语言中所有参数都是**值传递**，传递给调用函数的参数值存放在**临时变量**中而不是原来的位置

被调用函数**不能直接**修改主调函数中变量的值，而只能修改其私有的临时副本的值

在**被调用函数**中**参数**可以看作是便于初始化的局部变量，如

参数n作为临时变量

~~~c
//求底数的n次幂
int power(int base, int n){
    int p;
    for(p=1; n>0; --n)
    	p *= base;
    return p;
}
~~~

### 1.9字符数组

读入一组文本，并把最长文本行打印出来

~~~c
#include <stdio.h>
#define MAXLINE 1000 // 允许输入行的最大长度

int getline(char line[], int maxline);
void copy(char to[],char from[]);

main(){
    int len; // 当前行长度
    int max; // 目前发现最大长度
    char line[MAXLINE]; // 当前最大行
    char longset[MAXLINE]; // 用于保存最长的行
    int i;

    max = 0;
    while((len = getline(line,MAXLINE)) > 0){
        if(len > max){
            max = len;
            copy(longset, line);
        }
    }
    if(max > 0)
       /*printf("%s",longset);*/
       for(i=0;i<max;i++)
            printf("%c",longset[i]);
    return 0;
}

int getline(char s[], int lim){
    int c, i, j;

    for(i=0; i<lim-1 && (c = getchar()) != EOF && c!='\n'; ++i)
        s[i] = c;
    if(c == '\n'){
       s[i] = c;
       i++;
    }
    s[i] = '\0';

    return i;
}

void copy(char to[], char from[]){
    int i = 0;
    while((to[i] = from[i]) != '\0')
        ++i;
}
~~~

*注意*

getline函数应该在读到文件末尾返回一个信号，更有用的设计是他能够在读入文本行时返回改行的长度而遇到文本结尾时返回0，因为0不是有效长度，**每一行至少长度为1只包含换行符的行**。



`int getline(char s[], int lim);`中没有必要指明数组s的长度，因为该数组的长度是在**main函数**中设置的。



printf函数中的格式规范**%s**规定，对应的参数必须是以**‘\0’**结束的字符串‘。

**空字符串’\0‘**不是普通文本的一部分。

#### 1-16*打印任意长度的输入行的长度并尽可能多的打印文本

~~~c
#include <stdio.h>
#define MAXLINE 1000 // 允许输入行的最大长度

// 打印任意长度的输入行的长度并尽可能多的打印文本 maximum input line size
int getline(char line[], int maxline);
void copy(char to[],char from[]);

int main(){
    int len; // 当前行长度
    int max; // 目前发现最大长度
    char line[MAXLINE]; // 当前最大行
    char longset[MAXLINE]; // 用于保存最长的行
    int i;

    max = 0;
    while((len = getline(line,MAXLINE)) > 0){
        printf("%d, %s", len, line);
        if(len > max){
            max = len;
            copy(longset, line);
        }
    }
    if(max > 0)
       /*printf("%s",longset);*/
       for(i=0;i<max;i++)
            printf("%c",longset[i]);
    return 0;
}

int getline(char s[], int lim){
    int c, i, j;
    //字符串长度通过i返回
    //变量j记录被复制到字符串s中的个数
    j = 0;
    for(i=0; (c = getchar()) != EOF && c!='\n'; ++i)
        if(i < lim-2){//给换行符留一个位置 字符串结束符留一个位置
            s[j] = c;
            j++;
        }
    if(c == '\n'){
        s[j] = c;
        j++;
        i++;
    }
    s[j] = '\0';

    return i;
}

void copy(char to[], char from[]){
    int i = 0;
    while((to[i] = from[i]) != '\0')
        ++i;
}
~~~

#### 1-18删除每个输入行行末的空格以及制表符，并删除完全空格的行

~~~c
//自己写的版本
#include <stdio.h>
#define MAXLINE 1000 // 允许输入行的最大长度

// 删除每个输入行行末的空格以及制表符，并删除完全空格的行
int getline(char line[], int maxline);

int main(){
    char line[MAXLINE];
    int len = 0;

    while((len = getline(line,MAXLINE)) > 0)
        if(len !=1);
            printf("%s", line);
    
    return 0;
}

int getline(char s[], int lim){
    int c, i;
    int isEND = 0;
    int j; // 记录末尾共几个空格制表符
    i = j = 0;
    while((c = getchar()) != EOF && c != '\n'){
        if(c == ' ' || c == '\t'){
            isEND = 1;
            j++;
        }
        else{
            if(isEND){
                isEND = 0;
                j = 0;
            }
        }
        s[i++] = c;
    }
    if(c == '\n'){
        if((i-j) < 0) // 全是空格
            return 1;
        s[i-j] = c;
        i++;
    }
    s[i-j] = '\0';
    return i-j;
}
~~~

~~~c
//教材
#include <stdio.h>
#define MAXLINE 1000

int getline(char line[], int maxline);
int removeline(char s[]);

main(){
    char line[MAXLINE];

    while(getline(line, MAXLINE) > 0)
        if(removeline(line) > 0)
            printf("%s", line);
    return 0;
}

int getline(char s[], int lim){
    int c, i, j;
    //字符串长度通过i返回
    //变量j记录被复制到字符串s中的个数
    j = 0;
    for(i=0; (c = getchar()) != EOF && c!='\n'; ++i)
        if(i < lim-2){//给换行符留一个位置 字符串结束符留一个位置
            s[j] = c;
            j++;
        }
    if(c == '\n'){
        s[j] = c;
        j++;
        i++;
    }
    s[j] = '\0';

    return i;
}
// remove函数负责删掉字符串line末尾的空格和制表符并返回新的长度
// 若长度大于0则line中不是全由空格制表符构成
int removeline(char s[]){
    int i;

    i = 0;
    while(s[i] != '\n') // find newline character
        ++i;

    --i; // back off from '\n'

    // 从末尾开始检查
    while(i >= 0 && (s[i] == ' ' || s[i] == '\t'))
        --i;

    if(i >= 0){ // is it a nonblank line?
        ++i;
        s[i] = '\n'; // put newline character back
        ++i;
        s[i] = '\0';
    }

    return i;
}
~~~

####1-19使字符串中的字符顺序颠倒过来，每次点到一个输入行中的字符顺序

~~~c
#include <stdio.h>
#define MAXLINE 1000

int getline(char line[], int maxline);
void reverse_(char s[]);
// 使字符串中的字符顺序颠倒过来，每次点到一个输入行中的字符顺序
main(){
    char line[MAXLINE];

    while(getline(line, MAXLINE) > 0){
        reverse_(line);
        printf("%s",line);
    }

    return 0;
}

int getline(char s[], int lim){
    int c, i, j;
    //字符串长度通过i返回
    //变量j记录被复制到字符串s中的个数
    j = 0;
    for(i=0; (c = getchar()) != EOF && c!='\n'; ++i)
        if(i < lim-2){//给换行符留一个位置 字符串结束符留一个位置
            s[j] = c;
            j++;
        }
    if(c == '\n'){
        s[j] = c;
        j++;
        i++;
    }
    s[j] = '\0';

    return i;
}

void reverse_(char s[]){
    int i = 0;
    int j = i;
    char temp;

    while(s[i] != '\0')
        i++;
    i--;
    if(s[i] == '\n')
        i--;
    j = i;

    for(i=0; i<j; i++,j--){
        temp = s[i];
        s[i] = s[j];
        s[j] = temp;
    }
}
~~~

###1.10外部变量与作用域

~~~c
#include <stdio.h>
#define MAXLINE 1000 // 允许输入行的最大长度

int max;
char line[MAXLINE];
char longest[MAXLINE];

int getline(void);
void copy(void);

main(){
    int len; // 当前行长度
    extern int max; // 目前发现最大长度
    extern char longest[]; // 用于保存最长的行
    int i;

    max = 0;
    while((len = getline()) > 0){
        if(len > max){
            max = len;
            copy();
        }
    }
    if(max > 0)
       printf("%s",longest);
       return 0;
}

int getline(){
    int c, i;
    extern char line[];

    for(i=0; i<MAXLINE-1 && (c = getchar()) != EOF && c!='\n'; ++i)
        line[i] = c;
    if(c == '\n'){
       line[i] = c;
       i++;
    }
    line[i] = '\0';

    return i;
}

void copy(){
    int i = 0;
    //extern char longest[];
    while((longest[i] = line[i]) != '\0')
        ++i;
}
~~~

外部变量必须**定义**在所有函数之外，且只能定义一次，定义后编译程序将为它分配存储空间

每个需要访问外部变量的函数中必须声明相应的外部变量，可以用**extern语句 显示声明**，也可通过**上下文 隐式声明**

*注意*

**如果外部变量的定义出现在使用它的函数之前，那么在那个函数中就没有必要使用extern声明**

通常所有的外部变量的定义都放在源文件的开始处，这样就可以省略extern声明

！！！**定义（define）**和**声明declaration**区别

**定义**：创建变量或分配存储单元

**声明**：说明变量的性质，但**不分配存储空间**

####1-20*编写程序detab，将输入中的制表符替换成适当数目的空格，使空格充满到下一个制表符终止位的地方

假设每隔TABING个位置就出现一个制表符，每当遇到制表符时，程序需要计算出要到达下一个制表位需要的空格数

~~~c
#include <stdio.h>
// 编写程序detab，将输入中的制表符替换成适当数目的空格，使空格充满到下一个制表符终止位的地方
#define TABING 8

main(){
    int pos = 0; // 当前字符读取位置
    int nespace = 0; // 需要补充的空格数
    char c;

    while((c = getchar()) != EOF){
        pos++;
        if(c == '\t'){
            /*教材
            nespace = TABING - ( pos - 1 ) % TABING;
            pos += (nespace - 1);
                while(nespace > 0){
                    putchar('^');
                    nespace--;
                }
            */
            if( pos % TABING == 0)
                putchar('^');
            else{
                nespace = TABING - pos % TABING + 1;
                pos += (nespace - 1);
                while(nespace > 0){
                    putchar('^');
                    nespace--;
                }
            }
        }
        else if(c == '\n'){
            putchar(c);
            pos = 0;
        }
        else{
            putchar(c);
        }
    }
}
~~~

*注意*

教材中计算所需空格数的公式为**``nespace = TABING - ( pos - 1) % TABING;``**

~~~c
// 教材
#include <stdio.h>
// 编写程序detab，将输入中的制表符替换成适当数目的空格，使空格充满到下一个制表符终止位的地方
#define TABING 8
// replace tabs with the proper number of blanks
main(){
    int c, nb, pos;

    nb = 0; // number of blanks necessary
    pos = 1; // position of character in line
    while((c = getchar()) != EOF){
        if(c == '\t'){
            nb = TABING - ( pos - 1 ) % TABING;
            while(nb > 0){
                putchar(' ');
                ++pos;
                --nb;
            }
        }
        else if(c == '\n'){
            putchar(c);
            pos = 1;
        }
        else{
            putchar(c);
            ++pos;
        }
    }
}
~~~



#### 1-21*编写程序entab，将空格串替代为最少数量的制表符和空格，但要保持单词之间的间隔不变。当使用一个制表符或一个空格都可以到达下一个制表符终止位时，选用哪一种替换字符较好？

当输入为空格时，累计空格数直到满足制表符结束位替换为一个制表符（制表符++，空格数置零）

输入为其他时先输出累计的制表符

当输入为制表符时，如果还有多余的空格数将空格数置零，更改当前输入位占满到制表符结束位，并输出一个制表符

当输入为非空格制表符的其他字符时，先输出多余的空格，再输出当前字符，如果为换行符将位置初始化

~~~c
// 自己写的版本
# include <stdio.h>
// 编写程序entab，将空格串替代为最少数量的制表符和空格，但要保持单词之间的间隔不变。
# define TABING 8
main(){
    char c;
    int pos, blank, tab;

    pos = 0; // 记录当前字符位置
    blank = 0; // 当前空格数
    tab = 0; // 空白中可替换成tab数的数目

    while((c = getchar()) != EOF){
        pos++;
        if(c == ' '){
            blank++;
            if(pos % TABING == 0){
                tab++;
                blank = 0;
            }
        }
        else{
            while(tab > 0){
                putchar('\t');
                tab--;
            }
            if(c == '\t'){
                pos--;
                pos += TABING - ( pos - 1) % TABING;
                putchar('\t');
                if(blank > 0)
                    blank = 0;
            }
            else{
                while(blank > 0){
                    //putchar(' ');
                    putchar('*');
                    blank--;
                }
                putchar(c);
                if(c == '\n')
                    pos = 0;
            }
        }
    }
}
~~~

~~~c
// 教材
# include <stdio.h>

#define TABING 8
// replace strings of blanks with tabs and blanks
main(){
    int c, nb, nt, pos;

    nb = 0; // # of blanks necessary
    nt = 0; // # of tabs necessary
    for(pos = 1; (c = getchar()) != EOF; ++pos){
        if(c == ' '){
            if(pos % TABING != 0)
                ++nb; // increment # of blanks
            else{
                nb = 0; // reset # of blanks
                ++nt; // one more tab
            }
        }
        else{
            for(; nt > 0; --nt)
                putchar('\t'); // output tab(s)
            if(c == '\t') // forget the blank(s)
                nb = 0;
            else{ // output blank(s)
                for(; nb > 0; --nb)
                    putchar('*');
                    //putchar(' ');
            }
            putchar(c);
            if(c == '\n')
                pos = 0;
            else if(c == '\t')
                pos = pos + ( TABING - ( pos - 1 ) % TABING) - 1;
        }
    }
}
~~~

####1-22*把较长的输入行“折”成短一些的两行或多行，折行的位置在输入行的第n列之前的最后一个非空格之后。要保证程序能够智能地处理输入行很长以及在指定的列前没有空格或制表符时的情况

~~~c
// 自己写的版本
# include <stdio.h>

# define TABING 8
# define N 2 // 折行列
# define MAX 500 // 最大输入字数
// 把较长的输入行“折”成短一些的两行或多行，折行的位置在输入行的第n列之前的最后一个非空格之后。
// 要保证程序能够智能地处理输入行很长以及在指定的列前没有空格或制表符时的情况
main(){
    int c, pos, i, j;
    char temp[MAX] = "";

    pos = 0; // 每行当前位置
    i = 0;
    while((c = getchar()) != EOF){
        pos++;
        if(c == '\n'){ // 遇到换行 如果缓存中还有值先输出再换行
            for(j=0; i>0; j++, i--){
                putchar(temp[j]);
                temp[j] = "";
            }
            putchar('\n');
            pos = 0;
        }
        else if(pos <= N){ // 当前小于折行列
            if(c != ' ' && c != '\t'){ // 非空格tab 将输入放入缓存中
                temp[i] = c;
                i++;
            }
            else{
                // 遇到空格tab时缓存中有值则先输出并置零
                for(j=0; i>0; j++, i--){
                    putchar(temp[j]);
                    temp[j] = "";
                }
                // 输出空格tab
                if(c == ' '){
                    putchar(c);
                }
                else{
                    pos--;
                    pos += TABING - ( pos - 1 ) % TABING;
                    putchar('\t');
                }
            }
        }
        else{ // 第N+1列
            if(c == ' ' || c == '\t'){
                putchar('\n');
                pos = 1;
                putchar(c);
                if(c == '\t')
                    pos = TABING;
            }
            else{ // 为字符时先判断缓存中是否有值 有值需要先输出 并更改pos位置
                for(j=0; i>0; j++, i--){
                    putchar(temp[j]);
                    temp[j] = "";
                    }
                putchar('\n');
                pos = 1;
                putchar(c);
            }
        }
    }
}
~~~



~~~c
// 教材
# include <stdio.h>

# define MAXCOL 10
# define TABING 8

char line[MAXCOL]; // inputline

int exptab(int pos);
int findblnk(int pos);
int newpos(int pos);
void printl(int pos);

main(){
    int c, pos;

    pos = 0;
    while((c = getchar()) != EOF){
        line[pos] = c; // store current character
        if(c == '\t') // expand tab character
            pos = exptab(pos);
        else if(c == '\n'){
            printl(pos); // print current input line
            pos = 0;
        }
        else if(++pos >= MAXCOL){
            pos = findblnk(pos);
            printl(pos);
            pos = newpos(pos);
        }
    }
}

// print line until pos column
// 打印输出位置零到位置pos-1之间的字符
void printl(int pos){
    int i;
    for(i=0; i<pos; ++i)
        putchar(line[i]);
    if(pos > 0) //any chars printed?
        putchar('\n');
}

// expand tab into blanks
int exptab(int pos){
    line[pos] = ' '; // tab is at least one blank
    for(++pos; pos< MAXCOL && pos % TABING != 0; ++pos)
        line[pos] = ' ';
    if(pos < MAXCOL)
        return pos; // room left in current line
    else{
        printl(pos);
        return 0; // reset current position
    }
}

// find blank's position
// 从输入行的pos开始倒退寻找第一个空格 如果找到一个空格符就返回紧跟在该空格后面的那个位置的下标，如果没有找到空格，返回MAXCOL
int findblnk(int pos){
    while(pos > 0 && line[pos] != ' ')
        --pos;
    if(pos == 0) // no blank in the line
        return MAXCOL;
    else // at least one blank
        return pos+1; // position after the blank
}

// rearrange line with new position
// 调整输入行，把从位置pos开始的字符复制到下一个输出行的开始，然后再返回变量pos的新值
int newpos(int pos){
    int i, j;

    if(pos <= 0 || pos >= MAXCOL)
        return 0; // nothing to rearrange
    else{
        i = 0;
        for(j=pos; j<MAXCOL; ++j){
            line[i] = line[j];
            ++i;
        }
        return i; // new position in line
    }
}
~~~

#### 1-23删除C语言程序中所有的注释语句，要正确处理带引号的字符串与字符常量，在C语言中不允许嵌套

状态转移图画出来后其实可以直接用一个变量用不同数值保存状态，用布尔变量太麻烦了，之后有时间可以修改一下

**注意字符串中``"\"sdf"``这种``"``被转义的情况**

~~~c
// 自己写的版本
# include <stdio.h>
// 删除C语言程序中所有的注释语句，要正确处理带引号的字符串与字符常量，在C语言中不允许嵌套
main(){
    int isStr, maybCom, bCom1, bCom2, mayeCom, eCom, isCom, isNor;
    char c;

    isNor = 1; // 正常状态
    isStr = 0; // 不是字符串
    maybCom = 0; // 遇到/可能开始注释
    bCom1 = 0; // 遇到//开始注释
    bCom2 = 0; // 遇到/*开始注释
    mayeCom = 0; // 遇到/可能结束注释
    eCom = 0; // 遇到*/结束注释
    isCom = 0; // 不是注释
    while((c = getchar()) != EOF){
        if(isNor){
            if(c == '\'' || c == '\"'){
                isStr = 1;
                isNor = 0;
                putchar(c);
            }
            else if(c == '/'){
                maybCom = 1;
                isNor = 0;
            }
            else{
                putchar(c);
            }
        }
        else if(maybCom){
            if(c == '/'){
                bCom1 = 1;
                isCom = 1;
                maybCom = 0;
            }
            else if(c == '*'){
                bCom2 = 1;
                isCom = 1;
                maybCom = 0;
            }
            else{
                isNor = 1;
                maybCom = 0;
                putchar('/');
                putchar(c);
            }
        }
        else if(isCom){
            if(bCom1 && c == '\n'){
                isCom = 0;
                isNor = 1;
                bCom1 = 0;
            }
            else if(bCom2){
                if(c == '*'){
                    mayeCom = 1;
                }
                if(mayeCom && c == '/'){
                    isCom = 0;
                    isNor = 1;
                    mayeCom = 0;
                    bCom2 = 0;
                }
                else if(mayeCom && c != '/' && c != '*'){ // 这里注意不能是* 因为当前为第一个*时满足在mayeCom且不会/会出错
                    mayeCom = 0;
                }
            }
        }
        else if(isStr){
            if(c == '\\'){ // 注意字符串中的转义情况！！！
                c = getchar();
                putchar(c);
            }
            else if(c != '\'' && c != '\"'){
                putchar(c);
            }
            else{
                isStr = 0;
                isNor = 1;
                putchar(c);
            }
        }
    }
}
~~~

教材上未考虑``//``的注释情况

~~~c
// 教材
# include <stdio.h>
// 删除C语言程序中所有的注释语句，要正确处理带引号的字符串与字符常量，在C语言中不允许嵌套
// remove all comments from a valid C program
void rcomment(int c);
void in_comment(void);
void echo_quote(int c);

main(){
    int c, d;

    while((c = getchar()) != EOF)
        rcomment(c);
    return 0;
}

// rcomment: read each character, remove the comments
void rcomment(int c){
    int d;
    if(c == '/')
        if((d = getchar()) == '*')
        in_comment(); // beginning the comments
    else if(d == '/'){ // another slash
        putchar(c);
        rcomment(d);
    }
    else{ // not a comment
        putchar(c);
        putchar(d);
    }
    else if(c == '\'' || c == '\"')
        echo_quote(c); // quote begin
    else
        putchar(c); // not a comment
}

// in_commeent: inside of a valid comment
void in_comment(void){
    int c, d;
    c = getchar(); // prev character
    d = getchar(); // curr character
    while(c != '*' || d != '/'){ // search for end
        c = d;
        d = getchar();
    }
}

// echo_quote: echo characters within quotes
void echo_quote(int c){
    int d;

    putchar(c);
    while((d = getchar()) != c){ // search for end
        putchar(d);
        if(d == '\\'){
            putchar(getchar()); //ignore escape seq
        }
    }
    putchar(d);
}
~~~

#### 1-24*查找C语言程序中的基本语法错误，如括号不匹配。要正确处理引导（包括单引号、双引号）、转义字符序列与注释

~~~c
// 自己写的版本
# include <stdio.h>
// 查找C语言程序中的基本语法错误，如括号不匹配。要正确处理引导（包括单引号、双引号）、转义字符序列与注释
# define NORMAL 0
# define MAYBCOM 1
# define INCOM1 2
# define INCOM2 3
# define UNBALAN 4
# define MAYECOM 5
# define INSTR1 6
# define INSTR2 7

int brace, brack, paren; // { [ (

main(){
    char c, d;
    int state;

    state = NORMAL;
    while((c = getchar()) != EOF){
        if(state == MAYBCOM){
           // printf("MAYBCOM ");
            if(c == '/'){
                state = INCOM1;
                c = getchar();
            }
            else if(c == '*'){
                state = INCOM2;
                c = getchar();
            }
            else
                state = NORMAL;
        }
        if(state == INCOM1){
           // printf("INCOM1 ");
            if(c == '\n'){
                state = NORMAL;
                c = getchar();
            }
        }
        if(state == INCOM2){
           // printf("INCOM2 ");
            if(c == '*'){
                state = MAYECOM;
                c = getchar();
            }
        }
        if(state == MAYECOM){
            //printf("MAYECOM ");
            if(c == '/'){
                state = NORMAL;
                c = getchar(); // 否则会重复判断状态为MAYBCOM状态
            }
            else
                state = INCOM2;
        }
        if(state == INSTR1){
            if(c == '\\') // 处理转义
                getchar();
            else if(c == '\'' ){
                state = NORMAL;
                c = getchar();
            }
        }
        if(state == INSTR2){
            if(c == '\\') // 处理转义
                getchar();
            else if(c == '\"' ){
                state = NORMAL;
                c = getchar();
            }
        }
        if(state == NORMAL){
            if(c == '/')
                state = MAYBCOM;
            else if(c == '\'')
                state = INSTR1;
            else if(c == '\"')
                state = INSTR2;
            else if(c == '{')
                brace++;
            else if(c == '}')
                brace--;
            else if(c == '[')
                brack++;
            else if(c == ']')
                brack--;
            else if(c == '(')
                paren++;
            else if(c == ')')
                paren--;
            if(brace < 0 || brack < 0 || paren < 0){
                state = UNBALAN;
                brace = brack = paren = 0;
            }
        }
    }
    if(state == INCOM2)
        printf("unbalance */\n");
    else if(state == INSTR1 || state == INSTR2)
        printf("unbalance \' or \"\n");
    if(state == UNBALAN || brace > 0 || brack > 0 || paren > 0)
        printf("unbalance brace\n");
}
~~~

*注意*

``if``和``else if``的区别 容易出错

~~~c
// 教材
# include <stdio.h>
// 查找C语言程序中的基本语法错误，如括号不匹配。要正确处理引导（包括单引号、双引号）、转义字符序列与注释
// rudimentary(基本的) syntax checker for C program
int brace, brack, paren;

void in_quote(int c);
void in_comment(void);
void search(int c);

main(){
    int c;
    while((c = getchar()) != EOF){
        if(c == '/'){
            if((c = getchar() == '*'))
                in_comment(); // inside comment
            else
                search(c);
        }
        else if(c == '\'' || c == '\"')
            in_quote(c); // inside quote
        else
            search(c);

        // output error
        if(brace < 0){
            printf("Unbalanced braces\n");
            brace = 0;
        }
        else if(brack < 0){
            printf("Unbalanced brackets\n");
            brack = 0;
        }
        else if(paren < 0){
            printf("Unbalanced parentheses\n");
            paren = 0;
        }
    }
    if(brace > 0)
        printf("Unbalanced braces\n");
    if(brack > 0)
        printf("Unbalanced brackets\n");
    if(paren > 0)
        printf("Unbalanced parentheses\n");
}

// search for rudimentary syntax errors
void search(int c){
    if(c == '{')
        ++brace;
    else if(c == '}')
        --brace;
    else if(c == '[')
        ++brack;
    else if(c == ']')
        --brack;
    else if(c == '(')
        ++paren;
    else if(c == ')')
        --paren;
}

// inside of a valid comment
void in_comment(void){
    int c, d;
    c = getchar(); // prev char
    d = getchar(); // curr char
    while(c != '*' || d != '/'){ // search for end
        c = d;
        d = getchar();
    }
}

// inside quote
void in_quote(int c){
    int d;
    while((d = getchar()) != c){ //search end quote
        if(d == '\\')
            getchar(); // ignore escape seq
    }
}
~~~

*注意*

``]][[``这种情况是非法的因此要加上

~~~c
if(brace < 0){
	printf("Unbalanced braces\n");
	brace = 0;
}
else if(brack < 0){
	printf("Unbalanced brackets\n");
	brack = 0;
}
else if(paren < 0){
	printf("Unbalanced parentheses\n");
	paren = 0;
}
~~~

