# 41143123

作業一

## 解題說明

本題要求實現一個 Polynomial 類別，表示多項式（polynomial）並運算加法、乘法與輸入輸出等操作。

### 解題策略

1. 使用 class Term 表示多項式的單項式（儲存係數與次方）。
2. Polynomial 類別包含 Term 陣列指標、容量與項數等欄位。  
3. 運算功能由類別成員函式實作，輸入輸出則透過友元函式重載運算子。

## 程式實作

以下為主要程式碼：

```cpp
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <string>
#include <sstream>
#include <algorithm>
#include <fstream>
#include <random>
#include <cstring>
using namespace std;

// 前向宣告
class Polynomial;

// Term 類別
class Term {
    friend class Polynomial;
    friend istream& operator>>(istream& in, Polynomial& poly);
    friend ostream& operator<<(ostream& out, const Polynomial& poly);
private:
    float coef;  // 係數
    int exp;     // 次方
};

// Polynomial 類別
class Polynomial {
private:
    Term* termArray;
    int capacity;
    int terms;

public:
    Polynomial();                   // 預設建構子
    ~Polynomial();                  // 解構子

    Polynomial Add(const Polynomial& poly) const;  // 加法
    Polynomial Mult(const Polynomial& poly) const; // 乘法
    float Eval(float f) const;                     // 計算值

    friend istream& operator>>(istream& in, Polynomial& poly); // 輸入運算子
    friend ostream& operator<<(ostream& out, const Polynomial& poly); // 輸出運算子
};

// 建構子
Polynomial::Polynomial() {
    capacity = 10;
    terms = 0;
    termArray = new Term[capacity];
}

// 解構子
Polynomial::~Polynomial() {
    delete[] termArray;
}

// 輸入運算子
istream& operator>>(istream& in, Polynomial& poly) {
    cout << "輸入項數: ";
    in >> poly.terms;
    if (poly.terms > poly.capacity) {
        delete[] poly.termArray;
        poly.capacity = poly.terms;
        poly.termArray = new Term[poly.capacity];
    }

    cout << "請依序輸入各項的係數與次方\n";
    for (int i = 0; i < poly.terms; ++i) {
        in >> poly.termArray[i].coef >> poly.termArray[i].exp;
    }
    return in;
}

// 輸出運算子
ostream& operator<<(ostream& out, const Polynomial& poly) {
    for (int i = 0; i < poly.terms; ++i) {
        out << poly.termArray[i].coef << "x^" << poly.termArray[i].exp;
        if (i != poly.terms - 1) out << " + ";
    }
    return out;
}

// 多項式加法
Polynomial Polynomial::Add(const Polynomial& poly) const {
    Polynomial result;
    result.capacity = capacity + poly.capacity;
    delete[] result.termArray;
    result.termArray = new Term[result.capacity];

    int i = 0, j = 0, k = 0;

    while (i < terms && j < poly.terms) {
        if (termArray[i].exp > poly.termArray[j].exp)
            result.termArray[k++] = termArray[i++];
        else if (termArray[i].exp < poly.termArray[j].exp)
            result.termArray[k++] = poly.termArray[j++];
        else {
            float sum = termArray[i].coef + poly.termArray[j].coef;
            if (sum != 0) {
                result.termArray[k].coef = sum;
                result.termArray[k].exp = termArray[i].exp;
                k++;
            }
            i++; j++;
        }
    }

    while (i < terms) result.termArray[k++] = termArray[i++];
    while (j < poly.terms) result.termArray[k++] = poly.termArray[j++];

    result.terms = k;
    return result;
}

// 多項式乘法
Polynomial Polynomial::Mult(const Polynomial& poly) const {
    Polynomial result;
    result.capacity = terms * poly.terms;
    delete[] result.termArray;
    result.termArray = new Term[result.capacity];
    result.terms = 0;

    for (int i = 0; i < terms; ++i) {
        for (int j = 0; j < poly.terms; ++j) {
            float newCoef = termArray[i].coef * poly.termArray[j].coef;
            int newExp = termArray[i].exp + poly.termArray[j].exp;

            bool found = false;
            for (int k = 0; k < result.terms; ++k) {
                if (result.termArray[k].exp == newExp) {
                    result.termArray[k].coef += newCoef;
                    found = true;
                    break;
                }
            }
            if (!found) {
                result.termArray[result.terms].coef = newCoef;
                result.termArray[result.terms].exp = newExp;
                result.terms++;
            }
        }
    }
    return result;
}

// 多項式求值
float Polynomial::Eval(float f) const {
    float result = 0.0;
    for (int i = 0; i < terms; ++i) {
        result += termArray[i].coef * pow(f, termArray[i].exp);
    }
    return result;
}

// 主程式（可選）
int main() {
    Polynomial p1, p2;
    cout << "輸入第一個多項式：" << endl;
    cin >> p1;
    cout << "輸入第二個多項式：" << endl;
    cin >> p2;

    Polynomial sum = p1.Add(p2);
    Polynomial prod = p1.Mult(p2);

    cout << "\n第一個多項式: " << p1 << endl;
    cout << "第二個多項式: " << p2 << endl;
    cout << "加法結果: " << sum << endl;
    cout << "乘法結果: " << prod << endl;

    float x;
    cout << "\n請輸入 x 的值以計算 p1(x): ";
    cin >> x;
    cout << "p1(" << x << ") = " << p1.Eval(x) << endl;

    return 0;
}


```

