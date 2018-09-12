# Ch6 结构

### 6.1 结构的基本知识

~~~c
struct {...} x, y, z;
// 语法角度上与下相同
int x, y, z;
~~~

~~~c
struct point {
    int x;
    int y;
}
struct rect {
    struct point pt1;
    struct point pt2;
}

struct rect screen;

// 可以引用screen的成员项pt1的x坐标
screen.pt1.x
~~~

### 6.2 结构与函数

~~~c
// 通过x、y坐标构造一个点
struct point makepoint(int x, int y){
    struct point temp;
    temp.x = x;
    temp.y = y;
    return temp;
}
~~~

现在可以使用makepoint函数动态初始化任意结构

~~~c
struct rect screen;
struct point middle;
screen.pt1 = makepoint(0, 0);
screen.pt2 = makepoint(XMAX, YMAX);
middle = makepoint((screen.pt1.x + screen.pt2.x)/2,
                   (screen.pt1.y + screen.pt2.y)/2);
~~~

结构类型参数和其他类型的参数一样，通过值传递

~~~c

~~~

~~~c
// 如果点p在矩形r内，则返回1， 否则返回0
int ptinrect(struct point p, struct rect r){
    return p.x >= r.p1.x && p.x <= r.p2.x 
        && p.y >= r.p1.y && p.y <= r.p2.y;
}
~~~

~~~c
// 将矩形坐标规范化
# define min(a,b) ((a) < (b) ? (a) : (b))
# define max(a,b) ((a) > (b) ? (a) : (b))
struct rect canonrect(struct rect r){
    struct rect temp;
    temp.p1.x = min(r.p1.x, r.p2.x);
    temp.p1.x = max(r.p1.x, r.p2.x);
    temp.p1.y = min(r.p1.y, r.p2.y);
    temp.p1.y = max(r.p1.y, r.p2.y);
	return temp;
}
~~~

如果传递给函数的结构很大，使用指针方式的效率通常比复制整个结构的效率要高

~~~c
struct point *pp;
// pp指向一个point结构， *pp为该结构 (*pp).x和(*pp).y是结构成员

struct point origin;
pp = &origin;
printf("origin is (%d, %d)\n", (*pp).x, (*pp).y);
~~~

**“.”的优先级比“*”的优先级高**

**(\*pp).x, (\*pp).y 等价于 pp->x, pp->y**

**运算符. 和->都是从左至右结合的**



结构运算符“.”和“->” 用于函数调用的“()” 用于下标的“[]” 优先级最高，同操作数之间的结合也最紧密

~~~~c
struct {
    int len;
    char *str;
} *p;
~~~~

~~~c
++p->len // 增加len的值而不是增加p的值
*p->str // 指针str所指向对象的值
*p->str++ // 读取指针str指向对象的值，然后再将str加1
(*p->str)++ // 将指针str指向的对象的值加1
*p++->str // 先读取指针str指向对象的值，然后再将p加1
~~~

### 6.3 结构数组

统计输入中各个C语言关键字出现的次数

~~~c
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAXWORD 100
#define NKEYS (sizeof keytab / sizeof(struct key)) // keytab中的个数

struct key{
    char *word;
    int count;
} keytab[] = {
    { "auto", 0 },
    { "break", 0 },
    { "case", 0 },
    { "char", 0 },
    { "const", 0 },
    { "continue", 0 },
    { "default", 0 },
    { "unsigned", 0 },
    { "void", 0 },
    { "volatile", 0 },
    { "while", 0 }
}; // 关键字序列以升序存储在keytab中

int getword(char *, int);
int binsearch(char *word, struct key tab[], int n);

main(){
    int n;
    char word[MAXWORD];

    while(getword(word, MAXWORD) != EOF){
        if(isalpha(word[0]))
            if((n = binsearch(word, keytab, NKEYS)) >= 0)
                keytab[n].count++;
    }
    for(n=0; n<NKEYS; n++)
        if(keytab[n].count > 0)
            printf("%4d %s\n",keytab[n].count, keytab[n].word);
    return 0;
}

// 在tab[0]到tab[n-1]中查找单词
int binsearch(char *word, struct key tab[], int n){
    int cond;
    int low, high, mid;

    low = 0;
    high = n - 1;
    while(low <= high){
        mid = (low + high)/2;
        if((cond = strcmp(word, tab[mid].word)) < 0)
            high = mid -1;
        else if(cond > 0)
            low = mid + 1;
        else
            return mid;
    }
    return -1;
}

// 从输入中读取下一个单词或字符
int getword(char *word, int lim){
    int c, getch(void);
    void ungetch(int);
    char *w = word;

    while(isspace(c = getch()));
    if(c != EOF)
        *w++ = c;
    if(!isalpha(c)){
        *w = '\0';
        return c;
    }
    for(; --lim>0; w++)
        if(!isalnum(*w = getch())){ // 识别字母数字
            ungetch(*w);
            break;
        }
    *w = '\0';
    return word[0];
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
~~~

*注*

~~~c
#define NKEYS (sizeof keytab / sizeof(struct key))
~~~

也可写为

~~~c
#define NKEYS (sizeof keytab / sizeof keytab[0])
~~~

\#if语句不能使用sizeof因为预处理器不对类型名进行分析，但处理器并不计算、#define中的表达式，所以这样写合法



