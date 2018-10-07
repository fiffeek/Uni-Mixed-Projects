#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct preprocess_right_to_left {
    long long min_odd, min_even;
};

struct preprocess_left_to_right {
    long long max_odd, max_even, sums;
};

struct preproces_vec {
    preprocess_left_to_right maxs;
    preprocess_right_to_left mins;
};

bool is_even(long long x) {
    return x % 2 == 0;
}

vector<preproces_vec> preprocess(vector<long long> &v) {
    vector<preprocess_left_to_right> pv;
    long long max_odd = -1, max_even = -1, sums = 0;

    for (long long i = 0; i < v.size(); ++i) {
        sums += v[i];

        if (is_even(v[i]) && v[i] > max_even) {
            max_even = v[i];
        }

        if (!is_even(v[i]) && v[i] > max_odd) {
            max_odd = v[i];
        }

        pv.push_back({max_odd, max_even, sums});
    }

    vector<preprocess_right_to_left> pr;
    long long min_odd = -1, min_even = -1;

    for (long long i = v.size() - 1; i >= 0; --i) {
        pr.push_back({min_odd, min_even});

        if (min_odd == -1 && !is_even(v[i])) min_odd = v[i];
        if (min_even == -1 && is_even(v[i])) min_even = v[i];

        if (is_even(v[i]) && v[i] < min_even) {
            min_even = v[i];
        }

        if (!is_even(v[i]) && v[i] < min_odd) {
            min_odd = v[i];
        }
    }

    reverse(pr.begin(), pr.end());

    vector<preproces_vec> pp;
    pp.push_back({{-1, -1, 0}, {-1, -1}});

    for (long long i = 0; i < v.size(); i++) {
        pp.push_back({pv[i], pr[i]});
    }

    return pp;
}

long long return_sum(vector<long long> &v, vector<preproces_vec> &pv, long long k) {
    long long last_k = pv[pv.size() - 1].maxs.sums - pv[pv.size() - 1 - k].maxs.sums;
    if (!is_even(last_k)) return last_k; /// 1 [6]
    else {
        long long to_return_max = -1;
        int pp = pv.size() - 1 - k;

        if (pv[pv.size() - 1 - k].mins.min_even != -1 && pv[pv.size() - 1 - k].maxs.max_odd != -1) {
            to_return_max = last_k - pv[pv.size() - 1 - k].mins.min_even + pv[pv.size() - 1 - k].maxs.max_odd;
        }


        if (pv[pv.size() - 1 - k].mins.min_odd != -1 && pv[pv.size() - 1 - k].maxs.max_even != -1) {
            long long helper = last_k - pv[pv.size() - 1 - k].mins.min_odd + pv[pv.size() - 1 - k].maxs.max_even;

            if (helper > to_return_max) {
                to_return_max = helper;
            }
        }

        return to_return_max;
    }
}

int main() {
    long long n;
    cin >> n;

    vector<long long> c;

    for (long long i = 0; i < n; ++i) {
        long long aux;
        cin >> aux;

        c.emplace_back(aux);
    }

    vector<preproces_vec> pv = preprocess(c);

    long long m;
    cin >> m;

    while (m--) {
        long long k;
        cin >> k;

        cout << return_sum(c, pv, k) << endl;
    }
}
