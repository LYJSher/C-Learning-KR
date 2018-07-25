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

#### 4-2 对atof函数进行扩充其可以处理123.45e-6的科学表示法，其中，浮点数后面可能会紧跟一个e或E以及一个指数（可能有正负号）

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