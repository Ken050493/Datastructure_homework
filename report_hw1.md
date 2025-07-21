# 41143123

作業一

## 解題說明

本題有兩個問題
1.Akermann函數的遞迴與非遞迴
2.時做一個powerset函式

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

int ackermann(int m, int n)
{
    if (m == 0)
        return n + 1;
    else if (n == 0)
        return ackermann(m - 1, 1);
    else
        return ackermann(m - 1, ackermann(m, n - 1));
}

int main()
{
    int m, n;
    cout << "請輸入 m 和 n：";
    cin >> m >> n;
    try
    {
        if (m < 0 || n < 0)
            throw invalid_argument("m 和 n 需為非負整數");
        int result = ackermann(m, n);
        cout << "Ackermann(" << m << ", " << n << ") = " << result << endl;
    }
    catch (const exception& e)
    {
        cerr << "錯誤：" << e.what() << endl;
    }
    return 0;
}
```
非遞迴版
```cpp
#include <iostream>
#include <stack>
#include <utility>
#include <stdexcept>
using namespace std;

int ackermann_iterative(int m, int n)
{
    stack<pair<int, int>> s;
    s.push({m, n});

    while (!s.empty())
    {
        auto [m1, n1] = s.top();
        s.pop();

        if (m1 == 0)
        {
            n = n1 + 1;
        }
        else if (n1 == 0)
        {
            s.push({m1 - 1, 1});
        }
        else
        {
            s.push({m1 - 1, -1});
            s.push({m1, n1 - 1});
        }
        while (!s.empty() && s.top().second == -1)
        {
            auto m2 = s.top().first;
            s.pop();
            s.push({m2, n});
            break;
        }
    }
    return n;
}
int main()
{
    int m, n;
    cout << "請輸入 m 和 n：";
    cin >> m >> n;
    try
    {
        if (m < 0 || n < 0)
            throw invalid_argument("m 和 n 必須為非負整數");

        int result = ackermann_iterative(m, n);
        cout << "Ackermann_iterative(" << m << ", " << n << ") = " << result << endl;
    }
    catch (const exception& e)
    {
        cerr << "錯誤：" << e.what() << endl;
    }
    return 0;
}

```
## 效能分析

1. 時間複雜度：程式的時間複雜度為  $O(n)$。
2. 空間複雜度：空間複雜度為 $O(n)$。

## 測試與驗證

### 測試案例

| 測試案例 | 輸入參數 $n$ | 預期輸出 | 實際輸出 |
|----------|--------------|----------|----------|
| 測試一   | $m = 0$   $n = 1$     | 2        | 2        |
| 測試二   | $m = 1$   $n = 2$     | 4        | 4        |
| 測試三   | $m = 2$   $n = 2$     | 7        | 7        |
| 測試四   | $m = 3$   $n = 2$     | 29       | 29       |

### 編譯與執行指令

```shell
$ g++ -std=c++17 -o sigma sigma.cpp
$ ./sigma
6
```

### 結論

1. 程式能正確計算 $n$ 到 $1$ 的連加總和。  
2. 在 $n < 0$ 的情況下，程式會成功拋出異常，符合設計預期。  
3. 測試案例涵蓋了多種邊界情況（$n = 0$、$n = 1$、$n > 1$、$n < 0$），驗證程式的正確性。

## 申論及開發報告

### 選擇遞迴的原因

在本程式中，使用遞迴來計算連加總和的主要原因如下：

1. **程式邏輯簡單直觀**  
   遞迴的寫法能夠清楚表達「將問題拆解為更小的子問題」的核心概念。  
   例如，計算 $\Sigma(n)$ 的過程可分解為：  

   $$
   \Sigma(n) = n + \Sigma(n-1)
   $$

   當 $n$ 等於 1 或 0 時，直接返回結果，結束遞迴。

2. **易於理解與實現**  
   遞迴的程式碼更接近數學公式的表示方式，特別適合新手學習遞迴的基本概念。  
   以本程式為例：  

   ```cpp
   int sigma(int n) {
       if (n < 0)
           throw "n < 0";
       else if (n <= 1)
           return n;
       return n + sigma(n - 1);
   }
   ```

3. **遞迴的語意清楚**  
   在程式中，每次遞迴呼叫都代表一個「子問題的解」，而最終遞迴的返回結果會逐層相加，完成整體問題的求解。  
   這種設計簡化了邏輯，不需要額外變數來維護中間狀態。

透過遞迴實作 Sigma 計算，程式邏輯簡單且易於理解，特別適合展示遞迴的核心思想。然而，遞迴會因堆疊深度受到限制，當 $n$ 值過大時，應考慮使用迭代版本來避免 Stack Overflow 問題。
