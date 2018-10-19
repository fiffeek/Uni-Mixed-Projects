#include <iostream>
#include <vector>
#include <set>

using namespace std;

int main() {
    long long n, m;
    cin >> n >> m;

    long long arr[n];

    for (int i = 0; i < n; ++i) {
        arr[i] = 1;
    }

    for (long long i = 0; i < m; ++i) {
        for (long long j = 0; j < n; ++j) {
            int aux;
            cin >> aux;

            arr[aux - 1] *= 2;
            if (j >= n / 2) {
                arr[aux - 1] += 1;
            }
        }
    }

    set<long long> s(arr, arr + n);

    if (s.size() == n) {
        cout << "TAK" << endl;
    } else {
        cout << "NIE" << endl;
    }
}