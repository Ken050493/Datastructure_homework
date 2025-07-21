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
        cout << "錯誤：m 和 n 必須是非負整數" << endl;
        return 1;
    }

    int result = ackermann(m, n);
    cout << "Ackermann(" << m << ", " << n << ") = " << result << endl;

    return 0;
}
