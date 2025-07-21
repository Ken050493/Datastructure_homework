#include <iostream>
#include <string>
using namespace std;

const int MAX_SIZE = 10;

void generatePowerset(string set[],
                      string current[],
                      int index,
                      int currentSize,
                      int setSize) {
    if (index == setSize) {
        cout << "{ ";
        for (int i = 0; i < currentSize; ++i) {
            cout << current[i] << " ";
        }
        cout << "}" << endl;
        return;
    }

    generatePowerset(set, current, index + 1, currentSize, setSize);

    current[currentSize] = set[index];
    generatePowerset(set, current, index + 1, currentSize + 1, setSize);
}

int main() {
    string set[MAX_SIZE] = {"a", "b", "c"};
    string current[MAX_SIZE];
    int setSize = 3;

    cout << "Powerset of {a, b, c}:" << endl;
    generatePowerset(set, current, 0, 0, setSize);

    return 0;
}
