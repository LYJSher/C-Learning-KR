# Ch3控制流

### 3.3else-if语句

~~~c
# include <stdio.h>

// binsearch：在v[0]<=v[1]<=v[2]<=...<=v[n]中查找x （即升序）
int binsearch(int x, int v[], int n){
    int low, high, mid;

    low = 0;
    high = n - 1;
    while(low <= high){ // 一定要有等号 否则当查找值为首位或末位值时 跳出判断导致错误
        mid = (low + high) / 2;
        if(x < v[mid])
            high = mid - 1;
        else if(x > v[mid])
            low = mid + 1;
        else
            return mid;
    }
    return -1;
}

main(){
    int v[4] = {0,1,2,3};
    printf("%d\n",binsearch(4,v,4));
    printf("%d\n",binsearch(0,v,4));
    printf("%d\n",binsearch(1,v,4));
    printf("%d\n",binsearch(3,v,4));
}
~~~

#### 3-1修改binsearch

~~~c
int binsearch(int x, int v[], int n){
    int low, high, mid;

    low = 0;
    high = n - 1;
    mid = (low + high) / 2;
    while((low<=high) && (x!=v[mid])){
        if(x < v[mid])
            high = mid - 1;
        else
            low = mid + 1;
        mid = (low + high) / 2;
    }
    if(x == v[mid])
        return mid;
    else
        return -1;
}
~~~

两种方案的执行时间基本没有差异，并没有得到很大改进反而失掉了代码的可读性，不提倡修改。

### 3.4switch语句

~~~c
# include <stdio.h>

main(){
    int c, i, nwhite, nother, ndigit[10];

    nwhite = nother = 0;
    for(i=0; i<10; i++)
        ndigit[i] = 0;
    while((c = getchar()) != EOF){
        switch(c){
        case '0': case '1': case '3': case '4': case '5':
        case '6': case '7': case '8': case '9': case '10':
            ndigit[c-'0']++;
            break;
        case ' ':
        case '\n':
        case '\t':
            nwhite++;
            break;
        default:
            nother++;
            break;
        }
    }
    printf("digits=");
    for(i=0; i<10; i++)
        printf("%d: %d    ",i,ndigit[i]);
    printf("\nwhite=%d\nother=%d\n",nwhite,nother);
    return 0;
}
~~~

*注意*

**``case``**后面只能跟常量或**常量表达式**

#### 3-2 编写escape(s,t)将字符串s复制到t中并再复制过程中将换行符和制表符等不可显示字符分别转换为\n、\t等相应的可显示的转义字符序列。在编写一个具有相反功能的函数。

~~~c
# include <stdio.h>
void escape(char s[], char t[]){
    int i,j;
    i = j = 0; // 因为'\'转义字符的位数会不一致 所以分别计数
    while(s[i]!='\0'){ // 字符串只能写'\0' 不能写EOF
        switch(s[i]){
        case '\t':
            t[j++] = '\\';
            t[j++] = 't';
            break;
        case '\n':
            t[j++] = '\\';
            t[j++] = 'n';
            break;
        default:
            t[j++] = s[i];
            break;
        }
        i++;
    }
    t[j] = '\0'; // 不要忘记字符串最后加'\0'
}

// 自己写的版本
void unescape(char s[], char t[]){
    int i,j;
    i = j = 0;
    while(s[i]!='\0'){
        switch(s[i]){
        case '\\':
            if(s[i++]=='n')
                t[j++] = '\n';
            else if(s[i]=='t')// i在上一个判断中已执行过所以条件中为s[i]
                t[j++] = '\t';
            else if(s[i]!='n' && s[i]!='t'){ // 同理
                t[j++] = '\\';
                t[j++] = s[i++];
            }
            break;
        default:
            t[j++] = s[i++];
        }
    }
    t[j] = '\0';
}
// 教材1
void unescape(char s[], char t[]){
    int i,j;
    for(i=j=0; s[i]!='\0'; i++){
        if(s[i] != '\\')
            t[j++] = s[i];
    else
       switch(s[++i]){ // 先自加再引用
            case 'n':
                t[j++] = '\n';
                break;
            case 't':
                t[j++] = '\t';
                break;
            default:
                t[j++] = '\\';
                t[j++] = s[i];
                break;
       }
    }
    t[j] = '\0';
}
// 教材2 switch 可嵌套 还可写为
void unescape(char s[], char t[]){
    int i,j;
    for(i=j=0; s[i]!='\0'; i++){
        switch(s[i]){
        case '\\':
            switch(s[++i]){ // 先自加再引用
            case 'n':
                t[j++] = '\n';
                break;
            case 't':
                t[j++] = '\t';
                break;
            default:
                t[j++] = '\\';
                t[j++] = s[i];
            }
            break;
        default:
            t[j++] = s[i];
            break;
        }
    }
    t[j] = '\0';
}

