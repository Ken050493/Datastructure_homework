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
    Polynomial p, q;
    float x;

    cout << "輸入多項式 p: ";
    cin  >> p;
    cout << "輸入多項式 q: ";
    cin  >> q;
    cout << "輸入 x: ";
    cin  >> x;

    cout << "p="   << p << '\n';
    cout << "q="   << q << '\n';
    cout << "p+q=" << (p+q) << '\n';
    cout << "p-q=" << (p-q) << '\n';
    cout << "p*q=" << (p*q) << '\n';
    cout << "p(" << x << ")=" << p.Evaluate(x) << '\n';
    cout << "q(" << x << ")=" << q.Evaluate(x) << '\n';
}

