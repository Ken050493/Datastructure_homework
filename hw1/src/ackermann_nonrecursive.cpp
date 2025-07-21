#include <iostream>
#include <string>
using namespace std;

const int MAX_STACK = 100000;

struct StackFrame {
    int m;
    int n;
};

int ackermann_iterative(int m, int n) {
    StackFrame stack[MAX_STACK];
    int top = 0;
    stack[top++] = {m, n};

    while (top) {
        StackFrame frame = stack[--top];
        if (frame.m == 0) {
            n = frame.n + 1;
        } else if (frame.n == 0) {
            stack[top++] = {frame.m - 1, 1};
        } else {
            stack[top++] = {frame.m - 1, -1};
            stack[top++] = {frame.m, frame.n - 1};
        }

        while (top > 0 && stack[top - 1].n == -1) {
            StackFrame waiting = stack[--top];
            stack[top++] = {waiting.m, n};
        }
    }

    return n;
}

int main() {
    int m, n;
    cout << "請輸入 m 和 n：";
    cin >> m >> n;

    if (m < 0 || n < 0) {
        cerr << "錯誤：m 和 n 必須是非負整數" << endl;
        return 1;
    }

    int result = ackermann_iterative(m, n);
    cout << "Ackermann_iterative(" << m << ", " << n << ") = " << result << endl;

    return 0;
}
