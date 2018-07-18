#Ch2 类型、运算符与表达式

### 2.1变量名

ANSI标准规定，标识符可以为任意长度，但**外部名必须至少能由前6个字符唯一地区分，并且不区分大小写**。这里**外部名指的是在链接过程中所涉及的标识符，其中包括文件间共享的函数名和全局变量名**。因此外部名abcdefgh和abcdef将被当作同一个标识符处理。

ANSI标准还规定**内部名必须至少能由前31个字符唯一地区分**。**内部名指的是仅出现于定义该标识符的文件中的那些标识符**。C语言中的字母是有大小写区别的，因此count Count COUNT是三个不同的标识符。标识符不能和C语言的关键字相同，也不能和用户已编制的函数或C语言库函数同名。

#### *2-1确定分别由signed和unsigned限定的char、short、int与long类型变量的取值范围。采用打印标准头文件中的相应值以及直接计算两种方式实现

~~~c
# include <stdio.h>
# include <limits.h>

// determine ranges of types
main(){
    printf("利用头文件\n");
    // signed types
    printf("signed char  min = %d\n", SCHAR_MIN);
    printf("signed char  max = %d\n", SCHAR_MAX);
    printf("signed short min = %d\n", SHRT_MIN);
    printf("signed short max = %d\n", SHRT_MAX);
    printf("signed int   min = %d\n", INT_MIN);
    printf("signed int   max = %d\n", INT_MAX);
    printf("signed long  min = %d\n", LONG_MIN);
    printf("signed long  max = %d\n", LONG_MAX);
    // unsigned types
    printf("unsigned char  max = %u\n", UCHAR_MAX);
    printf("unsigned short max = %u\n", USHRT_MAX);
    printf("unsigned int   max = %u\n", UINT_MAX);
    printf("unsigned long  max = %u\n", ULONG_MAX);
    printf("按位运算\n");
    // signed types
    printf("signed char  min = %d\n", -(char)((unsigned char)~0 >> 1) - 1);
    printf("signed char  max = %d\n", (char)((unsigned char)~0 >> 1));
    printf("signed short min = %d\n", -(short)((unsigned short)~0 >> 1) - 1);
    printf("signed short max = %d\n", (short)((unsigned short)~0 >> 1));
    printf("signed int   min = %d\n", -(int)((unsigned int)~0 >> 1) - 1);
    printf("signed int   max = %d\n", (int)((unsigned int)~0 >> 1));
    printf("signed long  min = %d\n", -(long)((unsigned long)~0 >> 1) - 1);
    printf("signed long  max = %d\n", (long)((unsigned long)~0 >> 1));
    // unsigned types
    printf("unsigned char  max = %u\n", (unsigned char)~0);
    printf("unsigned short max = %u\n", (unsigned short)~0);
    printf("unsigned int   max = %u\n", (unsigned int)~0);
    printf("unsigned long  max = %u\n", (unsigned long)~0);
}
~~~

按位运算计算方法 以signed int 为例

1. ``~0``把数字0的各个二进制位全部转换为1

2. ``(unsigned int)~0``将结果值转换为unsigned int类型

3. ``(unsigned int)~0 >> 1``**把这个unsigned int类型右移一位清除符号位**

4. ``(int)(unsigned int)~0 >> 1``最后转换为int类型

   *注意*  在计算min时 因为0只占一个 所以最小值多一项-1

### 2.3常量

字符**'0'**的值为48与数值0无关

字符**‘\0’**表示值为0的字符，也就是空字符(null) 我们通常用**‘\0’**的形式替代0以强调某些表达式的字符属性，但其数字值为0



**字符常量**与**仅包含一个字符的字符串**之间的区别：（如**‘x’**和**"x"**）

**字符常量**是一个整数，其值是字母x在机器字符集中对应的数值（内部表示值）

**仅包含一个字符的字符串**是一个包括一个字符（即字母x）以及一个结束符‘\0’的**字符数组**



**枚举**为建立常量值与名字之间的关联提供了一种便利的方法。相对于#define语句来说，他的优势在于常量值可以自动生成

### 2.4声明

const限定符可配合数组参数使用表明函数**不能修改数组元素**的值``int strlen(const char[])``

**如果试图修改const限定符限定的值其结果取决于具体的实现**   *~~（编译原理指向静态内容的静态指针啥的？）~~*

###2.6关系运算符与逻辑运算符

#### 2-2不使用&&和||的条件下编写等价代码

~~~c
for(i=0; i<lim-1 && (c=getchar())!=EOF && c!='\n'; ++i)
    s[i] = c;
~~~

~~~c
enum loop {NO, YES};
enum loop okloop = YES;

i = 0;
while(okloop){
    if(i >= lim-1);
    	okloop = NO;
    else if((c = getchar()) == EOF)
    	okloop = NO;
    else if((c = getchar()) == '\n')
    	okloop = NO;
    else{
        s[i] = c;
        i++;
    }
}
~~~