main(){
    char s[6] = "asd\tf"; // 实际字符为a s d \t f共5位 留出一位自动补'\0'
    /* int i=0;
    while(s[i]!='\0')
        i++;
    printf("%d\n",i); */
    char t[10];
    char tt[10];
    escape(s,t);
    printf("s:%s\nt:%s\n",s,t);
    unescape(t,tt);
    printf("t:%s\ntt:%s\n",t,tt);

    return 0;
}
~~~

*注意*

定义字符串数组时要**多留出来一位** 给系统自动**补'\0'**

因为``'\t'``为一个字符，转换之后变为``'\'``和``'t'``两个字符 所以需要分别计数

不要漏掉``asd\m\tdf``中``\m``不是转义字符应该正常输出的情况

### 3.5while循环和for循环

编写将字符串转换为对应数值的函数atoi，可以处理可选的前导空白符以及一个可选的加号(+)或减号(-)

~~~c
int atoi(char s[]){
    int i, n, sign;
    for(i=0; isspace(s[i]); i++); // 跳过空白符
    sign = (s[i] == '-') ? -1 : 1;
    if(s[i] == '+' || s[i] == '-')
        i++;
    for(n=0; isdigit(s[i]); i++)
        n = 10 * n + (s[i] - '0');
    return sign*n;
}
~~~

***

对整型数组进行排序的Shell排序算法

###**&&Shell排序算法**的基本思想：（缩小增量排序法）

https://www.cnblogs.com/gcczhongduan/p/4593943.html

先比较距离远的元素而不是像简单交换排序算法那样比较相邻的元素

这样可以快速减少大量无序情况，从而减轻后续工作。

被比较的元素之间的距离逐步减少，知道减少为1，这是排序变成了相邻元素的交换

~~~c
// 按递增顺序对v[0]...v[n-1]进行排序
void shellsort(int v[], int n){
    int gap, i, j, temp;
    for(gap=n/2; gap>0; gap/=2) //"gap/=2"表明for循环的控制变量不是算术级数(等差数列)
        for(i=gap; i<n; i++)
            for(j=i-gap; j>=0&&v[j]>v[j+gap]; j-=gap){
                temp = v[j];
                v[j] = v[j+gap];
                v[j+gap] = temp;
            }
}
~~~

最外层for语句控制两个被比较元素之间的距离，从n/2开始，逐步进行对折，知道距离为0

中间层for语句用于在元素间移动位置

最内层for语句用于比较各对相距gap个位置的元素，当这两个元素逆序时把他们互换

***

reverse(s)倒置字符串s中的各个字符的位置

