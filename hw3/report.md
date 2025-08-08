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
| 測試一   | p:0、q:0      |p=0 q=0 p+q=0  p-q=0 p*q=0 |相同|
| 測試二   | p:3 3 4 2 2 -5 0  q:3 -1 3 4 2 7 0  |p=3 3 4 2 2 -5 0  q=3 -1 3 4 2 7 0  p+q=4 3 4 -1 3 6 2 2 0  p-q=4 3 4 1 3 -2 2 -12 0  p*q=7 -3 7 12 6 -2 5 29 4 5 3 -6 2 -35 0|相同 |
| 測試三   | 依上提帶入x=2      |p(2)=51  q(2)=15|p(2)=51  q(2)=15|
| 測試四   |p：3 5 5 -5 5 1 0  q:0|p=1 1 0 q=0  p+q=1 1 0 p-q=1 1 0 p*q=0|相同|
| 測試五   |p:1 5 0   q:1 -2 0|p=1 5 0 q=1 -2 0 p+q=1 3 0  p-q=1 7 0 p*q=1 -10 0| 相同 |

### 編譯與執行指令

```shell
$ g++ -std=c++17 -o sigma sigma.cpp
$ ./sigma
6
```

### 結論

1. 輸入、輸出、加法、減法、乘法與 Evaluate都能執行
2. 能進行兩個帶有head的多項式運算
3. 零、同次合併後係數相消等邊界情況皆正確，預計的跟計算後的一樣。

## 申論及開發報告

### 選擇遞迴的原因

選擇資料結構的原因:  

1.使用帶表頭的循環串列表示多項式。表頭作哨兵，空表與首尾插刪處理一致。  
2.節點僅含三欄：coef | exp | link。指數維持嚴格遞減，走訪與合併直觀。  

易於維護與理解:  

1.設計單一寫入入口 insertTerm(c,e)，統一排序與合併規則。  
規則：遇同指數相加，結果為 0 立即刪除。輸入皆為(c,e)格式 。  
```cpp

void insertTerm(int c, int e){
    if(!c) return;
    // 找到第一個 exp <= e 的位置
    // exp 相同 -> 相加；為 0 -> 刪除
    // 否則在有序位置插入
    }

```
使用head能去除邊界分支讓錯誤率下降，使用空間鏈降低配置成本，但會積累記憶體，要在程式最後要結束時加上清理的程式，乘法時項數太大時複雜度的成長太大了。