### 2.7类型转换

atoi函数：将字符串s转换为相应的整形数 （如将字符型的“123”转换为整形的123）

~~~c
int atoi(char s[]){
    int i, n;
    n=0;
    for(i=0; s[i]>='0' && s[i]<='9'; i++)
        n = 10 * n + ( s[i] - '0' );
    return n;
}
~~~



~~~c
int i;
char c;

i = c;
c = i;
~~~

执行后c的值不变，但若把两个赋值语句次序颠倒一下则执行后可能会丢失数据

#### 2-3编写函数htoi(s)，把十六进制数字组成的字符串（包含可选的前缀0x或0X）转换为与之等价的整型值。字符串中允许包含的数字包括：0~9，a~f以及A~F

~~~c
// 自己写的版本
# include <stdio.h>
// 编写函数htoi(s)，把十六进制数字组成的字符串（包含可选的前缀0x或0X）
// 转换为与之等价的整型值。字符串中允许包含的数字包括：0~9，a~f以及A~F
int htoi(char s[]){
    int i, n, isHex;
    n = 0;
    i = 0;
    isHex = 1; // 在十六进制表示数范围内
    if(s[i] == '0'){
        i++;
        if(s[i] == 'X' || s[i] == 'x')
            i++;
    }
    while(isHex){
        if(s[i] >= '0' && s[i] <= '9')
            n = 16 * n + (s[i] - '0');
        else if(s[i] >= 'a' && s[i] <= 'f')
            n = 16 * n + (s[i] - 'a' + 10);
        else if(s[i] >= 'A' && s[i] <= 'F')
            n = 16 * n + (s[i] - 'A' + 10);
        else
            isHex = 0;
        ++i;
    }
    return n;
}

main(){
    char s[100];
    char c;
    int i;
    for(i=0; (c = getchar())!=EOF; i++)
        s[i] = c;
    printf("%d",htoi(s));
}
~~~

~~~c
// 教材
# include <stdio.h>
# define YES 1
# define NO 0
// htol: convert hexadecimal strings to integer
int htoi(char s[]){
    int i, n, inhex, hexdigit;

    i = 0;
    if(s[i] == '0'){ // skip optional 0x or 0X
        i++;
        if(s[i] == 'X' || s[i] == 'x')
            i++;
    }
    n = 0; // integer value to be returned
    inhex = YES; // assume valid hexadecimal digit
    for(; inhex == YES; ++i){
        if(s[i] >= '0' && s[i] <= '9')
            hexdigit = s[i] - '0';
        else if(s[i] >= 'a' && s[i] <= 'f')
            hexdigit = s[i] - 'a' + 10;
        else if(s[i] >= 'A' && s[i] <= 'F')
            hexdigit = s[i] - 'A' + 10;
        else
            inhex = NO; // not a valid hexadecimal digit
        if(inhex == YES)
            n= 16 * n + hexdigit;
    }
    return n;
}

main(){
    char s[100];
    char c;
    int i;
    for(i=0; (c = getchar())!=EOF; i++)
        s[i] = c;
    printf("%d",htoi(s));
}
~~~

### 2.8自增运算符与自减运算符

~~~c
// strcat：将字符串t连接到字符串s的尾部，s必须有足够大的空间
void strcat(char s[], char t[]){
    int i, j;
    i = j = 0;
    while(s[i] != '\0')
        i++;
    while((s[i++] = t[j++]) != '\0');
}
~~~

*注意*

**n++是先使用变量n的值，然后再将n的值+1**

#### 2-4编写aqueeze(s1,s2)，将字符串s1中任何与字符串s2中字符匹配的字符都删除

~~~c
# include <stdio.h>

// 编写aqueeze(s1,s2)，将字符串s1中任何与字符串s2中字符匹配的字符都删除
void squeeze(char s1[], char s2[]){
    int i,j,k;

    for(i=0; s2[i]!='\0'; i++){
        for(j=0, k=0; s1[j]!='\0'; j++){
            if(s1[j] != s2[i]){
                s1[k++] = s1[j];
            }
        }
        s1[k] = '\0'; // 不能忘
    }
}

main(){
    char s1[10] = "asdfasdf";
    char s2[10] = "ad";
    squeeze(s1,s2);
    printf("%s",s1);
}
~~~

*注意*

``s1[k] = '\0';``保证每次循环之后正常结束

####2-5编写any(s1,s2),将字符串s2中的任一字符在字符串s1中出现的位置作为结果返回。如果s1中不包括s2中的字符，则返回-1。(标准库函数strpbrk具有同样的功能，但它返回的是指向该位置的指针)

~~~c
# include <stdio.h>

