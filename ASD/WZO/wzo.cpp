#include <iostream>
#include <vector>
#include <set>
#include <cstring>

using namespace std;

using ull = long long;
const ull P = 105751;
const ull MOD = 15489079;
const ull MAX_N = 100005;

struct edge {
    int to, length;
    ull hashed;

    edge(int to, int length, ull hashed)
            : to(to), length(length), hashed(hashed) {}

    bool operator<(const edge& lhs) const {
        if (to == lhs.to) {
            return lhs.hashed < hashed;
        }

        return lhs.to < to;
    }
};

ull power[MAX_N];
set<edge> G[MAX_N];
ull prefix_hash[MAX_N];

void calculate_prefix_hash(const char *word) {
    ull hash = 0;
    ull s = 1;
    size_t till = strlen(word);

    for (size_t i = 0; i < till; ++i) {
        hash = ((hash * P) % MOD + (word[i] - 'a')) % MOD;
        prefix_hash[i] = hash;

        power[i] = s % MOD;
        s = (s * P) % MOD;
    }
}

ull calc_hash(const char *word) {
    ull hash = 0;
    size_t till = strlen(word);

    for (size_t i = 0; i < till; ++i) {

        hash = ((hash * P) % MOD + (word[i] - 'a')) % MOD;
    }

    return hash;
}

ull interval_hash(int start, int end) {
    if (start > end) {
        return -1;
    }

    if (start - 1 < 0) {
        return prefix_hash[end];
    }

    long long to_delete = (power[end - start + 1] * prefix_hash[start - 1]) % MOD;

    if (to_delete < 0) {
        to_delete += MOD;
    }

    long long hash = (prefix_hash[end] - to_delete) % MOD;

    if (hash < 0) {
        hash += MOD;
    }

    return hash;
}

int main() {
    int n, m, from, to;
    scanf("%d %d", &n, &m);

    while (m--) {
        char str[101];
        scanf("%d %d %s", &from, &to, str);

        G[from].emplace(to, (int) strlen(str), calc_hash(str));
    }

    char word[MAX_N];
    scanf("%s", word);
    calculate_prefix_hash(word);
    int word_length = strlen(word);
    bool odp[MAX_N][101];

    for (int i = 0; i < word_length; ++i) {
        for (int j = 1; j <= n; ++j) {
            odp[i][j] = false;
        }
    }

    for (int j = 1; j <= n; ++j) {
        odp[0][j] = true;
    }

    for (int i = 0; i < word_length; ++i) {
        for (int j = 1; j <= n; ++j) {
            if (odp[i][j]) {
                for (const edge& e: G[j]) {
                    auto len = e.length;

                    if (i + len < MAX_N && e.hashed == interval_hash(i, i + len - 1)) {
                        odp[i + len][e.to] = true;

                        if (i + len == word_length) {
                            cout << "TAK" << endl;

                            return 0;
                        }
                    }
                }
            }
        }
    }

    cout << "NIE" << endl;
}