~~~c
void reverse(char s[]){
    int c, i, j;
    
    for(i=0, j=strlen(s); i<j; i++,j--){
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}
~~~

逗号运算符：从左至右顺序求值

~~~c
void reverse(char s[]){
    int c, i, j;
    for(i=0, j=strlen(s); i<j; i++,j--)
        c = s[i], s[i] = s[j], s[j] = c;
}
~~~

####*3-3 编写expand(s1, s2)将字符串s1中类似于a-z一类的速记符号在字符串s2中扩展为等价的完整列表abc...xyz。该函数可以处理大小写字母和数字，并可以处理a-b-c、a-z0-9与-a-z等类似的情况。作为前导和尾随的-字符原样打印。

~~~c
// 自己写的版本
# include <stdio.h>
# include <ctype.h>

// 编写expand(s1, s2)将字符串s1中类似于a-z一类的速记符号在字符串s2中扩展为等价的完整列表abc...xyz。
// 该函数可以处理大小写字母和数字，并可以处理a-b-c、a-z0-9与-a-z等类似的情况。
// 作为前导和尾随的-字符原样打印。
void expand(char s1[], char s2[]){
    int i, j, state;
    char c;
    state = 0; // 初始状态
    for(i=j=0; s1[i]!='\0'; i++){
        switch(state){
        case 0:
            if(isdigit(s1[i]) || isalpha(s1[i]))
                state = 1;
            s2[j++] = s1[i];
            break;
        case 1:
            if(s1[i] == '-')
                state = 2;
            else if(!isdigit(s1[i]) && !isalpha(s1[i])){
                state = 0;
                s2[j++] = s1[i];
            }
            else
                s2[j++] = s1[i];
            break;
        case 2:
            if(isdigit(s1[i]) || isalpha(s1[i]) && s1[i] > s1[i-2]){ // 展开的前位要大于后位
                c = s1[i-2];
                while(c<=s1[i]){
                    s2[j-1] = c++;// 覆盖第一位展开
                    j++;
                }
                j--;
                state = 3;
            }
            else{
                state = 0;
                s2[j++] = s1[i-1];
                s2[j++] = s1[i];
            }
            break;
        case 3:
            if(s1[i] == '-')
                state = 2;
            else if(isdigit(s1[i]) || isalpha(s1[i])){
                s2[j++] = s1[i];
                state = 1;
            }
            else{
                state = 0;
                s2[j++] = s1[i];
            }

        }
    }
    s2[j] = '\0';
}

main(){
    //char a[20] = "a-b-c";
    //char a[20] = "a-z0-9";
    char a[20] = "-a-z";
    char b[100];
    expand(a, b);
    printf("%s\n", b);
    return 0;
}
~~~

~~~c
// 教材
// expand shorthand notation in s1 into s2
void expand(char s1[],char s2[]){
    char c;
    int i,j;

    while((c = s1[i++]) != '\0'){ // fetch a char from s1[]
        if(s1[i] == '-' && s1[i+1] >= c){
            i++;
            while(c < s1[i]) // expand shorthand
                s2[j++] = c++;
        }
        else
            s2[j++] = c; // copy the character
    }
    s2[j] = '\0';
}
~~~

### 3.6do-while循环

itoa函数：将数字n转换为字符串并保存在s中

~~~c
void itoa(int n, char s[]){
    int i, sign;
    
    if((sign = n) < 0) // 记录符号
        n = -n; // 使n成为整数
    i = 0;
    do{ // 以反序生成数字
        s[i++] = n % 10 + '0'; //取下一个数字
    }while((n /= 10) > 0); // 删除该数字
    if(sign < 0)
        s[i++] = '-';
    s[i] = '\0';
    reverse(s);
}
~~~

#### *3-4 在数的对二的补码表示中，如上编写的itoa函数不能处理最大的负数，即n = 

$$
-(2^{字长-1})
$$

#### 的情况。解释原因并修改函数使它在任何机器上运行时都能打印出正确的值。

原因：
$$
-(2^{字长-1})
$$
无法通过语句``n = -n;``转换位一个正数，因为对二的补码所能表示的最大正数只是
$$
(2^{字长-1})-1
$$

~~~c
// 教材
# define abs(x) ((x) < 0 ? -(x) : (x))
void itoa(int n, char s[]){
    int i, sign;
    
    sign = n;
    i = 0;
    do{ // 以反序生成数字
        s[i++] = abs(n % 10) + '0'; //取下一个数字
    }while((n /= 10) != 0); // 删除该数字
    if(sign < 0)
        s[i++] = '-';
    s[i] = '\0';
    reverse(s);
}
~~~

*注*

变量sign无法保存n的初值，所以用宏abs计算n%10的绝对值，此外将``(n /= 10) > 0``改为``(n /= 10) != 0``避免因为n是负数陷入死循环

#### *3-5 编写itob(n,s,b)将整数n转换为以b为底的数并以字符的形式存到字符串s中。例如itob(n,s,16)把整数n格式化成十六进制整数保存在s中

~~~c
# include <stdio.h>
# include <string.h>
# define abs(x) ((x)> 0 ? (x) : -(x))

void reverse(char s[]){
    int i,j;
    char temp;
    j = strlen(s)-1;
    for(i=0; i<j; i++, j--){
        temp = s[i];
        s[i] = s[j];
        s[j] = temp;
    }
}

void itob(int n, char s[], int b){
    int sign, i, j;
    i = 0;
    sign = n;
    n = abs(n);
    do{
        j = n % b;
        s[i++] = (j <= 9) ? j + '0' : j + 'a' - 10; // 注意 
    }while((n /= b) > 0);

    if(sign < 0)
        s[i++] = '-';
    s[i] = '\0';
    reverse(s);
}

main(){
    char s[20];
    int n = 255;
    int b = 16;
    itob(n,s,b);
    printf("%d(10) = %s(%d)\n", n, s, b);
    return 0;
}
~~~

*注意*

````c
s[i++] = (j <= 9) ? j + '0' : j + 'a' - 10;
````

1. 对十六进制超过10的进行字母处理
2. 将小于10的由int型转换为char型

### 3.7break语句与continue语句

trim函数用于删除字符串尾部的空格符、制表符与换行符。

当发现最右边的字符位非空格符、非制表符、非换行符时，使用break语句从循环退出。

~~~c
int trim(char s[]){
    int n;
    for(n = strlen(s) - 1; n >= 0; n--)
        if(s[n] != '' && s[n] != '\t' && s[n] != '\n')
            break;
    s[n+1] = '\0';
    return n;
}
~~~

