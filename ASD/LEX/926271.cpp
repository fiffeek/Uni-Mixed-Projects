#include <iostream>
#include <cmath>
#include <cassert>

using namespace std;

using ull = long long;
const ull P = 105751;
const ull MOD = 15489079;
const ull MAX_N = 300005;

ull power[MAX_N];

void calculate_prefix_hash(const string &word, ull (&pref)[MAX_N]) {
    ull hash = 0;

    for (size_t i = 0; i < word.length(); ++i) {
        hash = ((hash * P) % MOD + (word[i] - 'a')) % MOD;
        pref[i] = hash;
    }
}

ull interval_hash(int start, int end, ull (&pref)[MAX_N]) {
    if (start - 1 < 0) {
        return pref[end];
    }

    long long to_delete = (power[end - start + 1] * pref[start -  1]) % MOD;

    if (to_delete < 0) {
        to_delete += MOD;
    }

    long long hash = (pref[end] - to_delete) % MOD;

    if (hash < 0) {
        hash += MOD;
    }

    return hash;
}


int main() {
    std::ios_base::sync_with_stdio(false);

    ull s = 1;
    for (int i = 0; i < MAX_N; ++i) {
        power[i] = s % MOD;
        s = (s * P) % MOD;
    }

    int n, q;
    string word;
    ull prefix_hash[MAX_N];

    cin >> n >> q >> word;
    calculate_prefix_hash(word, prefix_hash);

    for (int i = 0; i < q; ++i) {
        int a1, a2, b1, b2;
        cin >> a1 >> b1 >> a2 >> b2;
        a1--; a2--; b1--; b2--;

        if (b1 - a1 == b2 - a2 &&
            interval_hash(a1, b1, prefix_hash) == interval_hash(a2, b2, prefix_hash)) {
            cout << "=" << endl;
        } else {
            int e = min(b1 - a1, b2 - a2);
            int s = 0;

            while (s < e) {
                int middle = (s + e) / 2;

                if (interval_hash(a1, a1 + middle, prefix_hash) != interval_hash(a2, a2 + middle, prefix_hash)) {
                    e = middle;
                } else {
                    s = middle + 1;
                }
            }

            if (s == e) {
                if (interval_hash(a1, a1 + s, prefix_hash) == interval_hash(a2, a2 + s, prefix_hash)) {
                    s++;
                }
            }

            if (a1 + s <= b1 && a2 + s <= b2) {
                if (word[a1 + s] < word[a2 + s]) {
                    cout << "<" << endl;
                } else {
                    cout << ">" << endl;
                }
            } else {
                if (b1 - a1 > b2 - a2) {
                    cout << ">" << endl;
                } else {
                    cout << "<" << endl;
                }
            }
        }
    }
}