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

#### 6-1 上诉getword函数不能正确处理下划线、字符串常量、注释及预处理控制器指令。请编写一个更完善的getword函数

~~~c
// 从输入中读取下一个单词或字符
int getword(char *word, int lim){
    int c, d, getch(void), comment(void);
    void ungetch(int);
    char *w = word;

    while(isspace(c = getch()));
    
    if(c != EOF)
        *w++ = c;
        
    // 处理下划线和与编译器控制指令    
    if(isalpha(c) || c=='_' || c=='#'){ // 第一个字符是字母数字下划线或# 
        for(; --lim>0; w++)
            if(!isalnum(*w = getch()) && *w != '_'){
                ungetch(*w);
                break;
            }
    }
    else if(c == '\'' || c == '\"'){ // 处理字符串常量
        for(; --lim>0; w++)
            if((*w = getch()) == '\\') /* 若出现\ 往后多读一个\ */
                *++w = getch();
            else if(*w == c){
                w++;
                break;
            }
            else if(*w == EOF)
                break;
    }
    else if(c == '\\')
        if((d = getch()) == '*')
            c = comment();
        else 
            ungetch(d);
    *w = '\0';
    return c;
}

int comment(void){
    int c;
    while((c = getch()) != EOF)
        if(c == '*')
            if((c = getch()) == '\\')
                break;
            else
                ungetch(c);
    return c;
}
~~~

### 6.4 指向结构的指针

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
struct key *binsearch(char *, struct key *, int);

// 统计关键字的出现次数：采用指针方式的版本
main(){
    char word[MAXWORD];
    struct key *p;

    while(getword(word, MAXWORD) != EOF){
        if(isalpha(word[0]))
            if((p = binsearch(word, keytab, NKEYS)) != NULL)
                p->count++;
    }
    for(p=keytab; p<keytab+NKEYS; p++)
        if(p->count > 0)
            printf("%4d %s\n", p->count, p->word);
    return 0;
}

// 在tab[0]到tab[n-1]中查找与读入单词匹配的元素
struct key *binsearch(char *word, struct key *tab, int n){
    int cond;
    struct key *low = &tab[0]; // low初值指向表头元素的指针
    struct key *high = &tab[n]; // high初值指向表未元素后面一个元素的指针
    
    while(low <= high){
        mid = low + (high-low)/2; // 注 high-low得到之间的元素个数
        if((cond = strcmp(word, tab->word)) < 0)
            high = mid;
        else if(cond > 0)
            low = mid + 1;
        else
            return mid;
    }
    return NULL;
}
~~~

*注*

无法通过下列表达式计算中间元素的位置

~~~c
mid = (low + high) / 2; // 错误
~~~

因为两个指针之间的加法运算是非法的，但是**指针的减法运算却是合法的（指向同一个数组），high-low的值就是数组元素的个数**

&tab[-1] 和 &tab[n] 都超过了数组tab的范围，前者绝对非法，后者的间接引用也是非法的

**C语言的定义保证数组末尾之后的第一个元素（即&tab[n]）的指针算术运算可以正确执行**



结构的长度不一定等于各成员长度的和，因为不同的对象有不同的对齐要求，所以结构中可能会出现未命名的“空穴”（hole），假设char类型占用1个字节，int类型占用4个字节，则下列结构

~~~c
struct{
	char c;
    int i;
};
~~~

可能需要8个字节的存储空间，而不是5个字节，使用sizeof运算符可以返回正确的对象的长度

### 6.5 自引用结构

统计输入中所有单词的出现次数

~~~c
struct tnode{ // 树的节点
    char *word; // 指向单词的指针
    int count; // 单词出现的次数
    struct tnode *left; // 左子节点
    struct tnode *right; // 右子节点
};
~~~

两个结构互相引用

~~~c
struct t{
	struct s *p; // p指向一个s结构
};

struct s{
    struct t *q; // q指向一个t结构
}；
~~~

~~~c
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define MAXWORD 100

struct tnode{ // 树的节点
    char *word; // 指向单词的指针
    int count; // 单词出现的次数
    struct tnode *left; // 左子节点
    struct tnode *right; // 右子节点
};

struct tnode *talloc(void);
char *strdup_(char *s);
struct tnode *addtree(struct tnode *, char *); // 将单词插入树中
void treeprint(struct tnode *);
int getword(char *, int); // 读入单词

// 单词出现频率的统计
main(){
    struct tnode *root;
    char word[MAXWORD];

    root = NULL;
    while(getword(word, MAXWORD) != EOF)
        if(isalpha(word[0]))
            root = addtree(root, word);
    treeprint(root);
    return 0;
}

// addtree在p的位置或p的下方增加一个节点
struct tnode *addtree(struct tnode *p, char *w){
    int cond;

    if(p == NULL){ // 该单词是一个新单词
        p = talloc(); // 创建一个新节点
        p->word = strdup(w);
        p->count = 1;
        p->left = p->right = NULL;
    }
    else if((cond = strcmp(w, p->word)) == 0) // 新单词与节点中的单词匹配
        p->count++;
    else if(cond < 0)
        p->left = addtree(p->left, w); // 新单词小于节点单词，进入左子树
    else
        p->right = addtree(p->right, w); // 新单词大于节点单词，进入右子树
    return p;
}

// 按顺序打印树p
void treeprint(struct tnode *p){
    if(p != NULL){
        treeprint(p->left);
        printf("%4d %s\n", p->count, p->word);
        treeprint(p->right);
    }
}

// 创建一个tnode
struct tnode *talloc(void){
    return (struct tnode *) malloc(sizeof(struct tnode));
}

// 把通过其参数传入的字符串复制到某个安全的位置
char *strdup_(char *s){
    char *p;
    p = (char *)malloc(strlen(s) + 1);
    if(p != NULL)
        strcpy(p, s);
    return p;
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

#### 6-2 用以读入一个C语言程序，并按字母表顺序分组打印变量名，要求每一组内各变量名的前6个字符相同，其余字符不同。字符串和注释中的单词不予考虑。请将6作为一个可在命令行中设定的参数。

~~~

~~~

#### 6-3 编写一个交叉引用程序，打印文档中所有单词的列表，并且每个单词还有一个列表，记录出现过的该单词的行号。对the、and等非实义单词不予考虑。

#### 6-4 根据单词的出现频率按降序打印输入的各个不同单词，并在每个单词的前面标上它的出现次数

#### 6-5 编写函数undef，它将从由lookup和install维护的表中删除一个变量名及其定义

#### 6-6 以本节介绍的函数为基础，编写一个适合C语言程序使用的#define处理器的简单版本（即无参数的情况）。你会发现getch和ungetch函数非常有用。



