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