// 编写any(s1,s2),将字符串s2中的任一字符在字符串s1中出现的位置作为结果返回。
// 如果s1中不包括s2中的字符，则返回-1。(标准库函数strpbrk具有同样的功能，但它返回的是指向该位置的指针)
int any(char s1[], char s2[]){
    int i,j,k;

    for(i=0; s1[i]!='\0'; i++){
        for(j=0; s2[j]!='\0'; j++){
            if(s1[i] == s2[j]){
                return i+1;
            }
        }
    }
    return -1;
}

main(){
    char s1[10] = "asdfasdf";
    char s2[10] = "d";
    printf("%d",any(s1,s2));
}
~~~

### 2.9按位运算符

getbits函数：返回x中从第p位开始的n位

``x >> (p+1-n)``将期望获得的字段移位到字的最右端

``~0``的所有位都为1，这里使用语句``~0 << n``将~0左移n位，并将最右边的n位用0填补

再使用~运算对他进行按位取反，这样就建立了最右边n位全为1的屏蔽码

~~~ c
unsigned getbits(unsigned x, int p, int n){
    return (x >> (p+1-n) & ~(~0 << n));
}
~~~

#### 2-6* 编写setbits(x, p, n, y) 返回对x执行下列操作后的结果值，将x中从第p位开始的n个（二进制）位设置为y中最右边n位的值， x的其余各位保持不变

~~~c
// setbits: set n bits of x at position p with bits of y
int setbits(unsigned x, int p, int n, unsigned y){
    return (x & ~(~(~0 << n) << (p + 1 - n)) | (y & ~(~0 << n)) << (p + 1 - n))
}
~~~

*注意*

移动位置位（p + 1 - n）可画图理解

#### 2-7*编写invet(x, p, n) 返回对x执行下列操作后的结果值, 将x中从第p位开始的n个（二进制）位求反（即1变0，0变1）x的其余各位保持不变

~~~c
// invert: inverts the n bits of x that begin at position p
int invert(unsigned x, int p, int n){
    return (x ^(~(~0 << n) << (p-n+1)))
}
~~~

*注意*

``^``异或操作 不同为1 相同为0

#### 2-8*编写rightrot(x,  n) 返回将x循环右移（即从最右端移出的位将从最左端移入）n（二进制）位后所得到的结果值

~~~c
// rightrot: rotate x to the right by n positions
// 方案一
unsigned rightrot(unsigned x, int n){
    int rbit; // rightmost
    int wordlength(void);

    while(n--){
        rbit = (x & 1) << (wordlength() - 1);
        x >> 1; // shift x 1 position right
        x = x | rbit; // complete one rotation
    }
    return x;
}

// 方案二
unsigned rightrot(unsigned x, int n){
    int rbits;
    int wordlength(void);

    if((n = n % wordlength()) > 0){
        rbits = ~(~0 << n) & x; // n rightmost bits of x
                               // n rightmost bits of left
        rbits = rbits << (wordlength() - n) // 注意和上一题区别 移几位草稿纸上画有一下
        x = x >> n;
        x = x | rbits;
    }
    return x;
}

// wordlength: computes word length of the machine
int wordlength(void){
    int i;
    unsigned v = (unsigned) ~0;

    for(i=1; (v = v >> 1)>0; i++);

    return i;
}
~~~

*注意*

wordlength()计算出运行程序计算机所使用的字长

如果对x进行循环右移的总位数（n）与一个无符号整数的二进制位数（即这台电脑的字长）相等，完成这些次循环右移后的结果将与x完全一样，一次不必再对x进行循环右移

如果n小于这台机器字长，必须将x循环右移n位

如果n大于这台机器字长，需利用取模运算符求出n对这台计算机的字长的余数，再把x循环右移这个余数所代表的次数

### 2.10赋值运算符与表达式

统计整形参数的值为1的二进制位的个数

~~~c
int bitcount(unsigned x){
    int b;
    for(b=0; x!=0; x>>=1)
    	if(x & 01)
    		b++;
    return b;
}
~~~

####2-9 在求对二的补码时，表达式``x&=(x-1)``可以删除x中最右边值为1的一个二进制位，用这一方法重写bitcount函数以加快执行速度

~~~c
int bitcount(unsigned x){
	int b;
    for(b=0; x!=0; x&=(x-1))
        b++;
    return b;
}
~~~

习题解答p35

### 2.11条件表达式

循环打印一个数组的n个元素，每行打印10个元素，每列用一个空格隔开，每行用一个换行符结束(包括最后一行)

~~~
for(i=0; i<n; i++)
	printf("%6d%c", a[i]，(i%10==9 || i==n-1)? '\n' : ' ')
~~~

### 2.12运算符优先级与求值顺序

```c
printf("%d%d\n", ++n, power(2,n)); // 错
```

**在不同编译器中可能产生不同结果，这取决于n的自增运算再power调用之前还是之后执行。**

解决方法

~~~c
++n;
printf("%d%d\n", n, power(2,n));
~~~

