#include <iostream>
#include <vector>

using namespace std;

#define N 1000
vector<int> v;

int main() {
    long long MOD = 1000000000;

    size_t n;
    cin >> n;

    while (n--) {
        long long aux;
        cin >> aux;

        v.emplace_back(aux);
    }

    n = v.size();
    long long rememeber[N][N][2]{0};

    for (int i = 0; i < n; ++i) {
        rememeber[i][i][0] = 1;
    }

    for (int len = 0; len < n; ++len) {
        for (int start = 0; start < n - len; ++start) {
            int end = start + len;

            if (start + 1 < n && v[start] < v[start + 1]) {
                rememeber[start][end][0] += rememeber[start + 1][end][0];
                rememeber[start][end][0] %= MOD;
            }

            if (v[end] > v[start]) {
                rememeber[start][end][0] += rememeber[start + 1][end][1];
                rememeber[start][end][0] %= MOD;
            }

            if (end - 1 >= 0 && v[end - 1] < v[end]) {
                rememeber[start][end][1] += rememeber[start][end - 1][1];
                rememeber[start][end][1] %= MOD;
            }


            if (v[end] > v[start]) {
                rememeber[start][end][1] += rememeber[start][end - 1][0];
                rememeber[start][end][1] %= MOD;
            }
        }
    }

    cout << (rememeber[0][n - 1][0] + rememeber[0][n - 1][1]) % MOD << endl;
}