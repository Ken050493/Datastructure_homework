# 41143123

作業一

## 解題說明

本題有兩個問題  
1.Akermann函數的遞迴與非遞迴。  
2.時做一個powerset函式。  

### 解題策略
第一題  
1. 根據Ackermann定義寫程式碼。  
2. 實作非遞迴使用堆疊模擬呼叫過程。  

第二題  
1. 使用遞迴拆成小問題。

## 程式實作

以下為第一題程式碼：

遞迴版  

```cpp

#include <iostream>

using namespace std;

int ackermann(int m, int n) {
    if (m == 0)
        return n + 1;
    else if (n == 0)
        return ackermann(m - 1, 1);
    else
        return ackermann(m - 1, ackermann(m, n - 1));
}

int main() {
    int m, n;
    cout << "請輸入 m 和 n：";
    cin >> m >> n;

    if (m < 0 || n < 0) {
        cout << "錯誤：m 和 n 必須是非負整數。" << endl;
        return 1;
    }

    int result = ackermann(m, n);
    cout << "Ackermann(" << m << ", " << n << ") = " << result << endl;

    return 0;
}

```  
        
非遞迴版
```cpp
#include <iostream>
#include <string>
#include <cstdlib>
using namespace std;

const int MAX_STACK = 100000;
struct StackFrame 
{
    int m;
    int n;
};
int ackermann_iterative(int m, int n) 
{
    StackFrame stack[MAX_STACK];
    int top = 0;
    stack[top++] = {m, n};
    while (top > 0) 
	{
        StackFrame frame = stack[--top];
        if (frame.m == 0) 
		{
            n = frame.n + 1;
        } 
		else if (frame.n == 0) 
		{
            stack[top++] = {frame.m - 1, 1};
        } 
		else 
		{
            stack[top++] = {frame.m - 1, -1};
            stack[top++] = {frame.m, frame.n - 1};
        }
        while (top > 0 && stack[top - 1].n == -1) 
		{
            StackFrame waiting = stack[--top];
            stack[top++] = {waiting.m, n};
        }
    }
    return n;
}

int main() 
{
    int m, n;
    cout << "請輸入 m 和 n：";
    cin >> m >> n;
    if (m < 0 || n < 0) 
	{
        cerr << "錯誤：m 和 n 必須為非負整數" << endl;
        return 1;
    }
    int result = ackermann_iterative(m, n);
    cout << "Ackermann_iterative(" << m << ", " << n << ") = " << result << endl;
    return 0;
}


```

以下為第二題程式碼：
```cpp
#include <iostream>
#include <string>
using namespace std;

const int MAX_SIZE = 10; 
void generatePowerset(string set[], string current[], int index, int currentSize, int setSize) 
{
    if (index == setSize) 
	{
        cout << "{ ";
        for (int i = 0; i < currentSize; ++i) 
		{
            cout << current[i] << " ";
        }
        cout << "}" << endl;
        return;
    }
    generatePowerset(set, current, index + 1, currentSize, setSize);
    current[currentSize] = set[index];
    generatePowerset(set, current, index + 1, currentSize + 1, setSize);
}

int main() 
{
    string S[MAX_SIZE] = {"a", "b", "c"};
    string current[MAX_SIZE];
    int setSize = 3;
    cout << "Powerset of {a, b, c}:" << endl;
    generatePowerset(S, current, 0, 0, setSize);
    return 0;
}


```

## 效能分析
第一題:
1. 時間複雜度：程式的時間複雜度為  $O(n)$。
2. 空間複雜度：空間複雜度為 $O(n)$。

第二題:
1. 時間複雜度：程式的時間複雜度為  $O(n \cdot 2^n)$。
2. 空間複雜度：空間複雜度為 $O(n)$。

## 測試與驗證

### 測試案例
**Akermann函數:**

| 測試案例 | 輸入參數 $n$ | 預期輸出 | 實際輸出 |
|----------|--------------|----------|----------|
| 測試一   | $m = 0$   $n = 1$     | 2        | 2        |
| 測試二   | $m = 1$   $n = 2$     | 4        | 4        |
| 測試三   | $m = 2$   $n = 2$     | 7        | 7        |
| 測試四   | $m = 3$   $n = 2$     | 29       | 29       |
| 測試五   | $m = 4$   $n = 1$     | 29       | 崩潰      |

**Powerset:**
Powerset of {a, b, c}:  
{}  
{ c }  
{ b }  
{ b c }  
{ a }  
{ a c }  
{ a b }  
{ a b c }  
### 編譯與執行指令

```shell
$ g++ -std=c++17 -o sigma sigma.cpp
$ ./sigma
6
```

### 結論
第一題:
1. 程式能根據輸入的m、n計算akermann函數的值。  
2. 在 $m < 0$或$n < 0$ 的情況下，程式會提醒m、n要大於等於0。
   
第二題:
1. 能正確顯示出所有的子集合。  


## 申論及開發報告

### Akermann函數遞迴與非遞迴的選擇:  
1.**空間需求**  
在遞迴時每次ackermann()都會呼叫新的stack frame，所以當遞迴過多時資源容易耗盡。  

2.**簡單理解**  
從程式邏輯來看**遞迴版**更容易看懂。  
以本題為例: 
```cpp
int main()
{
    int m, n;
    cout << "請輸入 m 和 n：";
    cin >> m >> n;
    if (m < 0 || n < 0)
    {
        cout << "錯誤：m 和 n 必須是非負整數。" << endl;
        return 1;
    }
    int result = ackermann(m, n);
    cout << "Ackermann(" << m << ", " << n << ") = " << result << endl;
    return 0;
}
```  

3.**非遞迴的優點**:可以處理較大輸入  
以本題為例:
```cpp
int ackermann_iterative(int m, int n) 
{
    StackFrame stack[MAX_STACK];
    int top = 0;
    stack[top++] = {m, n};
    while (top > 0) 
	{
        StackFrame frame = stack[--top];
        if (frame.m == 0) 
		{
            n = frame.n + 1;
        } 
		else if (frame.n == 0) 
		{
            stack[top++] = {frame.m - 1, 1};
        } 
		else 
		{
            stack[top++] = {frame.m - 1, -1};
            stack[top++] = {frame.m, frame.n - 1};
        }
        while (top > 0 && stack[top - 1].n == -1) 
		{
            StackFrame waiting = stack[--top];
            stack[top++] = {waiting.m, n};
        }
    }
    return n;
}

```  


