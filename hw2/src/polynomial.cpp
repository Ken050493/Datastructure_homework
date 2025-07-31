#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
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
  Polynomial();   // 預設建構子
  ~Polynomial();  // 解構子

  Polynomial Add(const Polynomial& poly) const;   // 加法
  Polynomial Mult(const Polynomial& poly) const;  // 乘法
  float Eval(float f) const;                      // 計算值

  friend istream& operator>>(istream& in, Polynomial& poly);  // 輸入運算子
  friend ostream& operator<<(ostream& out,
                             const Polynomial& poly);  // 輸出運算子
};

// 建構子
Polynomial::Polynomial() {
  capacity = 10;
  terms = 0;
  termArray = new Term[capacity];
}

// 解構子
Polynomial::~Polynomial() { delete[] termArray; }

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
      i++;
      j++;
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
