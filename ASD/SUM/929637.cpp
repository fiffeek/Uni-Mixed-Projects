#include <iostream>
#include <set>

using namespace std;
using ull = unsigned long long;

int main() {
    std::ios_base::sync_with_stdio(false);

    int n;
    cin >> n;
    set<ull> s;
    ull actual_sum = 0;

    for (int i = 0; i < n; ++i) {
        ull aux;
        cin >> aux;

        auto in = s.insert(aux);

        if (i > 0) {
            auto inserted = in.first;

            if (inserted != s.begin()) {
                auto it = prev(inserted, 1);
                auto after = next(inserted, 1);

                if (after != s.end()) {
                    actual_sum -= ((*after) - (*it)) * ((*after) - (*it));
                    actual_sum += ((*inserted) - (*it)) * ((*inserted) - (*it));
                    actual_sum += ((*after) - (*inserted)) * ((*after) - (*inserted));
                } else {
                    actual_sum += ((*inserted) - (*it)) * ((*inserted) - (*it));
                }
            } else {
                auto after = next(inserted, 1);

                if (after != s.end()) {
                    actual_sum += ((*after) - (*inserted)) * ((*after) - (*inserted));
                }
            }

            cout << actual_sum << endl;
        }
    }
}