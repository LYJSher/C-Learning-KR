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