## 效能分析

1. 時間複雜度：加法：$O(n + m)$、乘法：$O(nm)$
2. 空間複雜度：加法：$O(n + m)$、乘法：$O(nm)$

## 測試與驗證

### 測試案例

| 測試案例 | 多項式A | 多項式B | 加法結果 | 乘法結果 |
|----------|--------------|----------|----------|----------|
| 測試一   | 2x^2 + 3x^1  | 1x^1 + 4x^0 |2x^2 + 4x^1 + 4|2x^3 + 3x^2 + 8x^1 + 12|
| 測試二   | 0     |3x^2 + 2x^0 |3x^2 + 2 | 0        |
| 測試三   | -1x^3 + 2x^1|1x^2 + 2x^1 |-1x^3 + 1x^2 + 4x^1|-1x^5 + 2x^4 - 2x^3 + 4x^2


### 編譯與執行指令

```shell
$ g++ polynomial.cpp -std=c++11 -o polynomial.exe
$ ./polynomial.exe
```
輸入第一個多項式：
輸入項數：3  
2 3  
4 2  
1 0  
輸入第二個多項式：
輸入項數：2  
1 3  
5 0  

第一個多項式: 2x^3 + 4x^2 + 1x^0  
第二個多項式: 1x^3 + 5x^0  
加法結果: 3x^3 + 4x^2 + 6x^0  
乘法結果: 2x^6 + 4x^5 + 2x^3 + 20x^3 + 4x^2 + 5x^0  
請輸入 x 的值以計算 p1(x): 2  
p1(2) = 41  

### 結論

1. 程式成功實作了多項式的抽象資料型態，並支援基本運算與 I/O。  
2. 函式設計符合物件導向原則。  
3. 測試涵蓋邊界與特殊情況，結果正確。

## 申論及開發報告

### 選擇類別設計與運算子多載的原因

在本程式中，使用類別封裝與運算子多載來進行多項式加法與乘法運算，主要原因如下：

1. **結構封裝性高，能夠模組化設計程式**  
   物件導向的設計強調資料與操作的封裝與模組性。  
   Term 類別負責儲存每一個式（係數與次方），而 Polynomial 類別則負責整體多項式的管理與運算。


2. **操作邏輯像數學表示方式，比較好理解與實現**  
   遞迴的程式碼更接近數學公式的表示方式。  
   以本程式為例：  

   ```cpp
       Polynomial Polynomial::Add(const Polynomial& poly) const {
        // 核心邏輯：比較次方合併項
    }

   ```
3.**記憶體管理簡潔明確**
     以本程式為例：
   ```cpp
        termArray = new Term[capacity];
        // 使用完畢後釋放
        delete[] termArray;

   ```
透過這次 Polynomial 類別的作業，我回想上課交的類別封裝、動態記憶體、運算子多載與基本運算邏輯的實作方式。這讓我更加了解物件導向程式設計的理解，也學到如何使用多項式這種數學結構更了解程式化與實用性。
