# 41143123

作業一

## 解題說明

以 C++ 實作單變數整數係數多項式類別 Polynomial。
操作整數係數的一元多項式
帶表頭節點的循環串列，節點含 coef | exp | link。
刪除節點丟回可用空間鏈以重用。
按照這樣的格式寫出多項式n, c1, e1, c2, e2, c3, e3, …, cn, en
ei:指數、ci:係數、n:多項式項數

### 解題策略

1.用head的循環串列存多項式，節點包含係數與指數。  
2.寫一個 insertTerm(c,e)，維持指數遞減，同指數相加，變 0 就刪掉。  
3.加減：兩串列同時對比，比指數，大的先放，指數相同就相加。  
4.乘法：雙迴圈逐項相乘，結果傳給 insertTerm 合併。  
5.Evaluate：逐項累加 coef * x^exp。  
6.輸出輸入：讀 n 組 (c,e) 逐項插入；輸出為 n c1 e1 ... cn en。  


## 程式實作

以下為主要程式碼：

```cpp
#include <iostream>
#include <cmath>
using namespace std;

class Polynomial {
private:
    struct Node {
        int coef, exp;
        Node* link;
        Node(int c=0, int e=0, Node* l=nullptr): coef(c), exp(e), link(l) {}
    };
    Node* head;                 // circular list header
    static Node* avail;         // available-space list

    static Node* getNode(int c, int e) {
        Node* p;
        if (avail) { p = avail; avail = avail->link; p->coef=c; p->exp=e; p->link=nullptr; }
        else       { p = new Node(c,e,nullptr); }
        return p;
    }
    static void retNode(Node* p) { p->link = avail; avail = p; }

    void clear() {
        Node* p = head->link;
        while (p != head) { Node* q = p; p = p->link; retNode(q); }
        head->link = head;
    }

    void insertTerm(int c, int e) {
        if (c == 0) return;
        Node* prev = head;
        Node* cur  = head->link;
        while (cur != head && cur->exp > e) { prev = cur; cur = cur->link; }
        if (cur != head && cur->exp == e) {
            cur->coef += c;
            if (cur->coef == 0) { prev->link = cur->link; retNode(cur); }
        } else {
            Node* p = getNode(c,e);
            prev->link = p; p->link = cur;
        }
    }

    int termCount() const {
        int cnt = 0; for (Node* p=head->link; p!=head; p=p->link) ++cnt; return cnt;
    }

public:
    Polynomial() { head = getNode(0, 0); head->link = head; }

    Polynomial(const Polynomial& a) : Polynomial() {
        Node* last = head;
        for (Node* p=a.head->link; p!=a.head; p=p->link) {
            Node* t = getNode(p->coef, p->exp);
            last->link = t; last = t;
        }
        last->link = head;
    }

    Polynomial& operator=(const Polynomial& a) {
        if (this == &a) return *this;
        clear();
        Node* last = head;
        for (Node* p=a.head->link; p!=a.head; p=p->link) {
            Node* t = getNode(p->coef, p->exp);
            last->link = t; last = t;
        }
        last->link = head;
        return *this;
    }

    ~Polynomial() { clear(); retNode(head); }

    Polynomial operator+(const Polynomial& b) const {
        Polynomial r; Node* tail=r.head;
        Node *pa=head->link, *pb=b.head->link;
        while (pa!=head && pb!=b.head) {
            if (pa->exp==pb->exp) {
                int c = pa->coef + pb->coef;
                if (c) { Node* t=getNode(c,pa->exp); tail->link=t; tail=t; }
                pa=pa->link; pb=pb->link;
            } else if (pa->exp>pb->exp) {
                Node* t=getNode(pa->coef,pa->exp); tail->link=t; tail=t; pa=pa->link;
            } else {
                Node* t=getNode(pb->coef,pb->exp); tail->link=t; tail=t; pb=pb->link;
            }
        }
        for (; pa!=head; pa=pa->link) { Node* t=getNode(pa->coef,pa->exp); tail->link=t; tail=t; }
        for (; pb!=b.head; pb=pb->link) { Node* t=getNode(pb->coef,pb->exp); tail->link=t; tail=t; }
        tail->link=r.head; return r;
    }

    Polynomial operator-(const Polynomial& b) const {
        Polynomial r;
        for (Node* p=b.head->link; p!=b.head; p=p->link) r.insertTerm(-p->coef, p->exp);
        for (Node* p=head->link; p!=head; p=p->link)   r.insertTerm( p->coef, p->exp);
        return r;
    }

    Polynomial operator*(const Polynomial& b) const {
        Polynomial r;
        for (Node* pa=head->link; pa!=head; pa=pa->link)
            for (Node* pb=b.head->link; pb!=b.head; pb=pb->link)
                r.insertTerm(pa->coef*pb->coef, pa->exp+pb->exp);
        return r;
    }

    float Evaluate(float x) const {
        float s = 0.0f;
        for (Node* p=head->link; p!=head; p=p->link)
            s += p->coef * static_cast<float>(pow(x, static_cast<float>(p->exp)));
        return s;
    }

    friend istream& operator>>(istream& is, Polynomial& x) {
        int n; if (!(is >> n)) return is;
        x.clear();
        for (int i=0;i<n;++i){ int c,e; is>>c>>e; x.insertTerm(c,e); }
        return is;
    }
    friend ostream& operator<<(ostream& os, const Polynomial& x) {
        os << x.termCount();
        for (Node* p=x.head->link; p!=x.head; p=p->link) os<<' '<<p->coef<<' '<<p->exp;
        return os;
    }
};

Polynomial::Node* Polynomial::avail = nullptr;

int main() {
    Polynomial p,q;
    cin >> p >> q;
    cout << "p=" << p << '\n';
    cout << "q=" << q << '\n';
    cout << "p+q=" << (p+q) << '\n';
    cout << "p-q=" << (p-q) << '\n';
    cout << "p*q=" << (p*q) << '\n';
    cout << "p(2)=" << p.Evaluate(2.0f) << '\n';
    cout << "q(2)=" << q.Evaluate(2.0f) << '\n';
}

```

## 效能分析

時間複雜度：O((mn)^2)。
空間複雜度：O(mn)。


## 測試與驗證

### 測試案例

| 測試案例 | 輸入參數 $n$ | 預期輸出 | 實際輸出 |
|----------|--------------|----------|----------|
| 測試一   | p:0、q:0      |p=0；q=0；p+q=0；p-q=0；p*q=0 |相同|
| 測試二   | p:3 3 4 2 2 -5 0 q:3 -1 3 4 2 7 0      |p=3 3 4 2 2 -5 0；q=3 -1 3 4 2 7 0；p+q=4 3 4 -1 3 6 2 2 0；p-q=4 3 4 1 3 -2 2 -12 0；p*q=7 -3 7 12 6 -2 5 29 4 5 3 -6 2 -35 0|相同 |
| 測試三   | 依上提帶入x=2      |p(2)=51；q(2)=15|p(2)=51；q(2)=15|
| 測試四   |p：3 5 5 -5 5 1 0 q:0|p=1 1 0；q=0；p+q=1 1 0；p-q=1 1 0；p*q=0|相同|
| 測試五   |p:1 5 0 q:1 -2 0|p=1 5 0；q=1 -2 0；p+q=1 3 0；p-q=1 7 0；p*q=1 -10 0| 相同 |

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
