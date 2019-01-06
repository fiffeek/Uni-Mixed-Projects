#include <iostream>
#include <vector>
#include <unordered_set>

using namespace std;

int main() {
    int n, d;
    cin >> n >> d;
    vector<int> numbers;
    unordered_set<int> exists;

    while (n--) {
        int aux;
        cin >> aux;

        numbers.push_back(aux);
    }

    for (const int number: numbers) {
        if (exists.count(number - d)) {
            cout << number << " " << number - d << endl;
            return 0;
        } else if (exists.count(number + d)) {
            cout << number + d << " " << number << endl;
            return 0;
        }

        exists.insert(number);
    }

    cout << "NIE" << endl;
}
